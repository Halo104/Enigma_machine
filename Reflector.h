#ifndef _REFLECTOR_H_
#define _REFLECTOR_H_

#include <fstream>
#include <string>
#include <vector>

class Reflector
{
public:
  int openReflectorFile(const std::string &loadFromFile);
  char Decipher(const char input);

private:
  int ParseFile(std::ifstream &file, const std::string &fileName);
  bool contain13Pairs();

  std::vector<int> reflectorValues_;
};

#endif //_REFLECTOR_H_
