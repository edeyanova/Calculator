# Calculator
A client server application which uses posix shared memory.

Task Objectives

Use C/C++ programming language to develop 1 server application and 2 client applications for Linux that communicate via IPC (shared memory) and perform basic operations:

•	math operations with 32-bit signed integers

i.	add 2 numbers

ii.	subtract 2 numbers

iii.	multiply 2 numbers

iv.	divide 2 numbers

•	string operations (strings with length between 1 and 16 characters)

i.	concatenate 2 strings

ii.	search for substring within a string (return the position of the substring)


Implementation Guidelines

Communication Library

The developer must implement a shared library (.so) for Inter-Process Communication (IPC). This includes:

•	creating a dedicated shared memory for the data transfer (IPC);

•	creating shared memory mapping between the processes;

•	defining a format for data transfers over the shared memory communication channel;

implementing synchronous (add, subtract) and asynchronous (multiply, divide, concatenate, search) function calls over the communication channel to the clients;

•	synchronizing the access to the shared memory between the processes (use mutexes, semaphores, named semaphores, conditional variables);

•	using separate POSIX threads for each message processing (one thread for each command).


Server Application

The server must use the communication library to exchange requests and responses with his clients. The server implementation must include separate POSIX thread for different command types (1 thread for math & 1 thread for string operations).


Client Applications

The 2 client applications must provide the following functionality:

•	the 1st client application must implement the operations “add 2 numbers”, “multiply 2 numbers” and “concatenate 2 strings”;

•	the 1st client application must use the shared library directly (rely on the process loader for loading & linking);

•	the 2nd client application must implement the operations “subtract 2 numbers”, “divide 2 numbers” and “search for substring within a string”;

•	the 2nd client application must load the shared library in runtime (using dlopen(), dlclose(), dlsym()).

The 2 applications must provide a simple command line user interface for demonstrating the functionality.

Project files

•	ipc_defs.hpp (header file that will be used both from the server application and from the libcomm.so library)
•	comm_lib.hpp
•	comm_lib.cpp
•	server.cpp
•	client1.cpp
•	client2.cpp
•	(optional) Makefile

Project compilation and demonstration
The whole project (3 applications) must be compiled with GCC and GNU Make in the Linux text console by typing the following command:

make clean all

The compilation process must produce the following files for the Linux applications (including the shared library):
•	libcomm.so
•	server
•	client1
•	client2

These applications can be demonstrated in a Linux text console with a simple command-line user interface:
For client 1
(1)	Add 2 numbers
(2)	Multiply 2 numbers
(3)	Concatenate 2 strings
(4)	Exit

Enter command: 1
Enter number 1: 42
Enter number 2: 37
Sending request…
Receiving request…
Result is 79!

Another usecase for Client 1 could be(can be simulated with a small delay in the server while processing two numbers multiplication):

(1)	Add 2 numbers

(2)	Multiply 2 numbers

(3)	Concatenate 2 strings

(4)	Exit


Enter command: 2

Enter number 1: 7

Enter number 2: 8

Sending request (7*8) …


(1)	Add 2 numbers

(2)	Multiply 2 numbers

(3)	Concatenate 2 strings

(4)	Exit

Enter command: 3

Enter number 1: a

Enter number 2: b

Sending request concat(a,b) …

(1)	Add 2 numbers

(2)	Multiply 2 numbers

(3)	Concatenate 2 strings

(4)	Exit


Receiving request from (7*8) command

Result from (7*8) is 56!

Receiving request from concat(a,b) command

Result from concat(a,b) is ab!

Enter command:



For client 2

(1)	Subtract 2 numbers

(2)	Divide 2 numbers

(3)	Find substring in a string

(4)	Exit

Following usecase can be referred:

Enter command: 3

Enter substring: lo

Enter string: Hello!

Sending request…

Receiving request…

Result is : 3 (if substring was not found print NULL)

