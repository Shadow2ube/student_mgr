#include <fstream>
#include <iostream>
#include <utility>
#include "../student/Student.h"
#include "Menu.h"
#include "../Packet.h"

#define CLEAR "\033[2J\033[1;1H"

/**
 * Reads a string from stdin and returns it
 * @return a string from stdin
 */
std::string input() {
  std::string out;
  std::cin >> out;
  return out;
}

/**
 * Reads element of type T from stdin and returns it
 * @tparam T the type to return
 * @return the type T from stdin
 */
template<typename T>
T input() {
  T out;
  std::cin >> out;
  return out;
}

template<typename T>
T input(const std::string &prompt) {
  T out;
  std::cout << prompt;
  std::cin >> out;
  return out;
}

/**
 * Makes a char** into a std::vector<std::string>
 * @param size the size of the char**
 * @param values the array to convert
 * @return the array as a vector
 */
std::vector<std::string> to_vec(int size, char **values) {
  std::vector<std::string> out;
  for (int i = 0; i < size; ++i) out.emplace_back(values[i]);

  return out;
}

void sendToServer(std::initializer_list<std::vector<unsigned char>> in) {
  std::ofstream out("smi.pipe");
  for (auto i: in) {
    for (auto j: i) {
      out << j;
    }
  }
  out.close();
}

template<typename T>
std::vector<T> readArrFromServer() {
  std::ifstream in_pipe("smo.pipe");
  std::vector<T> out;
  ByteBuffer buffer({std::istreambuf_iterator<char>(in_pipe), {}});
  unsigned int size = buffer.read_uint();
  for (int i = 0; i < size; ++i) {
    out.push_back(Student::deserialize(buffer));
  }
  return out;
}

void addStudent(unsigned int sn, std::string f_name, std::string l_name, unsigned int age) {
  Student s{sn, std::move(f_name), std::move(l_name), age};
  std::ofstream out("smi.pipe");
  std::vector<util::byte_t> type = util::intToBytes(util::swap_endian(E_PACKET::ADD_STUDENT));
  for (auto byte: type) {
    out << byte;
  }
  for (auto byte: s.serialize()) {
    out << byte;
  }
  out << std::endl;
  out.close();
}

/**
 * Take attendance of students and send it to the server
 */
void attendance() {
  sendToServer({util::intToBytes(util::swap_endian((unsigned int) E_PACKET::GET_STUDENTS))});
  std::vector<Student> students = readArrFromServer<Student>();
  std::unordered_map<unsigned int, bool> here;
  std::cout << "Attendance: " << std::endl;

  for (const auto &student: students) {
    auto h = input<bool>("Is " + student.f_name + " " + student.l_name + " here? (1 for here, 0 for away): ");
    here.insert({student.sn, h});
  }

  for (auto [sn, h]: here) {
    std::cout << sn << ": " << (h ? "here" : "away") << std::endl;
  }

  std::vector<std::vector<unsigned char>> out;

  for (auto [sn, h]: here) {
    out.push_back(util::join({util::uintToBytes(util::swap_endian(sn)), {(unsigned char) h}}));
  }

  sendToServer(
    {
      util::intToBytes(util::swap_endian(E_PACKET::ATTENDANCE)),
      util::uintToBytes(util::swap_endian((unsigned int) here.size())),
      util::join(out)
    }
  );
}

/**
 * Remove a student by their student number
 * @param sn the student number to remove
 */
void remStudent(unsigned int sn) {
  sendToServer(
    {
      util::intToBytes(util::swap_endian(E_PACKET::REM_STUDENT)),
      util::uintToBytes(util::swap_endian(sn)),
    }
  );
}

auto main() -> int {
  Menu menu{
    {
      .name = "BASE",
      .type = MenuItem::FOLDER,
      .folder = {
        {
          .name = "Manage Students",
          .type = MenuItem::FOLDER,
          .folder = {
            {
              .name = "List students",
              .type = MenuItem::FUNCTION,
              .function = []() {
                sendToServer({util::intToBytes(util::swap_endian(E_PACKET::GET_STUDENTS))});
                std::vector<Student> students = readArrFromServer<Student>();

                std::cout << CLEAR << std::endl;
                for (const auto &i: students) {
                  i.print("  :  ");
                }
                std::cout << "Any key to continue..." << std::endl;
                std::getchar();
                std::cin.get();
              }
            },
            {
              .name = "Add student",
              .type = MenuItem::FUNCTION,
              .function = []() {
                auto sn = input<unsigned int>("Student number: ");
                auto f_name = input<std::string>("First name: ");
                auto l_name = input<std::string>("Last name: ");
                auto age = input<unsigned int>("Age: ");
                addStudent(sn, f_name, l_name, age);
              }
            },
            {
              .name = "Remove student",
              .type = MenuItem::FUNCTION,
              .function = []() {
                auto sn = input<int>("Enter a Student Number: ");
                remStudent(sn);
              }
            },
            {
              .name = "Modify student",
              .type = MenuItem::FUNCTION,
            }
          }
        },
        {
          .name = "Attendance",
          .type = MenuItem::FUNCTION,
          .function = []() {
            std::cout << CLEAR << std::endl;
            attendance();
            std::cout << "Any key to continue..." << std::endl;
            std::getchar();
            std::cin.get();
          }
        },
        {
          .name = "Quit",
          .type = MenuItem::FUNCTION,
          .function = []() {
            exit(0);
          }
        }
      }
    }
  };

  menu.print();

  int i;
  while ((i = input<int>()) != 'x') {
    menu.select(i);
    std::cout << CLEAR << std::endl;

    menu.print();
  }
  return 0;
}