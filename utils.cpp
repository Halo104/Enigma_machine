#include "errors.h"
#include "utils.h"

#include <iostream>
#include <vector>
#include <algorithm>

bool isNumeric(const std::string &inputString)
{
  const int length = inputString.size();
  int count = 0;
  for (int i = 0; i < length; ++i)
  {
    if (isdigit(inputString[i]))
    {
      count = count + 1;
    }
  }
  if (count == length)
  {
    return true;
  }
  return false;
}

bool isBetween0to25(const int newValue)
{
  if (newValue >= 0 && newValue <= 25)
  {
    return true;
  }
  return false;
}

int convertAlphaToIndex(char character)
{
  int index = static_cast<int>(character) - 65;
  return index;
}

char convertIndexToAlpha(int index)
{
  char alpha = static_cast<char>(index) + 65;
  return alpha;
}

void removeSpaces(std::string &inputString)
{
  inputString.erase(remove(inputString.begin(), inputString.end(), ' '), inputString.end());
}

void removeTabs(std::string &inputString)
{
  inputString.erase(remove(inputString.begin(), inputString.end(), '\t'), inputString.end());
}

bool isBetweenAandZ(const char inputStringChar)
{
  if (inputStringChar < 'A' || inputStringChar > 'Z')
  {
    return false;
  }
  return true;
}

bool hasEvenNumIndex(const std::vector<int> &data)
{
  int length = data.size();
  if (length % 2 == 0)
  {
    return true;
  }
  return false;
}

bool alreadyHasValue(const int newValue, const std::vector<int> &data)
{
  int length = data.size();
  for (int i = 0; i < length; ++i)
  {
    if (data[i] == newValue)
    {
      return true;
    }
  }
  return false;
}

int repeatValueLoc(const int newValue, const std::vector<int> &data)
{
  int length = data.size();
  int existLocation = 0;
  for (int i = 0; i < length; ++i)
  {
    if (data[i] == newValue)
    {
      existLocation = i;
    }
  }
  return existLocation;
}

std::string fileNameAfterDash(const std::string &fileLink)
{
  //Returns the filename, after the dash "/" of the link
  //Splitting the input of fileName by '/'
  int length = fileLink.length();
  std::string beforeDash;
  std::string afterDash;
  bool dashToken = false;

  for (int i = 0; i < length; i++)
  {
    if (dashToken == false)
    {
      beforeDash += fileLink[i];
    }
    else
    {
      afterDash += fileLink[i];
    }
    if (fileLink[i] == '/')
    {
      dashToken = true;
    }
  }
  return afterDash;
}

int commandLineHandling(int argc, char **argv)
{
  //Deals with checking that the input commandline contains enough arguments
  //and all the files required exists

  if (argc < 4)
  {
    std::cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions" << std::endl;
    return INSUFFICIENT_NUMBER_OF_PARAMETERS;
  }

  return NO_ERROR;
}

bool isFileExtentionValid(std::string fileName, std::string fileExtension)
{
  //Function used to check that the file extension is as intended
  int length = fileName.length();
  std::string beforeDot;
  std::string afterDot;
  bool dotToken = false;

  //Splitting the input of fileName by '.'
  for (int i = 0; i < length; i++)
  {
    if (dotToken == false)
    {
      beforeDot += fileName[i];
    }
    else
    {
      afterDot += fileName[i];
    }
    if (fileName[i] == '.')
    {
      dotToken = true;
    }
  }
  if (afterDot.compare(fileExtension))
  {
    return 0;
  }
  return 1;
}

int requestStringInput(std::string &inputString)
{
  std::cin >> std::ws;
  getline(std::cin, inputString);

  removeSpaces(inputString);
  removeTabs(inputString);

  if (inputString == "\0")
  {
    return lineFinished;
  }
  if (std::cin.eof())
  {
    return lineFinished;
  }
  return NO_ERROR;
}

void incrementNotches(Rotor &rotor0, Rotor &rotor1)
{
  //Increment rotor location if the previous rotor hits the linkage/notch location
  int numNotches = rotor0.linkValue_.size();

  if (rotor0.hasReachedLinkValue() && rotor1.notchResetToken == true)
  {
    rotor1.incrementConfigLocation();
    rotor1.notchResetToken = false;
  }

  int numCheck = 0;
  for (int j = 0; j < numNotches; ++j)
  {
    if (rotor0.getConfig() != rotor0.linkValue_[j])
    {
      ++numCheck;
    }
  }
  if (numCheck == numNotches)
  {
    rotor1.notchResetToken = true;
  }
}
