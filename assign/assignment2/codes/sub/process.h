#ifndef PROCESS_H
#define PROCESS_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>


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
extern unsigned int count_B;



// preprocess the S and B sequence
void preprocess(const string&);

// generate related sequence
void generete_related_sequence();

// generate the corresponding B_pr sequence.
void generate_B_pr();



#endif