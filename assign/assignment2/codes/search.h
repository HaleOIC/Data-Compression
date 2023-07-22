#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <iostream>
#include <map>


using namespace std;

extern const int CHARWIDTH;


// using for spliting block 
extern unsigned int BLOCK_SIZE, CHAR_BLOCK_SIZE, BIT_BLOCK_SIZE_OF_UINT;


// using for describing the B, B_pr and S
extern vector<char> S;
extern vector<unsigned int> B, B_pr;


// some useful built-in variables
extern unsigned int CS_table[];
extern unsigned int Length_table[];
extern vector<unsigned int> select_S, rank_B, rank_B_pr;
extern unsigned int S_block_count, rank_B_block_count, rank_B_pr_block_count;
extern unsigned int count_B_pr_1, count_B_1, count_B;



char get_Li(unsigned int col);

// search for the target string and print them out
void search_str(const string&);

// backward search key function during the assignment
pair<int, int> backward_search(const string&);

// make a transfer from L to F and b to bb
int reverse_BWT(unsigned int);

int find_index_record(int pos);

string match_new_str(int);


// helper function for searching
int rankB(unsigned int);
int rankS(char, unsigned int);
int selectS(char, unsigned int);
int selectB(unsigned int);
int selectB_pr(unsigned int);


void filter_string();


// unsigned int select_B_pr_search(unsigned int);

#endif 