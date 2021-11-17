#include "errors.h"
#include "PlugBoard.h"
#include "Reflector.h"
#include "Rotor.h"
#include "RotorPositions.h"
#include "utils.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
  //Validates input arguments
  {
    const int commandLineErrorCode = commandLineHandling(argc, argv);
    if (commandLineErrorCode != NO_ERROR)
    {
      return commandLineErrorCode;
    }
  }

  //Opening the files
  PlugBoard plugboard; //Creating a plugboard object and opening extracting data from the plugboard file
  {
    const int plugBoardErrorCode = plugboard.openPlugBoardFile(argv[1]);
    if (plugBoardErrorCode != NO_ERROR)
    {
      return plugBoardErrorCode;
    }
  }

  Reflector reflector; //Creating a reflector object and opening extracting data from the reflector file
  {
    const int reflectorErrorCode = reflector.openReflectorFile(argv[2]);
    if (reflectorErrorCode != NO_ERROR)
    {
      return reflectorErrorCode;
    }
  }

  int numRotors = argc - 4;
  RotorPositions rotorPositions; //Creating a rotor position object and opening extracting data from the rotor position file
  {
    const int rotorPositionsErrorCode = rotorPositions.openRotorPositions(argv[argc - 1], numRotors);
    if (rotorPositionsErrorCode != NO_ERROR)
    {
      return rotorPositionsErrorCode;
    }
  }

  auto createdRotors = Rotor::CreateRotors(rotorPositions, numRotors, argv);
  if (createdRotors.errorCode != NO_ERROR)
  {
    return createdRotors.errorCode;
  }
  auto &rotors = createdRotors.rotors;

  //Processing the input string to be encrypted/decypted
  int inputReturn = 0;
  do
  {
    string inputString;
    //Inputting character
    inputReturn = requestStringInput(inputString);

    if (inputReturn == NO_ERROR)
    {
      const int stringLength = inputString.size();
      for (int i = 0; i < stringLength; ++i)
      {
        const char inputStringChar = inputString[i];

        if (!isBetweenAandZ(inputStringChar))
        {
          cerr << inputStringChar << " is not a valid input character (input characters must be upper case letters A-Z)!" << endl;
          return INVALID_INPUT_CHARACTER;
        }

        char plugBoardOutput = '\0';
        char reflectorOutput = '\0';
        char rotorFWOutput = '\0';
        char rotorBKOutput = '\0';

        if (numRotors == 0)
        {
          plugBoardOutput = plugboard.Decipher(inputStringChar);
          reflectorOutput = reflector.Decipher(plugBoardOutput);
          plugBoardOutput = plugboard.Decipher(reflectorOutput);
        }
        else if (numRotors > 0)
        {
          plugBoardOutput = plugboard.Decipher(inputStringChar);

          //Incrementing first rotor location before circuit is closed
          rotors[0].incrementConfigLocation();
          rotorFWOutput = rotors[0].DecipherForward(plugBoardOutput);

          for (int i = 1; i < numRotors; ++i)
          {
            //Incrementing other rotors, if required
            incrementNotches(rotors[i - 1], rotors[i]);
            rotorFWOutput = rotors[i].DecipherForward(rotorFWOutput);
          }

          reflectorOutput = reflector.Decipher(rotorFWOutput);
          rotorBKOutput = rotors[numRotors - 1].DecipherBackward(reflectorOutput);
          for (int i = numRotors - 2; i >= 0; --i)
          {
            rotorBKOutput = rotors[i].DecipherBackward(rotorBKOutput);
          }
          plugBoardOutput = plugboard.Decipher(rotorBKOutput);
        }
        cout << plugBoardOutput;
      }
    }
    else if (inputReturn != lineFinished)
    {
      return inputReturn;
    }
  } while (inputReturn == NO_ERROR);

  return NO_ERROR;
}
