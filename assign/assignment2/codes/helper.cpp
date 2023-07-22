#include "helper.h"



void output_S_sequence() {
	int tot = 0;
	for (auto each : S) {
		tot++; 
		cout << tot << ": " << each << endl;
	}
}

void output_B_sequence() {
	int fre = 0;
	for (auto each : B) {
		printf("%b ", each);
		fre++;
		if (fre == 2) {
			cout << endl;
			fre = 0;
		}
	}
	cout << endl;
}

void output_length_table_S() {
	int charc = 0;
	while (charc < CHARWIDTH) {
		if (Length_table[charc] > 0) {
			cout << char(charc) << ", " << Length_table[charc] << endl;
		}
		charc++;
	}
}

void output_B_pr_sequence() {
	int fre = 0;
	for (auto each : B_pr) {
		printf("%x ", each);
		fre++;
		if (fre == 2) {
			cout << endl;
			fre = 0;
		}
	}
	cout << endl;
}

void output_rankB() {
	for (auto each: rank_B) {
		cout << each << " ";
	}
	cout << endl;
}

void output_rankB_pr() {
	for (auto each: rank_B_pr) {
		cout << each << " ";
	}
	cout << endl;
}

void output_CS_table() {
	for (int i = 0; i < CHARWIDTH; i++) {
		printf("%c: %d\n", char(i), CS_table[i]);
	}
}