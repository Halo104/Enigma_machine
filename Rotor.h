#ifndef _ROTOR_H_
#define _ROTOR_H_

#include "errors.h"
#include <fstream>
#include <string>
#include <vector>

class RotorPositions;
class Rotor
{
public:
  Rotor() : notchResetToken(true), configLocation(200) {}
  int openRotorFile(const std::string &loadFromFile);
  char DecipherForward(const char input);
  char DecipherBackward(const char input);
  void setConfig(int startValue);
  int getConfig();
  void incrementConfigLocation();
  bool hasReachedLinkValue();

  struct CreatedRotors
  {
    int errorCode;
    std::vector<Rotor> rotors;
    CreatedRotors() : errorCode(NO_ERROR) {}
  };
  static CreatedRotors CreateRotors(const RotorPositions &rotorPositions, int numRotors, char **argv);

  bool notchResetToken;
  std::vector<int> linkValue_;

private:
  int ParseFile(std::ifstream &file, const std::string &fileName);
  bool containAtLeastXIndexs(int num);
  void calShiftValues();

  std::vector<int> rotorValues_;
  std::vector<int> rotorShiftValues_;
  int configLocation;
};

#endif //_ROTOR_H_
