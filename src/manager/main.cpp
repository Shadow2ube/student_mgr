#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "../database/Database.h"
#include "../student/Student.h"
#include "../buffer/ByteBuffer.h"
#include "../Packet.h"
#include <chrono>
#include <thread>

#define ERROR(x) std::cerr << x << std::endl;

int handle(ByteBuffer buffer, Database &db);

/**
 * Get all students in the database
 * @param db the database to get data from
 */
void get_students(Database &db) {
  std::vector<std::string> data = db.select_all("students", {"SN", "F_NAME", "L_NAME", "AGE"});
  std::vector<Student> students;
  for (const auto &i: data) {
    students.push_back(Student::get(i));
  }
  std::ofstream out("smo.pipe");
  for (auto i: util::uintToBytes(util::swap_endian((unsigned int) students.size()))) {
    out << i;
  }
  for (const auto &i: students) {
    for (auto j: i.serialize()) {
      out << j;
    }
  }

  out.close();
}

int main() {
  Database db("students.db");

  mkfifo("smi.pipe", S_IRWXU | S_IRWXG | S_IRWXO);
  mkfifo("smo.pipe", S_IRWXU | S_IRWXG | S_IRWXO);

  std::ifstream in_pipe("smi.pipe", std::ios::binary);

  while (true) {
    ByteBuffer buffer({std::istreambuf_iterator<char>(in_pipe), {}});
    if (buffer.empty()) continue;
    int ret = handle(buffer, db);

    if (ret != -1) return ret;

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}

/**
 * Reads the first parameter from a packet, which should be its packet type
 * @param buffer the buffer to read from
 * @return -1 if no error, anything else otherwise
 */
int handle(ByteBuffer buffer, Database &db) {
  switch (E_PACKET(buffer.read_int())) {
    case KILL: {
      return 1;
    }
    case NONE:
      break;
    case GET_STUDENTS: {
      DEBUG("GET_STUDENTS");
      get_students(db);

      break;
    }
    case ADD_STUDENT: {
      Student s = Student::deserialize(buffer);
      s.sync(db);
      break;
    }
    case REM_STUDENT:
      db.remove("students", buffer.read_uint());
      break;
    case MOD_STUDENT: {
      // TODO: modify student
      break;
    }
    case ATTENDANCE: {
      unsigned int size = buffer.read_uint();
      for (int i = 0; i < size; ++i) {
        std::cout << "SN: " << buffer.read_uint() << ": " << (buffer.read_bool() ? "HERE" : "AWAY") << std::endl;
      }
      break;
    }
    case INVALID:
    default: {
      std::ofstream out_pipe("smo.pipe", std::ios::binary);
      out_pipe << "Invalid packet" << std::endl;
      out_pipe.close();
      break;
    }
  }

  return -1;
}
