cmake_minimum_required(VERSION 3.25)
project(student_mgr)

set(CMAKE_CXX_STANDARD 17)

add_executable(student_mgr
        src/manager/main.cpp
        src/util.h
        src/Packet.h
        lib/json.hpp
        src/buffer/ByteBuffer.h
        src/database/Database.cpp src/database/Database.h
        src/student/Student.cpp src/student/Student.h
        )
add_executable(student_mgr_cli
        src/cli/main.cpp
        src/util.h
        src/Packet.h
        lib/json.hpp
        src/database/Database.cpp src/database/Database.h
        src/student/Student.cpp src/student/Student.h
        src/cli/Menu.h)


include_directories(/usr/include)
link_directories(/usr/lib)
target_link_libraries(student_mgr sqlite3)
target_link_libraries(student_mgr_cli sqlite3)
