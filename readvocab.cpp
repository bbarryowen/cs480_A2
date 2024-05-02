#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "SHARED_DATA.h"

void *readVocab(void *arg)
{
  sharedDataStructure *data = reinterpret_cast<sharedDataStructure *>(arg);

  std::string line;
  while (getline(*data->vocab, line))
  {
    char *lineCharArray = new char[line.length() + 1];
    std::strcpy(lineCharArray, line.c_str());

    data->vocabWords->push_back(lineCharArray);
    *data->processedVocabChars += line.length();
  }
  return NULL;
}
