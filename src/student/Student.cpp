//
// Created by christian on 6/9/23.
//

#include "Student.h"

#include <utility>
#include "../buffer/ByteBuffer.h"

/**
 * Turns the student data into an array of bytes
 * @return a byte array of the data
 */
std::vector<util::byte_t> Student::serialize() const {
  return util::join(
    {
      util::uintToBytes(util::swap_endian(this->sn)),
      util::uintToBytes(util::swap_endian((unsigned int) this->f_name.size())),
      std::vector<unsigned char>(this->f_name.begin(), this->f_name.end()),
      util::uintToBytes(util::swap_endian((unsigned int) this->l_name.size())),
      std::vector<unsigned char>(this->l_name.begin(), this->l_name.end()),
      util::uintToBytes(util::swap_endian(this->age)),
    }
  );
}

/**
 * reads a student from a vector
 * @param in the buffer to read from, will remove this student from it
 * @return the student read from the data
 */
Student Student::deserialize(ByteBuffer &b) {
  unsigned int sn = b.read_uint();
  unsigned int f_name_len = b.read_uint();
  std::string f_name = b.read_string(f_name_len);
  unsigned int l_name_len = b.read_uint();
  std::string l_name = b.read_string(l_name_len);
  unsigned int age = b.read_uint();

  return {sn, f_name, l_name, age};
}

/**
 * Updates the database object with the student
 * @param db the database to sync to
 */
void Student::sync(Database &db) {
  if (db.select("students", {"SN"}, this->sn).empty()) {
    db.insert("students", util::join({{{"SN", std::to_string(this->sn)}}, this->data()}));
    return;
  }

  db.update("students", this->data(), this->sn);
}

/**
 * Prints the student as a comma separated list to stdout
 * @param between what to put between the characters
 */
void Student::print(std::string between) const {
  std::cout << to_string(*this, between) << std::endl;
}

/**
 * Creates a key-value list of the student object
 * @return a map of the student object
 */
std::vector<std::pair<std::string, std::string>> Student::data() {
  return {
//        {"SN",     std::to_string(sn)},
    {"F_NAME", {f_name}},
    {"L_NAME", {l_name}},
    {"AGE",    std::to_string(age)},
  };
}

/**
 * Turns a student into a comma separated list in a string
 * @param in the student to convert
 * @param between to put between values
 * @return a string of the student
 */
std::string Student::to_string(const Student &in, const std::string &between) {
  std::stringstream ss;
  ss << in.sn << between
     << in.f_name << between
     << in.l_name << between
     << in.age;
  return ss.str();
}

/**
 * Gets a student from the database provided
 * @param db where to get the student from
 * @param sn the Student number associated with the student
 * @return
 */
Student Student::get(Database &db, int sn) {
  std::vector<std::string> data_str = db.select("students", {"F_NAME,L_NAME,AGE"}, sn);
  std::vector<std::string> data = util::split(data_str[0], ',');

  return {
    static_cast<unsigned int>(sn),
    data[0],
    data[1],
    static_cast<unsigned int>(std::stoi(data[2])),
  };
}

/**
 * Creates a student from a string
 * @param str the input string (comma separated values)
 * @return a student object
 */
Student Student::get(const std::string &str) {
  std::vector<std::string> data = util::split(str, ',');

  return {
    static_cast<unsigned int>(std::stoi(data[0])),
    data[1],
    data[2],
    static_cast<unsigned int>(std::stoi(data[3])),
  };
}
