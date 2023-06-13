//
// Created by christian on 6/11/23.
//

#ifndef STUDENT_MGR_PACKET_H
#define STUDENT_MGR_PACKET_H

/**
 * The types of packets that can be sent
 */
enum E_PACKET {
  KILL = -2,
  INVALID = -1,
  NONE = 0,
  ADD_STUDENT,
  REM_STUDENT,
  MOD_STUDENT,
  GET_STUDENTS,
  ATTENDANCE,
};

#endif //STUDENT_MGR_PACKET_H
