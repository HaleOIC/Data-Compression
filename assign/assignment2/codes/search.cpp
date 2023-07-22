#include "search.h"

// ***** we assume all the array starts position is one. *****

map<int, string> filt_map;

/**
 * search the target string by using the backward search
 * for confirming the concrete range
 * then generate the string one by one
 * filter the duplicated string by using the map.
 * 
 * parameter:
 * 		tar: the demanding string
 * 
 * return:
 * 		None;
 */
void search_str(const string& tar) {
	auto rv = backward_search(tar);
	int range_left = rv.first, range_right = rv.second;

	// no matching string in the B and S file
	if (range_right - range_left + 1 <= 0) {
		return;
	}

	// for each outcome, search each string one by one
	// and finally store them into the map for filter
	for (int start = range_left; start <= range_right; ++start) {
		int tmp = find_index_record(start);
		if (filt_map.find(tmp) == filt_map.end()) {
			// which means it is a new record
			filt_map[tmp] = match_new_str(tmp);
		}
	}
}

/**
 * find the index(offset) of record by giving the start pos
 * 
 * parameter:
 * 		pos: the start position to find
 * 
 * return:
 * 		the index of record.
 */
int find_index_record(int pos) {
	string rv = "";
	int cnt = 0;
	while (get_Li(pos) != '[') {
		rv +=  get_Li(pos);
		pos = reverse_BWT(pos);
		cnt++;
	}
	// find '['
	rv += get_Li(pos);
	cnt++;
	string str = "";
	for (int i = cnt ; i >= 0; i--) {
		str +=  rv[i];
	}

	// find the corresponding index of the record.
	int pos_idx = 1, val = 0;
	for (int i = 0; i < int(str.size()); i++) {
		if (str[pos_idx] == ']') {
			break;
		}
		if (str[pos_idx] >= '0' && str[pos_idx] <= '9') {
			val = val * 10 + str[pos_idx] - '0';
		}
		pos_idx++;
	}
	return val;
}

/**
 * return the match new string by using the index
 * 
 * parameter:
 * 		idx: the demanding index
 * 
 * return:
 * 		string: the corresponding string of index
 */
string match_new_str(int idx) {
	int tar_num = idx + 1, pos = 0;
	while (true) {
		string tar_str = "[" + to_string(tar_num) + "]";	
		auto outcome = backward_search(tar_str);
		int res_left = outcome.first, res_right = outcome.second;
		// not find the target string
		if (res_right - res_left + 1 <= 0) {
			tar_num++;
			if (tar_num > idx + int(CHAR_BLOCK_SIZE)) {
				tar_num = 1;
			}
			if (tar_num == idx) {
				break;
			}
			continue;
		}
		pos = res_left;
		break;
	}

	// using reverse BWT to find the content
	string rv = "";
	while (get_Li(pos) != ']') {
		rv += get_Li(pos);
		pos = reverse_BWT(pos);
	}
	reverse(rv.begin(), rv.end());
	return rv;
}



/**
 * reverse the BWT or in order to calculate LF[col]
 * 
 * parameter:
 * 		col: the corresponding value of formula
 * 
 * return:
 * 		int: the value of LF[col]
 */
int reverse_BWT(unsigned int col) {
	char cur_char = get_Li(col);
	return selectB_pr(CS_table[int(cur_char)] + rankS(cur_char, rankB(col)))
			+ col - selectB(rankB(col));
}
/**
 * 
 * the main or even core of the algorithm:
 * 1. confirm the initial fst and lst
 * 2. calculate the corresponding value of fst and lst because of the 
 * 	  relation between Li[i] and cur_char
 * 3. return back the lst and fst 
 * 
 * parameter:
 * 		str: the searched string
 * 
 * return:
 * 		pair<int, int>: the first is fst and the second is lst.
 */
pair<int, int> backward_search(const string& str) {
	// init the search condition
	int pos = str.size() - 1;
	char cur_char = str[pos];
	int fst = selectB_pr(CS_table[int(cur_char)] + 1);
	int lst = selectB_pr(CS_table[int(cur_char)] + Length_table[int(cur_char)] + 1) - 1;

	// enter the iterative loop to find the final position
	while ((fst <= lst) && pos >= 1) {
		cur_char = str[pos - 1];

		// using the conditional transfer
		if (get_Li(fst - 1) != cur_char) {
			fst = selectB_pr(CS_table[int(cur_char)] + 1 + rankS(cur_char, rankB(fst - 1))) - 1 + 1;
		} else {
			fst = selectB_pr(CS_table[int(cur_char)] + rankS(cur_char, rankB(fst - 1))) 
					+ fst - 1 - selectB(rankB(fst - 1)) + 1;
		}


		if (get_Li(lst) != cur_char) {
			lst = selectB_pr(CS_table[int(cur_char)] + 1 + rankS(cur_char, rankB(lst))) - 1;
		} else {
			lst = selectB_pr(CS_table[int(cur_char)] + rankS(cur_char, rankB(lst))) 
					+ lst - selectB(rankB(lst));
		}
		pos--;
	}

	// return back the value of fst and lst
	return make_pair(fst, lst);
}


/**
 * return rank_c(S, num)
 * we may use select_S for speed up our calculating for rankS
 * 
 * paramter:
 * 		charac:	the demanding charac
 * 		num: corresponding to num in formula
 * 
 * return:
 * 		unsinged int: a corresponding value
 */
int rankS(char charc, unsigned int num) {
	// easy for calculating 
	int pos = num / CHAR_BLOCK_SIZE;

	// get the basic positionn of rank S
	int rv = select_S[pos * CHARWIDTH + int(charc)];
	int offset = num % CHAR_BLOCK_SIZE;

	// calculate the rest parts
	for (int i = 0; i < offset; i++) {
		if (S[pos * CHAR_BLOCK_SIZE + i] == charc) {
			rv++;
		}
	}

	return rv;
}

/**
 * return select_c(S, num) 
 * by using the binary search and select_S for speeding up
 * 
 * parameter:
 * 		charac: the demanding character
 * 		num: the corresponding num in formula
 * 
 * return:
 * 		unsigned int: a corresponding value
 */
int selectS(char charc, unsigned int num) {
	// the demanding num is not accessible
	if (num == 0 || num > Length_table[int(charc)]) {
		return -1;
	}

	// using the binary search to find the basic 
	int low = 0, high = int(S_block_count);
	while (low <= high) {
		int mid = (low + high) >> 1;
		if (select_S[mid * CHARWIDTH + int(charc)] >= num) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}

	// searching from the basic position
	unsigned int rv = select_S[high * CHARWIDTH + int(charc)];
	for (int i = 0; i < int(CHAR_BLOCK_SIZE); i++) {
		if (S[high * CHAR_BLOCK_SIZE + i] == charc) {
			rv++;
			if (rv == num) {
				return high * CHAR_BLOCK_SIZE + i + 1;
			}
		}
	}
	return -1;
}


/**
 * return rank_1(B, num)
 * we may use the rank_B for speed up our searching.
 * 
 * parameter:
 * 		nun : corresponding to num in formula
 * 
 * return:
 *		unsigned int: a corresponding value
 */
int rankB(unsigned int num) {
	// easy for calculating 
	int pos = num / (32 * BIT_BLOCK_SIZE_OF_UINT);

	// get the basic position of rank B 
	int rv = rank_B[pos];
	int offset = num % (32 * BIT_BLOCK_SIZE_OF_UINT);

	// By using the offset to find the rv one by one.
	for (int i = 0; i < offset / 32; i++) {
		for (int j = 0; j < 32; j++) {
			if (B[pos * BIT_BLOCK_SIZE_OF_UINT + i] & (1 << (31 - j))) {
				rv++;
			}
		}
	}

	// consider the rest part
	for (int i = 0; i < offset % 32; i++) {
		if (B[pos * BIT_BLOCK_SIZE_OF_UINT + offset / 32] & (1 << (31 - i))) {
			rv++;
		}
	}
	return rv;
}

/**
 * return select_1(B, num) 
 * the same way we will use binary search and rank_B vector
 * to speed up the race.
 * 
 * parameter:
 * 		num: the corresponding value to num in formula
 * 
 * return:
 * 		unsinged int: a corresponding value
 */
int selectB(unsigned int num) {
	// out of range
	if (num == 0) {
		return 0;
	}
	if (num > count_B_1) {
		return count_B + 1;
	}

	// binary search for the position
	int low = 0, high = int(rank_B_block_count);
	while (low <= high) {
		int mid = (low + high) >> 1;
		if (rank_B[mid] >= num) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}

	//search from the basic position
	unsigned int rv = rank_B[high];
	for (int i = 0; i < int(BIT_BLOCK_SIZE_OF_UINT); i++) {
		for (int j = 0; j < 32; j++) {
			if (B[high * BIT_BLOCK_SIZE_OF_UINT + i]
				& (1 << (31 - j))) {
				rv++;
				if (rv == num) {
					return (high * BIT_BLOCK_SIZE_OF_UINT + i) * 32 + j + 1;
				}
			}
		}
	}

	// default case
	return -1;
} 


/**
 * return select_1(B_pr, num) 
 * the same way we will use binary search and rank_B_pr vector
 * to speed up the race.
 * 
 * parameter:
 * 		num: the corresponding value to num in formula
 * 
 * return:
 * 		unsinged int: a corresponding value
 */
int selectB_pr(unsigned int num) {
	// out of range
	if (num == 0) {
		return 0;
	}
	if (num > count_B_pr_1) {
		return count_B + 1;
	}

	// binary search for the position
	int low = 0, high = rank_B_pr_block_count;
	while (low <= high) {
		int mid = (low + high) >> 1;
		if (rank_B_pr[mid] >= num) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}

	//search from the basic position
	unsigned int rv = rank_B_pr[high];
	for (int i = 0; i < int(BIT_BLOCK_SIZE_OF_UINT); i++) {
		for (int j = 0; j < 32; j++) {
			if (B_pr[high * BIT_BLOCK_SIZE_OF_UINT + i]
				& (1 << (31 - j))) {
				rv++;
				if (rv == num) {
					return (high * BIT_BLOCK_SIZE_OF_UINT + i) * 32 + j + 1;
				}
			}
		}
	}

	// default case
	return -1;
} 


/**
 * get L[i] by using the B and S
 *  the corresponding formula is
 * 			 L[i] = S[ rankB(i) - 1 ]
 * 
 * parameter:
 * 		col: the column of the ith
 * 
 * return:
 * 		char: the corresponding L[i]
 */
char get_Li(unsigned int col) {
	return S[rankB(col) - 1];
}