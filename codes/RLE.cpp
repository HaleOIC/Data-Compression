#include <vector>
#include <map>
#include <bitset>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

string runLengthEncode(string input) {
	string output = "";
	int count = 1;

	// iterate over each character in the input string
	for (int i = 1; i <= input.length(); ++i) {
		// If the current character is the same as the previous one, increment the count
        if (input[i] == input[i-1]) {
            count++;
        }
        // If the current character is different from the previous one
        // output the count and character
        else {
        	if (count == 1) {
        		output += input[i - 1];
        	} else if (count == 2) {
        		output += input[i-1];
        		output += input[i-1];
        	} else {
        		count -= 3;
        		output += input[i-1];
        		int flag = 1;
        		// Divide the count into 7-bit blocks 
        		// and encode them as a sequence of bytes
        		while (count || flag) {
        			output += char((count & 127) | 128);
        			count >>= 7;
        			flag = 0;
        		}
        	}
        	count = 1;
        }
	}

	return output;
}



int main(int argv, char **argc) {
	if (argv != 2) {
		cout << "Please make sure you input the file you wanna encode!!!" << endl;
		return -1;
	}

	// convert the name of file from char* to string 
	string filename = string(argc[1]);
	

	// read the content of the file and store them into a single string variable.
	ifstream infile(filename);
	string content((istreambuf_iterator<char>(infile)), (istreambuf_iterator<char>()));

	string encoded = runLengthEncode(content);

	ofstream outfile("output.txt");
	outfile << encoded;
	outfile.close();

	return 0;
}