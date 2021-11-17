#include "errors.h"
#include "Rotor.h"
#include "RotorPositions.h"
#include "utils.h"

#include <iostream>

int Rotor::openRotorFile(const std::string &loadFromFile)
{
  //Opens the rotor file, checks it opens properly and stores the values into a vector
  std::ifstream file;

  file.open(loadFromFile);
  if (file.fail())
  {
    std::cerr << "ERROR: Rotor file couldn't be open" << std::endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  int ParseFileReturnValue = ParseFile(file, loadFromFile);
  file.close();

  return ParseFileReturnValue;
}

int Rotor::ParseFile(std::ifstream &file, const std::string &fileName)
{
  //Checks that the values within the rotor file is valid and stores it in a vector
  std::string input;

  while (file >> input)
  {
    //Checks that the next reflector value is OK
    if (!isNumeric(input))
    {
      std::cerr << "Non-numeric character for mapping in rotor file " << fileName << std::endl;
      return NON_NUMERIC_CHARACTER;
    }

    int inputValue = std::stoi(input);
    if (!isBetween0to25(inputValue))
    {
      std::cerr << "ERROR: Rotor file " << fileName << " contain numbers not between 0 and 25" << std::endl;
      return INVALID_INDEX;
    }

    if (alreadyHasValue(inputValue, rotorValues_))
    {
      if (rotorValues_.size() < 26)
      {
        int existLocation = repeatValueLoc(inputValue, rotorValues_);
        std::cerr << "Invalid mapping of input " << rotorValues_.size() << " to output " << inputValue << " (output ";
        std::cerr << inputValue << " is already mapped to from input " << existLocation << ") in rotor file " << fileName << std::endl;
        return INVALID_ROTOR_MAPPING;
      }
    }
    rotorValues_.push_back(inputValue);
  }
  
  const int numValues = rotorValues_.size();
  if (!containAtLeastXIndexs(26))
  {
    std::cerr << "Not all inputs mapped in rotor file: " << fileName << std::endl;
    return INVALID_ROTOR_MAPPING;
  }

  if (!containAtLeastXIndexs(27))
  {
    std::cerr << "No notch values in rotor file " << fileName << std::endl;
    return INVALID_ROTOR_MAPPING;
  }

  if (numValues > 52)
  {
    std::cerr << "Too many notch values in rotor file " << fileName << std::endl;
    return INVALID_ROTOR_MAPPING;
  }

  //Assigning the linkage/notch Value
  for (int i = 0; i < numValues - 26; ++i)
  {
    linkValue_.push_back(rotorValues_[26 + i]);
  }

  //Calculate Caesar Shift
  calShiftValues();

  return NO_ERROR;
}

void Rotor::calShiftValues()
{
  for (int i = 0; i < 26; ++i)
  {
    rotorShiftValues_.push_back(rotorValues_.at(i) - i);
  }
}

bool Rotor::containAtLeastXIndexs(int num)
{
  int length = rotorValues_.size();
  if (length < num)
  {
    return false;
  }
  else
  {
    return true;
  }
}

char Rotor::DecipherForward(const char input)
{
  //Calculates using the input character, the required output character from each rotor
  int inputStringIndex = convertAlphaToIndex(input);
  int currentRotorIndex = configLocation + inputStringIndex;
  if (currentRotorIndex > 25)
  {
    currentRotorIndex = currentRotorIndex - 26;
  }

  int outputStringIndex = inputStringIndex + rotorShiftValues_[currentRotorIndex];
  if (outputStringIndex > 25)
  {
    outputStringIndex = outputStringIndex - 26;
  }
  else if (outputStringIndex < 0)
  {
    outputStringIndex = outputStringIndex + 26;
  }

  int rotorOutput = convertIndexToAlpha(outputStringIndex);

  return rotorOutput;
}

void Rotor::setConfig(int startValue)
{
  configLocation = startValue;
}

int Rotor::getConfig()
{
  return configLocation;
}

char Rotor::DecipherBackward(const char input)
{
  //Calculates using the input character, the required output character as it goes back through each rotor
  int valuesInRotor = 26;
  bool indexFound = false;
  int inputStringIndex = convertAlphaToIndex(input);
  int currentRotorIndex = 0;
  int inputStringIndexWithConfig = inputStringIndex + configLocation;

  if (inputStringIndexWithConfig > 25)
  {
    inputStringIndexWithConfig = inputStringIndexWithConfig - 26;
  }
  else if (inputStringIndexWithConfig < 0)
  {
    inputStringIndexWithConfig = inputStringIndexWithConfig + 26;
  }

  for (int j = 0; j < valuesInRotor; ++j)
  {
    if (rotorValues_[j] == inputStringIndexWithConfig && !indexFound)
    {
      currentRotorIndex = j;
      indexFound = true;
    }
  }

  int outputStringIndex = currentRotorIndex - configLocation;
  if (outputStringIndex > 25)
  {
    outputStringIndex = outputStringIndex - 26;
  }
  else if (outputStringIndex < 0)
  {
    outputStringIndex = outputStringIndex + 26;
  }

  char rotorOutput = convertIndexToAlpha(outputStringIndex);

  return rotorOutput;
}

void Rotor::incrementConfigLocation()
{
  //When a rotor to it's right has reached the notch value, the rotor should increment the configlocation (i.e. rotate)
  ++configLocation;
  if (configLocation > 25)
  {
    configLocation = configLocation % 25 - 1;
  }
  else if (configLocation < 0)
  {
    configLocation = configLocation + 26;
  }
}

bool Rotor::hasReachedLinkValue()
{
  int length = linkValue_.size();
  for (int i = 0; i < length; ++i)
  {
    if (configLocation == linkValue_[i])
    {
      return true;
    }
  }
  return false;
}

Rotor::CreatedRotors Rotor::CreateRotors(const RotorPositions &rotorPositions, int numRotors, char **argv)
{
  CreatedRotors createdRotors{}; //Creating the required number of rotor objects and opening extracting data from the rotor file(s)
  if (numRotors == 0)
  {
    return createdRotors;
  }

  const int firstRotorFileArg = numRotors + 2;
  for (int i = 0; i < numRotors; ++i)
  {
    createdRotors.rotors.emplace_back(Rotor());
    auto &rotor = createdRotors.rotors.back();

    createdRotors.errorCode = rotor.openRotorFile(argv[firstRotorFileArg - i]);
    if (createdRotors.errorCode != NO_ERROR)
    {
      createdRotors.rotors.clear();
      return createdRotors;
    }
    rotor.setConfig(rotorPositions.getRotorPosition(i));
  }
  return createdRotors;
}
