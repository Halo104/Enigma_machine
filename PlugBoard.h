#ifndef _PLUGBOARD_H_
#define _PLUGBOARD_H_

#include <fstream>
#include <string>
#include <vector>

class PlugBoard
{
public:
  char Decipher(const char &input);
  int openPlugBoardFile(const std::string &loadFromFile);

private:
  int ParseFile(std::ifstream &file, const std::string &fileName);

  std::vector<int> plugBoardValues_;
};

#endif //_PLUGBOARD_H_
