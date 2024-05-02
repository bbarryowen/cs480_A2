#include <iostream>
#include <fstream>
#include <queue>
#include <cstring>
#include <mutex>

#include "SHARED_DATA.h"

void *readLines(void *arg)
{
  sharedDataStructure *data = reinterpret_cast<sharedDataStructure *>(arg);

  std::string line;

  while (getline(*data->textFile, line))
  {
    char *lineCharArray = new char[line.length() + 1];
    strcpy(lineCharArray, line.c_str());

    std::unique_lock<std::mutex> lock(*data->textMutex);
    data->textLines->push(lineCharArray);
    lock.unlock();
  }
  return NULL;
}
