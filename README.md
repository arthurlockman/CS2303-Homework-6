# CS2303-Homework-6
Homework 6 for WPI CS 2303 by Arthur Lockman.

##Part 3: Timing

It seems as though the binary read is the most efficient way to copy files. The buffer size is most efficient the larger it is; I found that the difference between a buffer size of 2048 bytes is the sweet spot for copy mode 3. 

##Compiling

To compile the program, simply change into the project directory and type:

    make

To clean previous builds, type the following command:

    make clean

To build the doxygen documentation, type the following command:

    make docs

#Programs

This assignment produces two programs, both of which have similar functionality. cptest copies one file to another with options, and cptest2 is essentially the same as cptest except with more robustness and flexibility. These programs are both detailed below.

##cptest

cptest copies one file to another. It can also copy a file into a folder with the file as the same name. It takes parameters in the following form:

    user@computer:~/CS2303-Homework-6$ ./cptest
    Usage: ./cptest infile outfile [copymode = 1, 2, 3] [buffer size]
    Copy mode and buffer size are optional.

The buffer size option is optional, and only valid in copy mode 3. The copy modes are detailed below.

* Mode 1: copy files using character file open mode.
* Mode 2: copy files in binary mode using a fixed buffer size.
* Mode 3: copy files in binary mode using a variable buffer size, specified by the buffer size parameter.

The program will 
Examples of running the program for each of the copy modes are shown below. Time outputs will differ for each file and each system.

###Copy Mode 1

    user@computer:~/CS2303-Homework-6$ ./cptest README.md rd.md 1
    1425269066 seconds, 893589 microseconds
    1425269066 seconds, 893870 microseconds
    Time difference: 0 seconds, 281 microseconds

###Copy Mode 2

    user@computer:~/CS2303-Homework-6$ ./cptest README.md rd.md 2
    1425269093 seconds, 764728 microseconds
    1425269093 seconds, 764778 microseconds
    Time difference: 0 seconds, 50 microseconds

###Copy Mode 3

    user@computer:~/CS2303-Homework-6$ ./cptest README.md rd.md 3 4096
    1425269129 seconds, 664718 microseconds
    1425269129 seconds, 665213 microseconds
    Time difference: 0 seconds, 495 microseconds

##cptest2

cptest2 is almost identical to cptest, except that it can copy any number of files into a directory as well as copying one file to another file and one file into a directory. The copy modes are exactly the same as for cptest. Details on usage are below.

    user@computer:~/CS2303-Homework-6$Usage: ./cptest2 [-cb] filesource filedestination
    Copy mode and buffer size are optional.

Command line options are detailed below.

* -c The copy mode to use. Defaults to 1 if not specified.
* -b The buffer size to use for copy mode 3. Ignored if copy mode 3 is not set.

Examples for each copy mode are shown below.

###Copy Mode 1

    user@computer:~/CS2303-Homework-6$ ./cptest2 README.md -c rd.md -b 1
    1425269066 seconds, 893589 microseconds
    1425269066 seconds, 893870 microseconds
    Time difference: 0 seconds, 281 microseconds

###Copy Mode 2

    user@computer:~/CS2303-Homework-6$ ./cptest2 README.md -c rd.md -b 2
    1425269093 seconds, 764728 microseconds
    1425269093 seconds, 764778 microseconds
    Time difference: 0 seconds, 50 microseconds

###Copy Mode 3

    user@computer:~/CS2303-Homework-6$ ./cptest2 README.md rd.md -c 3 -b 4096
    1425269129 seconds, 664718 microseconds
    1425269129 seconds, 665213 microseconds
    Time difference: 0 seconds, 495 microseconds

###Wildcards

This program also accepts wildcard inputs. For instance, calling the following command in the root level directory creates this directory output.

    user@computer:~/CS2303-Homework-6$ ls
    Doxyfile  README.md cptest    cptest.c  cptest.o  cptest2   cptest2.c cptest2.o makefile  rd.md     test
    user@computer:~/CS2303-Homework-6$ ./cptest2 -c 3 -b 2048 * test
    1425271341 seconds, 483458 microseconds
    1425271341 seconds, 484707 microseconds
    Time difference: 0 seconds, 1249 microseconds
    user@computer:~/CS2303-Homework-6$ ls test
    Doxyfile  README.md cptest    cptest.c  cptest.o  cptest2   cptest2.c cptest2.o makefile  rd.md     test

This can also work with multiple files specified on the command line instead of a wildcard. 