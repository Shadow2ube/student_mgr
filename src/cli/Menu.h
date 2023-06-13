//
// Created by christian on 6/11/23.
//

#ifndef STUDENT_MGR_MENU_H
#define STUDENT_MGR_MENU_H

#include <variant>
#include "../../lib/json.hpp"


struct MenuItem {
  std::string name;
  enum {
    NONE = 0, FOLDER, FUNCTION, enum_length
  } type = NONE;
  std::vector<MenuItem> folder{};
  std::function<void()> function = []() {
    std::cout << "this is the default function" << std::endl;
  };


  [[nodiscard]] std::vector<MenuItem> select() const {
    if (type == NONE) {
      std::cout << "This menu item doesn't exist" << std::endl;
      exit(1);
    }
    if (type == FUNCTION) {
      function();
      return {};
    }
    return folder;
  }

  MenuItem get_child(int i) {
    return folder[i];
  }

  [[nodiscard]] std::vector<MenuItem> children() const {
    return folder;
  }
};

struct Menu {
  MenuItem base;
  MenuItem current = base;

public:
  void select(int i) {
    if (i < 0 || i >= MenuItem::enum_length) {
      current = base;
      return;
    }

    if (current.get_child(i).type == MenuItem::FUNCTION) {
      current.get_child(i).function();
      return;
    }
    current = current.get_child(i);
  }

  void print() const {
    std::vector<MenuItem> children = current.children();
    int i = 0;
    for (; i < children.size(); ++i) {
      std::cout << i << ": " << children[i].name << std::endl;
    }
    std::cout << (i + 1) << ": Home" << std::endl;
  }
};

#endif //STUDENT_MGR_MENU_H
