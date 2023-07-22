#include "process.h"


map<char, vector<int>> map_B_pr;

/**
 * preprocess the datas in the string char by char
 * we wanna just generete B and S file in the program
 * hope it will not beyond the space limits =.=
 *
 * parameter:
 * 		infile: the name of the input file
 * 
 * return:
 * 		nothing
 */
void preprocess(const string& filename) {
	// prepare two char in case the double char occassion
	char cur, lst = '\0';
	ifstream infile(filename);

	// used for calculating the times of character.
	unsigned int tot = 0;
	int times = -1, val_pos = 31;
	unsigned int value_add = 0;

	while (infile.get(cur)) {
		int tmp_value = 0;
		char tmp_char;
		if (cur & (1 << 7)) {
			// the c is the number of character
			if (times < 0) {
				// set the flag in case the zero occassion
				times = 0;
				tot = 0;
			}
			tot = tot + ((cur & 127) << (times * 7));
			times++;
		} else {
			// the c is just the character
			if (lst == '\0') {
				lst = cur;
				continue;
			} else if (lst == cur) {
			// meet two consecutive identical
				tmp_char = lst;
				tmp_value = 2;
				lst = '\0'; 
				cur = '\0';
				tmp_value = 2;
			} else {
				// add the last character into the block
				tmp_char = lst;
				tmp_value = (times >= 0) ? tot + 3 : 1;
				times = -1;
				lst = cur;
			}
			// add the char and its corresponding frequency to 
			// B and S vector
			S.push_back(tmp_char);
			map_B_pr[tmp_char].push_back(tmp_value);
			value_add = value_add | (1 << val_pos);
			if (val_pos - tmp_value >= 0) {
				val_pos = val_pos - tmp_value;
			} else {
				B.push_back(value_add);
				for (int i = 0; i < (int) (tmp_value - val_pos - 1) / 32; ++i) {
					B.push_back(0);
				}
				val_pos = 31 - ((tmp_value - val_pos - 1) % 32);
				value_add = 0;
			}
		}
	}

	// solve the special case
	if (lst != '\0') {
		int tmp_value = (times >= 0) ? tot + 3 : 1;
		S.push_back(lst);
		map_B_pr[lst].push_back(tmp_value);
		value_add = value_add | (1 << val_pos);
		B.push_back(value_add);
		if (val_pos - tmp_value < 0) {
			for (int i = 0; i < (int) (tmp_value - val_pos - 1) / 32; ++i) {
				B.push_back(0);
			}
			val_pos = 31 - ((tmp_value - val_pos - 1) % 32);
			value_add = 0;
			if (val_pos != 31) {
				B.push_back(value_add);
			}
		}
	} else {
		B.push_back(value_add);
	}
	infile.close();

}


/**
 * init the related variable and then 
 * generate the related sequence especially for the rank and select
 * and then using the corresponding thing to generate the 
 * B_pr sequence.
 *
 * parameter:
 * 		None
 * 
 * return:
 * 		None
 */
void generete_related_sequence() {
	// init the select and rank variables in the file.
	for (int i = 0; i < CHARWIDTH; i++) {
		select_S.push_back(0);
		CS_table[i] = 0;
		Length_table[i] = 0;
	}
	rank_B_pr.push_back(0);
	rank_B.push_back(0);

	// generate rank_b for selectB and rankB
	// and b_count means the count of 1s in B
	unsigned int B_count = 0, cnt = 0;
	for (auto each : B) {
		for (int pos = 0; pos < 32; pos++) {
			if (each & (1 << (31 - pos))) {
				B_count++;
			}
		}
		if ((cnt + 1) % BIT_BLOCK_SIZE_OF_UINT == 0) {
			rank_B.push_back(B_count);
			rank_B_block_count++;
		}
		cnt++;
	}
	count_B_1 = B_count;

	// generate rank_s and select_s 
	cnt = 0;
	for (auto each : S) {
		Length_table[(int)each]++;
		if ((cnt + 1) % CHAR_BLOCK_SIZE == 0) {
			for (int j = 0; j < CHARWIDTH; ++j) {
				select_S.push_back(Length_table[j]);
			}
			S_block_count++;
		}
		cnt++;
	}

	// generate the CS_table
	cnt = 0;
	for (int i = 0; i < CHARWIDTH; ++i) {
		CS_table[i] = cnt;
		cnt += Length_table[i];
	}
}

/**
 * generate the B_pr sequence and 
 * generate the B_pr corresponding rank_b
 * 
 * parameter:
 * 		None
 * 
 * return:
 * 		None
 */

void generate_B_pr() {
	// generate the corresponding B_pr
	int val_pos = 31, val_add = 0;
	for (auto each : map_B_pr) {
		for (auto val : each.second) {
			count_B += val;
			val_add = val_add | (1 << val_pos);
			if (val_pos - val >= 0) {
				val_pos = val_pos - val;
			} else {
				B_pr.push_back(val_add);
				for (int i = 0; i < (int) (val - val_pos - 1) / 32; ++i) {
					B_pr.push_back(0);
				}
				val_pos = 31 - ((val - val_pos - 1) % 32);
				val_add = 0;
			}
		}
	}
	if (val_pos != 31) {
		B_pr.push_back(val_add);
	}

	// generate the B_pr corresponding select_B_pr
	unsigned int cnt = 0, B_count = 0;
	for (int i = 0; i < int(B_pr.size()); i++) {
		for (int j = 0; j < 32; j++) {
			if (B_pr[i] & (1 << (31 - j))) {
				B_count++;
			}
		}
		if ((cnt + 1) % BIT_BLOCK_SIZE_OF_UINT == 0) {
			rank_B_pr.push_back(B_count);
			rank_B_pr_block_count++;
		}
		cnt++;
	}
	count_B_pr_1 = B_count;
	
}