//
// Created by christian on 6/7/23.
//

#ifndef STUDENT_MGR_DATABASE_H
#define STUDENT_MGR_DATABASE_H


#include <sqlite3.h>
#include <string>
#include <functional>
#include <iostream>
#include "../util.h"

//static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
//    int i;
//    for (i = 0; i < argc; i++) {
//        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
//}

class Database {
private:
  sqlite3 *db = nullptr;

  std::function<void(std::vector<std::string>)> logger = [](auto v) {
    for (const auto &s: v)
      std::cout << s;
    std::cout << std::endl;
  };
  std::function<void(std::vector<std::string>)> err_logger = [](auto v) {
    for (const auto &s: v)
      std::cerr << "ERROR: " << s;
    std::cerr << std::endl;
  };

  int error_code = 0;


  std::vector<std::string> s3_exec(const std::string &query);

public:
  typedef std::vector<std::pair<std::string, std::string>> map_t;

  explicit Database(const std::string &db_name) {
    error_code = sqlite3_open(db_name.c_str(), &db);
    if (error_code) {
      err_logger({"Could not open database : ", sqlite3_errmsg(db)});
    } else {
      logger({"Opened database successfully"});
    }
  }

  ~Database() {
    sqlite3_close(db);
  }

  [[nodiscard]] int err() const {
    return error_code;
  }

  std::vector<std::string> query(const std::string &query);

  void insert(const std::string &where, map_t data);

  void update(const std::string &where, map_t data, unsigned int sn);

  void remove(const std::string& table, unsigned int sn);

  std::vector<std::string> select(const std::string &table, std::vector<std::string> what, unsigned int sn);

  std::vector<std::string> select_all(const std::string &table, std::vector<std::string> what);
};


#endif //STUDENT_MGR_DATABASE_H
