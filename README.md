# Student Manager

## What is it?

Student Manager is a set of 2 programs developed for my grade 12 computer science summative.
It is meant to manage a class of students through the commandline and works in a
Client-Server architecture

## How does it work?

The Server receives and sends information to the client via named pipes at the location of the server itself.
The protocol is a simple in-house solution to a dumb problem. see [here](#whats-in-a-packet) for implementation details.

## What's in a packet
The first 4 bytes is the packet type

- ADD_STUDENT, MOD_STUDENT
    - student number (4 bytes)
    - first name length (4 byte)
    - first name (size of first name length)
    - last name length (4 bytes)
    - last name (size of last name length)
    - age (4 bytes)
- REM_STUDENT
    - student number (4 bytes)
- ATTENDANCE
    - size of list (4 bytes)
    - for each element:
        - student number (4 bytes)
        - here or not (1 byte)
- EVERYTHING ELSE
    - none


## Installation

### Linux
1. Install sqlite3 & cmake
```shell
sudo pacman -S sqlite3 cmake # Archlinux
apt install sqlite3 cmake    # Ubuntu
```
2. clone the repository
```shell
git clone https://github.com/Shadow2
```