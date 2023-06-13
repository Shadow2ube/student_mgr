//
// Created by christian on 6/11/23.
//

#ifndef STUDENT_MGR_BYTEBUFFER_H
#define STUDENT_MGR_BYTEBUFFER_H


#include <utility>
#include <vector>
#include "../util.h"

/**
 * A stream-like buffer that will consume data once requested
 * Reads bytes into types
 */
class ByteBuffer {
  std::vector<util::byte_t> data;

public:
  /**
   * Creates a ByteBuffer
   * @param data a vector of bytes
   */
  explicit ByteBuffer(std::vector<util::byte_t> data) : data(std::move(data)) {}

  /**
   * Reads an unsigned int from the buffer
   * consumes 4 bytes
   * @return the value read
   */
  unsigned int read_uint() {
    unsigned int out = util::bytesToUint({data.begin(), data.begin() + 4});
    data.erase(data.begin(), data.begin() + 4);
    return out;
  }

  /**
   * Reads an int from the buffer
   * consumes 4 bytes
   * @return the value read
   */
  int read_int() {
    int out = util::bytesToInt({data.begin(), data.begin() + 4});
    data.erase(data.begin(), data.begin() + 4);
    return out;
  }

  /**
   * Reads a string from the buffer
   * consumes {size} bytes
   * @param size how long the string is
   * @return
   */
  std::string read_string(size_t size) {
    std::string out;
    for (int i = 0; i < size; ++i) {
      out += (char) data[i];
    }
    data.erase(data.begin(), data.begin() + (long) size);
    return out;
  }

  /**
   * Reads a bool from the buffer
   * consumes 1 byte
   * @return
   */
  bool read_bool() {
    auto out = (bool) data[0];
    data.erase(data.begin());
    return out;
  }

  /**
   * Whether the buffer is empty or not
   * @return true if empty, false if not
   */
  bool empty() {
    return data.empty();
  }

};


#endif //STUDENT_MGR_BYTEBUFFER_H
