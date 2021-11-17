#ifndef _ROTORPOSITIONS_H_
#define _ROTORPOSITIONS_H_

#include <fstream>
#include <string>
#include <vector>

class RotorPositions
{
public:
  int openRotorPositions(const std::string &loadFromFile, int enterNumRotors);
  int getRotorPosition(int rotorNum) const;

private:
  int ParseFile(std::ifstream &file, const std::string &fileName);
  bool hasEnoughRotorPos();

  std::vector<int> rotorPositionValues_;
  int numRotors;
};

#endif //_ROTORPOSITONS_H_
