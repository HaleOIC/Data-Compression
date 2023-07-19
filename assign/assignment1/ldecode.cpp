#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unordered_map>
#include <string>

using namespace std;

#define DICT_MAX_SIZE 32767
#define ENCODED_TEXT_FILE "./raw_text.lzw"

bool exists(const string &name)
{
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

string read_from_ifs(ifstream *ifs, unordered_map<int, string> *dict)
{
  // Check has next and move forward
  int c = ifs->get();
  if (c < 0)
    return "";
  // See if current byte indicates dictionary index
  if (c >> 7)
  {
    int first = c & 127;
    int second = ifs->get();
    int index = (first << 8) | second;
    return dict->at(index);
  }

  return string(1, c);
}

void dump_to_ofs(string text, ofstream *ofs)
{
  ofs->write(text.c_str(), sizeof(char) * text.size());
}

void lzw_decode(string src, string dst)
{
  unordered_map<int, string> dict = {};
  ifstream ifs(src, istream::binary);
  ofstream ofs(dst, ostream::binary);

  string p = "";
  string c = read_from_ifs(&ifs, &dict);
  int index = 1;
  while (!c.empty())
  {
    // First iteration
    if (p.empty())
    {
      p = c;
      ofs.put(c[0]);
      c = read_from_ifs(&ifs, &dict);
      continue;
    }

    // Output current
    dump_to_ofs(c, &ofs);
    // Populate dictionary
    dict.insert({{index++, p + c[0]}});
    // Preserve current and forward
    p = c;
    c = read_from_ifs(&ifs, &dict);
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
    lzw_decode(argv[1], argv[2]);
  else
    cout << "Src file " << argv[1]
         << " not found" << endl;
  return 0;
}
