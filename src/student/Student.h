//
// Created by christian on 6/9/23.
//

#ifndef STUDENT_MGR_STUDENT_H
#define STUDENT_MGR_STUDENT_H


#include <string>
#include <variant>
#include "../database/Database.h"
#include "../buffer/ByteBuffer.h"

struct Student {
  unsigned int sn;
  std::string f_name;
  std::string l_name;
  unsigned int age;

  static Student get(Database &db, int sn);

  static Student get(const std::string &str);

  static std::string to_string(const Student &in, const std::string &between = ",");

  std::vector<std::pair<std::string, std::string>> data();

  void print(std::string between = ",") const;

  void sync(Database &db);

  [[nodiscard]] std::vector<util::byte_t> serialize() const;

  static Student deserialize(ByteBuffer &in);

};


#endif //STUDENT_MGR_STUDENT_H
