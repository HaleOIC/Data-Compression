#include <cstdio>
#include <fstream>
#include <map>
#include <set>

using namespace std;

void lzwDecode(ifstream &input, ofstream &output);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("need 2 arguments, but %d given\n", argc - 1);
        return 0;
    }
    // open files
    ifstream input(argv[1], ios::binary);
    if (!input) {
        printf("cannot open file '%s'\n", argv[1]);
        return 0;
    }
    ofstream output(argv[2]);
    if (!output) {
        printf("cannot open file '%s'\n", argv[2]);
        return 0;
    }

    lzwDecode(input, output);
    return 0;
}

void lzwDecode(ifstream &input, ofstream &output) {
    set<string> allWords; // used to check if a word is in the dictionary
    map<size_t, string> dictionary; // the dictionary: index -> word
    string prevWord;
    string decodedWord;
    unsigned char currByte;

    if (!input.get((char &)currByte)) {
        return;
    }

    prevWord = (char)currByte;
    output << prevWord;

    while (input.get((char &)currByte)) {
        if (currByte >= 0x80) {
            // readed byte is a encoded word
            unsigned char lowByte;
            input.get((char &)lowByte);
            unsigned highByte = currByte & 0x7f; // remove the highest bit
            unsigned index = highByte << 8 | (unsigned)lowByte;

            if (dictionary.count(index)) {
                // in the dictionary
                decodedWord = dictionary[index];
            } else {
                // not in the dictionary
                decodedWord = prevWord + prevWord[0];
                dictionary[dictionary.size()] = decodedWord;
                allWords.insert(decodedWord);
            }
            // output the decoded word
            output << decodedWord;
            string newWord = prevWord + decodedWord[0];

            // check if the new word is in the dictionary
            if (allWords.count(newWord)) {
                // in the dictionary
                prevWord = newWord;
            } else {
                // not in the dictionary
                dictionary[dictionary.size()] = newWord;
                allWords.insert(newWord);
                prevWord = decodedWord;
            }
        } else {
            // readed byte is a char
            char currentChar = (char)currByte;
            output << currentChar;
            string key = prevWord + currentChar;
            if (allWords.count(key)) {
                // in the dictionary
                prevWord = key;
            } else {
                // not in the dictionary
                dictionary[dictionary.size()] = key;
                allWords.insert(key);
                prevWord = currByte;
            }
        }
    }
}
