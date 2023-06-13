//
// Created by christian on 6/7/23.
//

#include "Database.h"

#include <utility>
#include "../util.h"

/**
 * Should not be called by anything except s3_exec
 * Callback for return values of s3_exec, turns them into values for a vector
 * @param vec the void* of the vector to add values to
 * @param argc the # of arguments
 * @param argv the column values
 * @param _ the column names
 * @return
 */
int s3_exec_callback(void *vec, int argc, char **argv, char **_) {
  DEBUG(argc);
  std::string p;
  for (int i = 0; i < argc - 1; i++) {
    p += argv[i] ? argv[i] : "NULL";
    p += ',';
  }
  p += argv[argc - 1] ? argv[argc - 1] : "NULL";
  auto &out = *static_cast<std::vector<std::string> *>(vec);
  out.push_back(p);
  return 0;
}

/**
 * Wrapper function for sqlite3_exec, should not be called by any other function by query()
 * @param query the string to query the database with
 * @return a vector of the returned values from the database
 */
std::vector<std::string> Database::s3_exec(const std::string &query) {
  DEBUG(query);
  std::vector<std::string> temp;
  char *zErr = nullptr;

  error_code = sqlite3_exec(db, query.c_str(), s3_exec_callback, &temp, &zErr);
  DEBUG("next is error code");
  DEBUG(error_code);

  if (error_code != 0) {
    err_logger({"SQL error: ", zErr});
    sqlite3_free(zErr);
    return {};
  } else {
    logger({"Query completed: ", query});
  }
  sqlite3_free(zErr);


  std::cout << "s3_exec: \n";
  for (auto i: temp) {
    std::cout << i << std::endl;
  }
  std::cout << "s3_exec done ^" << std::endl;

  return temp;
}

/**
 * ***Should not be used unless you know what you're doing***
 * Sends a query to the database
 * @param query the SQL query to send to the database
 * @return a vector of string values that were returned by the databse
 */
std::vector<std::string> Database::query(const std::string &query) {
  DEBUG(query);
  auto out = s3_exec(query);
  if (!out.empty()) return out;
  return {};
}

/**
 * Creates a new element in a table
 * @param where the table to add to
 * @param data the data to add to the table
 */
void Database::insert(const std::string &where, std::vector<std::pair<std::string, std::string>> data) {
  auto [keys, vals] = util::map_to_str_list(std::move(data));
  std::stringstream ss;
  ss << "insert into " << where
     << " (" << keys << ") VALUES (" << vals << ");";

  query(ss.str());
}

/**
 * Selects a specific row from the table in regards to sn
 * @param table the table to select from
 * @param what what to select (ie. {"f_name", "l_name", "age"})
 * @param sn the SN to match against
 * @return
 */
std::vector<std::string> Database::select(const std::string &table, std::vector<std::string> what, unsigned int sn) {
  std::stringstream ss;
  ss << "select " << util::to_string(std::move(what)) << " from " << table << " where SN=" << sn << ';';
  return query(ss.str());
}

/**
 * Selects all rows in a table
 * @param table the table to select from
 * @param what what to select (ie. {"f_name", "l_name", "age"})
 * @return
 */
std::vector<std::string> Database::select_all(const std::string &table, std::vector<std::string> what) {
  std::stringstream ss;
  ss << "select " << util::to_string(std::move(what)) << " from " << table << ';';
  return query(ss.str());
}

/**
 * Updates a specific row in the database based on what SN is given
 * @param where what table to update
 * @param data the data to update the row with
 * @param sn the SN to match against
 */
void Database::update(const std::string &where, Database::map_t data, unsigned int sn) {
  std::stringstream ss;
  ss << "update " << where << " set ";
  for (auto [key, val]: data) {
    if (key == data[data.size() - 1].first) {
      ss << key << "='" << val << "' ";
      break;
    }
    ss << key << "='" << val << "', ";
  }
  ss << "where SN='" << sn << "'; ";
  ss << "SELECT * FROM " << where << " WHERE SN='" << sn << "';";
  this->query(ss.str());
}

void Database::remove(const std::string &table, unsigned int sn) {
  std::stringstream ss;
  ss << "DELETE FROM " << table << " WHERE SN=" << sn << ";";
  logger({ss.str()});
  this->query(ss.str());
}
