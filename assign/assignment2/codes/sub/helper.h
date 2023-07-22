#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <cstdio>
#include <iostream>


using namespace std;

// some useful general constants
extern const int CHARWIDTH;

// using for spliting block 
extern unsigned int BLOCK_SIZE, CHAR_BLOCK_SIZE, BIT_BLOCK_SIZE_OF_UINT;


// some useful general variables
extern vector<char> S;
extern vector<unsigned int> B, B_pr;


// some useful built-in variables
extern unsigned int CS_table[];
extern unsigned int Length_table[];
extern vector<unsigned int> select_S, rank_B, rank_B_pr;
extern unsigned int S_block_count, rank_B_block_count, rank_B_pr_block_count;
extern unsigned int count_B_pr_1, count_B_1;

/** 
 * this file is for some helper functions during the assignment
 * by using the following function efficiently, we can speed up 
 * our searching and quering time.
 */


// output the B in vector
void output_B_sequence();

// output the S in vector 
void output_S_sequence();

// print out the length table of S 
void output_length_table_S();

// output the B_pr in vector
void output_B_pr_sequence();

// print out the rankB vector
void output_rankB();

// print out the rankB_pr vector
void output_rankB_pr();

void output_CS_table();

#endif