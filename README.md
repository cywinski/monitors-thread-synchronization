# Thread synchronization with use of monitors

## Table of Contents

- [About](#about)
- [Usage](#usage)

## About <a name = "about"></a>

Project for SOI (Operating Systems) course at Warsaw University of Technology. Project solves the Producer-Consumer problem using monitors. There are three types of Users:
- VIP Users (Producer)
- Normal Users (Producer)
- Reader (Consumer)

Messages sent by VIP Users have higher priority than messages sent by Normal Users.

This solution uses monitor which is implemented in file `monitor.h`.


## Usage <a name = "usage"></a>

To run a test, user needs to uncomment particular test function, which are called from `main()` function in file `main.cpp`.

Compiling `main.cpp`:
```
g++ -o main main.cpp -pthread
```
```
./main
```