#ifndef _UTILS_H_
#define _UTILS_H_

#include "Rotor.h"

#include <string>
#include <vector>

bool isNumeric(const std::string &inputString);
bool isBetween0to25(const int newValue);

int convertAlphaToIndex(char character);
char convertIndexToAlpha(int index);
void removeSpaces(std::string &inputString);
void removeTabs(std::string &inputString);
bool isBetweenAandZ(const char inputStringChar);
int commandLineHandling(int argc, char **argv);
bool isFileExtentionValid(std::string fileName, std::string fileExtension);
std::string fileNameAfterDash(const std::string &fileLink);
bool hasEvenNumIndex(const std::vector<int> &data);
bool alreadyHasValue(const int newValue, const std::vector<int> &data);
int repeatValueLoc(const int newValue, const std::vector<int> &data);
int requestStringInput(std::string &inputString);
void incrementNotches(Rotor &rotor0, Rotor &rotor1);

enum exitCodes
{
    lineFinished = 100,
    exitInput = 1
};

#endif //_UTILS_H_
