#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;


string BWT_PLAIN(string text) {
	// this is the basic implementation 
	// of the BWT(Burrows-Wheeler transform)
	// and the time complexity is O(n^2).
	vector<string> rotations;
	string bwt = "";

	// Generate all possible ratations of the input text
	for (int i = 0; i < text.length(); ++i) {
		string rotation = text.substr(i) + text.substr(0, i);
		rotations.push_back(rotation);
	}

	// sort the rotations lexicographically
	sort(rotations.begin(), rotations.end());

	// Build the BWT by taking the last character of each rotation.
	for (int i = 0; i < rotations.size(); ++i) {
		bwt += rotations[i][text.length() - 1];
	}

	return bwt;
}

string BWT_optimal(string text) {
	// this is the optimal implementation of the BWT
	// its corresponding time complexity is O(n)
	return "";
}


int main() {
	string input_text = "[3]Frank Manola[6]Sandra Heiler[9]A 'RISC' Object Model for Object System Interoperation: Concepts and Applications.[12]GTE Laboratories Incorporated[15]TR-0231-08-93-165[18]August[21]1993[26]Frank Manola[29]Mark F. Hornick[32]Alejandro P. Buchmann[35]Object Data Model Facilities for Multimedia Data Types.[38]GTE Laboratories Incorporated[41]TM-0332-11-90-165[44]December[47]1990[52]Farshad Nayeri[55]Benjamin Hurwitz[58]Experiments with Dispatching in a Distributed Object System.[61]GTE Laboratories Incorporated[64]TR-0236-09-93-165[67]July[70]1993";
    string bwt_text = BWT_PLAIN(input_text);

    cout << "Input text: " << input_text << endl;
    cout << "BWT of input text: " << bwt_text << endl;

    return 0;
}