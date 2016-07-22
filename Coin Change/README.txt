CS 325 Project 2 Coin Change Using 3 different Algorithms

Project Group 13 CS325 SU2015
Author: 
Avant Mathur
Nicholas Madani
Tatyana Vlaskin


We used C++ for our script. 
There are 3 .cpp files with 3 different algorithms:
1. changedp.cpp
2. changegreedy.cpp
3. changeslow.cpp

Each algorithm file needs to be compiled and run separable
To compile files:
1. g++ -g -std=c++0x changedp.cpp -o name1
2. g++ -g -std=c++0x changegreedy.cpp -o name2
3. g++ -g -std=c++0x changeslow.cpp -o name3

where name1, name2 and name3 can we chosen by whoever compiles the program

To run programs:

User runs the programs on the command-line, specifying a file ([input filename].txt) 
in which the first line contains the array V, formatted as [1, 5, 10, 25], denominations in increasing order.
The next line contains one integer value for which we must make change.
An input file: coins1.txt is attached


These 3 files are identical, they just have different names.

Each program needs to be run separately.
EXAMPLE:
1. ./name1 Coin1.txt
2. ./name2 Coin1.txt
3. ./name3 Coin1.txt

 
Program output is a file named [input filename]change.txt where [input filename].txt is the input filename, 
and it is formatted with one change result and the minimum number of coins m, per line.
EXAMPLE:
Coin1change.txt

please NOTE [!!!!!] slow changes append to the file if it already exists, while dynamic and greedy replaces results. 


