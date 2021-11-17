#include "errors.h"
#include "RotorPositions.h"
#include "utils.h"

#include <algorithm>
#include <iostream>
#include <vector>

int RotorPositions::openRotorPositions(const std::string &loadFromFile, int enterNumRotors)
{
  //Open the rotor position file and checks that it can be opened properly
  numRotors = enterNumRotors;
  std::ifstream file;

  file.open(loadFromFile);
  if (file.fail())
  {
    std::cerr << "ERROR: RotorPositions file couldn't be open" << std::endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  int ParseFileReturnValue = ParseFile(file, loadFromFile);
  file.close();

  return ParseFileReturnValue;
}

int RotorPositions::ParseFile(std::ifstream &file, const std::string &fileName)
{
  //Checks that the values in the rotor position file is valid, and there is enough positions file
  //for the number of rotors entered in the command line
  std::string input;
  int n = 0;

  while (file >> input)
  {
    //Checks that the next RotorPosition value is OK
    if (!isNumeric(input))
    {
      std::cerr << "Non-numeric character in rotor positions file " << fileName << std::endl;
      return NON_NUMERIC_CHARACTER;
    }

    int inputValue = std::stoi(input);
    if (!isBetween0to25(inputValue))
    {
      std::cerr << "ERROR: RotorPosition file " << fileName << " contains values not between 0 and 25" << std::endl;
      return INVALID_INDEX;
    }

    //Stores the rotor positions that are on the fine, into a vector
    rotorPositionValues_.push_back(inputValue);
    n++;
  }

  std::reverse(rotorPositionValues_.begin(), rotorPositionValues_.end());

  if (!hasEnoughRotorPos())
  {
    std::cerr << "No starting position for rotor " << rotorPositionValues_.size() << " in rotor position file: " << fileName << std::endl;
    return NO_ROTOR_STARTING_POSITION;
  }
  return NO_ERROR;
}

bool RotorPositions::hasEnoughRotorPos()
{
  int length = rotorPositionValues_.size();
  if (numRotors <= length)
  {
    return true;
  }
  else
  {
    return false;
  }
  return false;
}

int RotorPositions::getRotorPosition(int rotorNum) const
{
  return rotorPositionValues_.at(rotorNum);
}
