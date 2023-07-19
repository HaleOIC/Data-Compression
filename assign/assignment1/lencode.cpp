#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unordered_map>
#include <string>

using namespace std;

#define DICT_MAX_SIZE 32767

bool exists(const string &name)
{
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

void dump_to_ofs(string text, ofstream *ofs)
{
  ofs->write(text.c_str(), sizeof(char) * text.size());
}

void dump_to_ofs(int index, ofstream *ofs)
{
  int first = 128 | (index >> 8);
  int second = index & 255;
  ofs->put(first);
  ofs->put(second);
}

void dump_to_ofs(string text, ofstream *ofs, unordered_map<string, int> *dict)
{
  // Output text
  if (text.size() == 1)
    ofs->put(text[0]);
  // Output the index of text
  else
    dump_to_ofs(dict->at(text), ofs);
}

void lzw_encode(string src, string dst)
{
  unordered_map<string, int> dict = {};
  ifstream ifs(src, istream::binary);
  ofstream ofs(dst, ostream::binary);

  string p = "";
  char c = ifs.get();
  int index = 1;

  while (true)
  {
    // Skip first step
    if (p.empty())
    {
      if (c < 0)
        break;
      p = string(1, c);
      c = ifs.get();
      continue;
    }

    string symbol = p + c;
    auto got = dict.find(symbol);
    // If not symbol not found in dictionary
    if (got == dict.end())
    {
      dict.insert({{symbol, index++}});
      dump_to_ofs(p, &ofs, &dict);
      p = string(1, c);
    }
    // if found, append current character to previous
    else
      p += c;
    // Cursor Forward
    if (c == EOF)
      break;
    c = ifs.get();
  }

  ifs.close();
  ofs.close();
}

int main(int argc, char const *argv[])
{
  if (argc != 3)
    cout << "Program takes exact 2 args, but "
         << argc << " given" << endl;
  else if (exists(argv[1]))
    lzw_encode(argv[1], argv[2]);
  else
    cout << "Src file " << argv[1]
         << " not found" << endl;
  return 0;
}
