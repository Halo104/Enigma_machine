#include "errors.h"
#include "Reflector.h"
#include "utils.h"

#include <iostream>

int Reflector::openReflectorFile(const std::string& loadFromFile){
  std::ifstream file;
  
  file.open(loadFromFile);
  if(file.fail()){
    std::cerr << "ERROR: Reflector file couldn't be open" << std::endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  int ParseFileReturnValue = ParseFile(file, loadFromFile);
  file.close();
  
  return ParseFileReturnValue;
}

int Reflector::ParseFile(std::ifstream& file, const std::string& fileName){
  //Checks that all the values in the reflector file is as it should be, and stores the values into a vector
  std::string input;
  int n = 0;
  while(file >> input){
    //Checks that the next reflector value is OK
    if(!isNumeric(input)){
      std::cerr << "Non-numeric character in reflector file " << fileName << std::endl;
      return NON_NUMERIC_CHARACTER;
    }
    int inputValue = std::stoi(input);
    if(!isBetween0to25(inputValue)){
      std::cerr << "ERROR: Values in reflector file " << fileName << " isn't between 0 and 25" << std::endl;
      return INVALID_INDEX;
    }
    if(alreadyHasValue(inputValue,reflectorValues_)){
      std::cerr << "ERROR: Reflector file " << fileName << " contains repeating values" << std::endl;
      return INVALID_REFLECTOR_MAPPING;
    }

    reflectorValues_.push_back(inputValue);
    n++;
  }
  if(!hasEvenNumIndex(reflectorValues_)){
    std::cerr << "Incorrect (odd) number of parameters in reflector file " << fileName << std::endl;
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }
  if(!contain13Pairs()){
    std::cerr << "Insufficient number of mappings in reflector file: " << fileName << std::endl;
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  }
  return NO_ERROR;
}

bool Reflector::contain13Pairs(){
  //As the reflector should always have 13 pair of values, it checks that overall there is
  //exactly 26 values from the reflector
  int length = reflectorValues_.size();
  if(length == 26){
    return true;
  }else{
    return false;
  }
}

char Reflector::Decipher(const char input){
  //Reflects the input character to the required output character
  int ReflectorLength = 26;
  int outputStringIndex = 200;
  int inputStringIndex = convertAlphaToIndex(input);
  bool match = false;

  for(int j = 0; j<ReflectorLength; ++j){
    if(reflectorValues_[j] == inputStringIndex){
      match = true;
      if(j%2==0){
	outputStringIndex = reflectorValues_[j+1];
      }else{
	outputStringIndex = reflectorValues_[j-1];
      }
    }else if(match == false){
      outputStringIndex = inputStringIndex;
    }
  }
  char reflectorOutput = convertIndexToAlpha(outputStringIndex);
  
  return reflectorOutput;
}
