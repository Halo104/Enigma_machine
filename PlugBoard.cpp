#include "errors.h"
#include "PlugBoard.h"
#include "utils.h"

#include <iostream>

int PlugBoard::openPlugBoardFile(const std::string &loadFromFile)
{
  //Opens the file and extract the values inside
  std::ifstream file;

  file.open(loadFromFile);
  if (file.fail())
  {
    std::cerr << "ERROR: Plugboard file couldn't be open" << std::endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  int ParseFileReturnValue = ParseFile(file, loadFromFile);
  file.close();

  return ParseFileReturnValue;
}

int PlugBoard::ParseFile(std::ifstream &file, const std::string &fileName)
{
  //Conducts the required checks to the file values and then stores the values into a vector
  std::string input;
  int n = 0;

  while (file >> input)
  {
    //Checks that the next plugboard value is OK
    if (!isNumeric(input))
    {
      std::cerr << "Non-numeric character in plugboard file " << fileName << std::endl;
      return NON_NUMERIC_CHARACTER;
    }
    int inputValue = std::stoi(input);
    if (!isBetween0to25(inputValue))
    {
      std::cerr << "ERROR: Plugboard file isn't between 0 and 25" << std::endl;
      return INVALID_INDEX;
    }
    if (alreadyHasValue(inputValue, plugBoardValues_))
    {
      std::cerr << "ERROR: Plugboard file " << fileName << " contains repeating values" << std::endl;
      return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
    }

    plugBoardValues_.push_back(inputValue);
    n++;
  }

  if (!hasEvenNumIndex(plugBoardValues_))
  {
    std::cerr << "Incorrect number of parameters in plugboard file " << fileName << std::endl;
    return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
  }
  if (n > 26)
  {
    std::cerr << "Incorrect number of parameters in plugboard file " << fileName << std::endl;
    return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
  }

  return NO_ERROR;
}

char PlugBoard::Decipher(const char &input)
{
  //Using the extracted values for the plugboard file, the input character is swapped to the required output character
  int plugBoardLength = plugBoardValues_.size();

  if (plugBoardValues_.empty())
  {
    return input;
  }

  int inputCharIndex = convertAlphaToIndex(input);
  int outputCharIndex = 200;
  bool match = false;

  for (int j = 0; j < plugBoardLength; ++j)
  {
    if (plugBoardValues_[j] == inputCharIndex)
    {
      match = true;
      if (j % 2 == 0)
      {
        outputCharIndex = plugBoardValues_[j + 1];
      }
      else
      {
        outputCharIndex = plugBoardValues_[j - 1];
      }
    }
    else if (match == false)
    {
      outputCharIndex = inputCharIndex;
    }
  }
  char plugBoardOutput;
  plugBoardOutput = convertIndexToAlpha(outputCharIndex);

  return plugBoardOutput;
}
