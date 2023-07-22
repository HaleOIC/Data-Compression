/**
 * in order to have a trade off between time and space.
 * I choose the split block as a way to save the memory.
 * the original source file can not be bigger than 160MB
 * in case we will use more than specific limits of runtime memory(13MB)
 * we choose the dynamic data structure like vector or even use new for 
 * acquiring the memory for use.
 */

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "./process.h"
#include "./helper.h"
#include "./search.h"

using namespace std;

/**
 *  Some general constants:
 * 
 *  	CHARWIDTH: the width of the char.
 * 		BLOCKSIZE: the default size of each block.
 * 		CHAR_BLOCK_SIZE: the block size for char(S)
 * 		BIT_BLOCK_SIZE_OF_UINT: the block size for bit(B, B_pr)
 */
const int CHARWIDTH = 128;
const int BLOCKSIZE = 2048;

unsigned int BLOCK_SIZE = BLOCKSIZE;
unsigned int CHAR_BLOCK_SIZE = BLOCKSIZE;
unsigned int BIT_BLOCK_SIZE_OF_UINT = BLOCKSIZE / 32;

// used for decode the RLB file to S and B file 
// and then generate the B_pr file.
vector<char> S;
vector<unsigned int> B, B_pr;


/**
 * some built-in variables for S
 * 		
 * 		CS_table: the accumulative tables for S 
 * 		Length_table: the length table for S
 * 		select_S: the total vector for S
 * 		S_block_count: make a total count of all blocks.
 */
unsigned int CS_table[CHARWIDTH];
unsigned int Length_table[CHARWIDTH];
vector<unsigned int> select_S;
unsigned int S_block_count = 0;

/**
 * some built-in variables for B
 * 
 * 		rank_B: for rankB and selectB function
 * 		rank_B_block_count: total block for B file
 * 		count_B_1: total number of ones in B
 * 		count_B: total number of B (bits level)
 */
vector<unsigned int> rank_B;
unsigned int rank_B_block_count = 0;
unsigned int count_B_1 = 0;
unsigned int count_B = 0;

/**
 * some built-in variables for B_pr
 * 		rank_B_pr: the same effect as rank_B
 * 		rank_B_pr_block_count: the same effect as rank_B_block_count
 * 		count_B_pr_1: the same effect as count_B_1
 */
vector<unsigned int> rank_B_pr;
unsigned int rank_B_pr_block_count = 0;
unsigned int count_B_pr_1 = 0;

// used for output the matching string.
extern map<int, string> filt_map;


int main(int argv, char** argc) {
	// only consider the input command
	if (argv < 4) {
		cout << "Please make sure you have input in the correct form!!!" << endl;
		return -1;
	}

	string file_name = argc[1];
	string idx_name = argc[2];
	string query_str = argc[3];

	// substring the query string without ""
	if (query_str.front() == '"' && query_str.back() == '"') {
		query_str = query_str.substr(1, query_str.length() - 2);
	}

	// generate the block one by one
	preprocess(file_name);

	// choose which case the program are facing
	if (B.size() > 102400) {
		BLOCK_SIZE = BLOCK_SIZE * 8;
		CHAR_BLOCK_SIZE = 20480;
		BIT_BLOCK_SIZE_OF_UINT *= 8;

	}

	// generate the corresponding B_pr sequence
	generete_related_sequence();
	generate_B_pr();
	

	// make a query by calling related function
	search_str(query_str);

	// print out the matching string one by one.
	for (auto each : filt_map) {
		cout << "[" + to_string(each.first) << "]" << each.second << endl;
	}

	// generate the idx file(no writing anything yet)
	ofstream idx(idx_name, std::ios::out);
	idx.close();

	return 0;
}