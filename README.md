# Thread synchronization with use of monitors

## Table of Contents

- [About](#about)
- [Usage](#usage)

## About <a name = "about"></a>

Project for SOI (Operating Systems) course at Warsaw University of Technology. Program is implemented in C++ language. Project solves the Producer-Consumer problem using monitors. There are three types of Users:
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

### Tests
#### `test_both_users()` creates:
- 2 threads of Normal Users
- 1 thread of VIP User
- 1 thread of reader

In this test communication of both types of Users is tested. Many messages sent by Normal Users, despite being sent before VIP messages, are being read after VIP messages, because VIP messages have higher priority. All messages are putting to queue one at a time.

#### `test_normal_users()` creates:
- 2 threads of Normal Users
- 1 thread of reader

In this test communication of one type of Users is tested. Messages are being read in the order of being putted to queue.

#### `test_vip_users()` creates:
- 2 threads of VIP Users
- 1 thread of reader

In this test communication of one type of Users is tested. Messages are being read in the order of being putted to queue.

#### `test_empty_buffer_vip()` creates:
- 1 thread of VIP User
- 1 thread of reader

In this test reader wants to read a message from empty buffer. Reader is blocked until first message is putted to queue.

#### `test_empty_buffer_normal()` creates:
- 1 thread of Normal User
- 1 thread of reader

In this test reader wants to read a message from empty buffer. Reader is blocked until first message is putted to queue.

#### `test_full_buffer_vip()` creates:
- 1 thread of VIP User
- 1 thread of reader

In this test VIP User wants to put a message to full buffer. VIP User is blocked until first message is removed from queue.

#### `test_full_buffer_normal()` creates:
- 1 thread of Normal User
- 1 thread of reader

In this test Normal User wants to put a message to full buffer. Normal User is blocked until first message is removed from queue.