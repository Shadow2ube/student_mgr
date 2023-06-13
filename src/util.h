//
// Created by christian on 6/7/23.
//

#ifndef STUDENT_MGR_UTIL_H
#define STUDENT_MGR_UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <cstring>

/**
 * Whether or not to show debug messages
 */
#define SHOULD_DEBUG 1

/**
 * A debugging function with file locations
 */
#define DEBUG(x)                                                                      \
{                                                                                     \
  if (SHOULD_DEBUG) {                                                                \
    std::cout << "[" << __FILE_NAME__ << "][" << __func__ << "]: " << x << std::endl; \
  }                                                                                   \
}

/**
 * Utility functions that have nowhere else to go
 */
namespace util {
  typedef unsigned char byte_t;

  /**
   * Splits a string by a deliminator
   * @param in the string to split
   * @param delim the character to split by
   * @return a vector of the split values
   */
  inline std::vector<std::string> split(const std::string &in, char delim) {
    std::stringstream ss(in);

    std::vector<std::string> out;
    std::string item;
    while (std::getline(ss, item, delim)) {
      out.push_back(item);
    }

    return out;
  }

  /**
   * Turns a map into 2 comma separated lists
   * @param in the map in
   * @param escape_key whether to put '' around keys
   * @param escape_val whether to put '' around values
   * @return
   */
  inline std::tuple<std::string, std::string>
  map_to_str_list(std::vector<std::pair<std::string, std::string>> in,
                  bool escape_key = false,
                  bool escape_val = true) {
    std::string keys;
    std::string vals;

    for (auto [k, v]: in) {
      auto key = escape_key ? "'" + k + "'" : k;
      auto val = escape_val ? "'" + v + "'" : v;
      if (k == in[in.size() - 1].first) {
        keys += key;
        vals += val;
        break;
      }
      keys += key + ',';
      vals += val + ',';
    }

    return {keys, vals};
  }

  /**
   * Turn a vector into a comma separated string
   * @tparam T any type
   * @param in the vector to convert
   * @return a comma separated array of values from the vector
   */
  template<typename T>
  inline std::string to_string(std::vector<T> in) {
    std::stringstream ss;
    for (auto i: in) {
      if (i == in[in.size() - 1]) {
        ss << i;
        break;
      }
      ss << i << ',';
    }
    return ss.str();
  }

  /**
   * Turns a uint into an array of bytes
   * @param in
   * @return
   */
  inline std::vector<byte_t> uintToBytes(unsigned int in) {
    std::vector<byte_t> arr(4);
    for (int i = 0; i < 4; ++i) {
      arr[3 - i] = in >> (i * 8);
    }
    return arr;
  }

  /**
   * Turns a uint into an array of bytes
   * @param in
   * @return
   */
  inline std::vector<byte_t> intToBytes(int in) {
    std::vector<byte_t> arr(4);
    for (int i = 0; i < 4; ++i) {
      arr[3 - i] = in >> (i * 8);
    }
    return arr;
  }

  /**
   * Turns an array of bytes (little endian) to a uint
   * @param in the vector to convert
   * @return the uint read
   */
  inline unsigned int bytesToUint(std::vector<byte_t> in) {
    unsigned int out;
    std::memcpy(&out, in.data(), sizeof(unsigned int));
    return out;
  }

  /**
   * Turns an array of bytes (little endian) to an int
   * @param in the vector to convert
   * @return the int read
   */
  inline int bytesToInt(std::vector<byte_t> in) {
    int out;
    std::memcpy(&out, in.data(), sizeof(int));
    return out;
  }


  /**
   * from https://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c
   * Swaps the endianess of a number
   */
  template<typename T>
  T swap_endian(T u) {

    union {
      T u;
      unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
      dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
  }

  /**
   * Join a series of vectors together
   * @tparam T any type, the data in the vectors
   * @param in a set of vectors to join together
   * @return a concatenated vector
   */
  template<typename T>
  std::vector<T> join(std::vector<std::vector<T>> in) {
    std::vector<T> out;
    for (auto i: in) {
      out.insert(out.end(), i.begin(), i.end());
    }
    return out;
  }

  /**
   * Join a series of vectors together
   * @tparam T any type, the data in the vectors
   * @param in a set of vectors to join together
   * @return a concatenated vector
   */
  template<typename T>
  std::vector<T> join(std::initializer_list<std::vector<T>> in) {
    std::vector<T> out;
    for (auto i: in) {
      out.insert(out.end(), i.begin(), i.end());
    }
    return out;
  }
}

#endif //STUDENT_MGR_UTIL_H
