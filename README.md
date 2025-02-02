Storage Manager Project

Overview

This project implements a simple storage manager in C that allows managing files and handling basic operations like creating, opening, reading, and writing to binary files. It also includes error handling mechanisms and test cases to verify its functionality.

Files in the Project

1. Source Files

storage_mgr.c - Implements the storage manager functions.

storage_mgr.h - Header file containing function prototypes and data structures.

dberror.c - Handles error messages for the storage manager.

dberror.h - Defines error codes and messages.

2. Test Files

test_assign1_1.c - Contains test cases for verifying storage manager functionality.

test_program.c - Another test file for verifying storage manager operations.

test_helper.h - Provides helper functions for running test cases.

3. Build Files

Makefile - Automates the compilation process.

test_program.exe - Compiled executable (if running on Windows).

testfile.bin - Sample binary file used for testing.

4. Configuration Files

tasks.json - Contains task configurations for the project.

Features Implemented

File creation and deletion

Opening and closing files

Reading from and writing to files

Error handling using predefined error codes

Unit tests for verifying correctness

How to Build and Run

1. Using Makefile

To compile the project, simply run:

make

This will generate the test_program executable.

2. Running the Tests

After compiling, execute the test program using:

./test_program

If using Windows, run:

test_program.exe

3. Manual Compilation (If Makefile is Not Used)

gcc -o test_program test_program.c storage_mgr.c dberror.c -Wall -Wextra
./test_program

Troubleshooting

If you encounter permission errors, try:

chmod +x test_program
./test_program

If the compilation fails, ensure that gcc is installed and that all necessary files are present.

License

This project is for educational purposes and is open for modifications and improvements.

Author

Developed as part of a database management system assignment to implement file-based storage operations in C.



