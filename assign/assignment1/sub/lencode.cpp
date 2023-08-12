#include <cstdio>
#include <fstream>
#include <map>

using namespace std;

void lzwEncode(ifstream &input, ofstream &output);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("need 2 arguments, but %d given\n", argc - 1);
        return 0;
    }
    // open files
    ifstream input(argv[1]);
    if (!input) {
        printf("cannot open file '%s'\n", argv[1]);
        return 0;
    }
    ofstream output(argv[2], ios::binary);
    if (!output) {
        printf("cannot open file '%s'\n", argv[2]);
        return 0;
    }

    lzwEncode(input, output);
    return 0;
}

void lzwEncode(ifstream &input, ofstream &output) {
    map<string, size_t> dictionary; // word -> index
    string prevWord;
    char currChar;

    while (input.get((char &)currChar)) {
        string prevPlusCurr = prevWord + currChar;
        if (dictionary.count(prevPlusCurr)) {
            // in the dictionary
            prevWord = prevPlusCurr;
        } else {
            // not in the dictionary
            if (prevWord.length() < 3) {
                // if the word is too short, there is no need to encode it,
                // as the encoded words' length is 2 bytes
                output << prevWord;
            } else {
                // if the word is long enough, encode it and output
                unsigned number = dictionary[prevWord];
                unsigned char low = number & 0xff;
                number >>= 8;
                unsigned char high =
                    0x80 | (unsigned char)number; // set the highest bit (flag)
                // write the encoded word to the output file
                output.put(high);
                output.put(low);
            }
            // add the new word to the dictionary,
            // if and only if the dictionary is not full, the word is not empty
            if (prevWord.length() && dictionary.size() < 32768) {
                dictionary[prevPlusCurr] = dictionary.size();
            }
            prevWord = currChar;
        }
    }

    // the last word
    if (prevWord.length() < 3) {
        output << prevWord;
    } else {
        unsigned number = dictionary[prevWord];
        unsigned char low = number & 0xff;
        number >>= 8;
        unsigned char high = 0x80 | (unsigned char)(number);
        output.put(high);
        output.put(low);
    }
}
