#include <thread>
#include <fstream>
#include <cstring>
#include <mutex>
#include <vector>
#include <iostream>
#include <queue>
#include <type_traits>
#include <pthread.h>

#include "readvocab.h"
#include "readlines.h"
#include "countvocabstrings.h"
#include "SHARED_DATA.h"

int checkArgs(int numArgs, char *argsList[])
{
  // checks if number of arguments is correct
  if (numArgs < 3)
  {
    printf("Error: expected atleast 2 arguments and got %d\n", numArgs - 1);
    return 1;
  }

  // loads in files
  const std::string argument1 = argsList[1];
  std::ifstream file1(argument1);

  const std::string argument2 = argsList[2];
  std::ifstream file2(argument2);
  int check = 0;

  // checks if file1 exists
  if (!file1)
  {
    printf("Unable to open <<%s>>\n", argument1.c_str());
    check = 1;
  }
  // checks if file2 exists
  if (!file2)
  {
    printf("Unable to open <<%s>>\n", argument2.c_str());
    check = 1;
  }
  // returns 1 if either of the files do not exists, else returns 0
  if (check != 0)
  {
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  int argsCode = checkArgs(argc, argv);
  // ends program with return code 1 if arguments are not to specifications
  if (argsCode != 0)
  {
    exit(1);
  }

  // sets default values for -p, -m and -v
  int numMarks = 50;
  int numHashes = 10;
  int countLimit = 0;

  // sets -p -m and -v to commmand line input if given
  for (int i = 3; i < argc; i++)
  {
    if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
    {
      numMarks = std::stoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-m") == 0 && i + 1 < argc)
    {
      numHashes = std::stoi(argv[i + 1]);
    }
    if (strcmp(argv[i], "-v") == 0 && i + 1 < argc)
    {
      countLimit = std::stoi(argv[i + 1]);
    }
  }
  // checks that args are within correct limits and exits if not
  if ((numHashes <= 0 || numHashes > 10) || (numMarks < 10))
  {
    if (numHashes <= 0 || numHashes > 10)
    {
      printf("Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10\n");
    }
    if (numMarks < 10)
    {
      printf("Number of progress marks must be a number and at least 10\n");
    }
    exit(1);
  }

  // reads in file arguments
  int vocabIndex = 1;
  int textIndex = 2;
  std::ifstream vocab(argv[vocabIndex]);
  std::ifstream textFile(argv[textIndex]);
  char *textName = argv[textIndex];
  char *vocabName = argv[vocabIndex];

  // initializes values for reading vocab and lines
  std::mutex textMutex;
  std::queue<char *> textLines;
  std::vector<char *> vocabWords;
  std::ofstream outFile("countNumOfContainedVocab.txt", std::ofstream::out | std::ofstream::trunc);
  bool readingFlag = true;
  struct sharedDataStructure sharedData;

  // sets values of sharedData
  sharedData.vocab = &vocab;
  sharedData.textFile = &textFile;
  sharedData.vocabName = vocabName;
  sharedData.textName = textName;
  sharedData.countLimit = countLimit;
  sharedData.numHashes = numHashes;
  sharedData.numMarks = numMarks;
  sharedData.textMutex = &textMutex;
  sharedData.outFile = &outFile;
  sharedData.textLines = &textLines;
  sharedData.vocabWords = &vocabWords;
  sharedData.readingTextFlag = &readingFlag;

  // gets the number of cahracters in vocab and sets sharedData
  int charCount = 0;
  int vocabLineCount = 0;
  std::string line;
  while (getline(*sharedData.vocab, line))
  {
    charCount += line.length();
    vocabLineCount += 1;
  }
  sharedData.numVocabLines = &vocabLineCount;
  sharedData.numVocabChars = &charCount;
  int processedChars = 0;
  sharedData.processedVocabChars = &processedChars;

  sharedData.vocab->clear();
  sharedData.vocab->seekg(0, std::ios::beg);

  // gets the number of lines in text and sets sharedData
  int lineCount = 0;
  while (getline(*sharedData.textFile, line))
  {
    lineCount += 1;
  }
  sharedData.numTextLines = &lineCount;
  int processedLines = 0;
  sharedData.processedTextLines = &processedLines;

  sharedData.textFile->clear();
  sharedData.textFile->seekg(0, std::ios::beg);

  // initializes values for calculating and printing progress
  double dashPercent = 100.0 / sharedData.numMarks;
  double percentComplete;
  int numPrinted;
  double numNeeded;
  int mCount;

  // initializes values for threads and starts read vocab and read lines threads
  pthread_t vocabThread;
  pthread_t textThread;
  pthread_t countThread;
  pthread_create(&vocabThread, NULL, &readVocab, &sharedData);
  pthread_create(&textThread, NULL, &readLines, &sharedData);

  // printing progress of reading vocab
  percentComplete = 0;
  numPrinted = 0;
  numNeeded = 0;
  mCount = 0;

  while (true)
  {
    percentComplete = (double(*sharedData.processedVocabChars) / *sharedData.numVocabChars) * 100;
    numNeeded = percentComplete / dashPercent;
    for (int i = numPrinted; i < numNeeded; i++)
    {
      mCount = (mCount + 1) % sharedData.numHashes;
      if (mCount == 0)
      {
        std::cout << "#";
      }
      else
      {
        std::cout << "-";
      }
      numPrinted += 1;
    }
    if (*sharedData.processedVocabChars == *sharedData.numVocabChars)
    {
      std::cout << "\nThere are " << *sharedData.numVocabLines << " lines in " << sharedData.vocabName << std::endl;
      break;
    }
  }

  // joining vocab thread and starting count thread
  pthread_join(vocabThread, nullptr);
  pthread_create(&countThread, NULL, &countVocabStrings, &sharedData);

  // prining progress of counting voab strings
  percentComplete = 0;
  numPrinted = 0;
  numNeeded = 0;
  mCount = 0;

  while (true)
  {
    percentComplete = (double(*sharedData.processedVocabChars) / *sharedData.numVocabChars) * 100;
    numNeeded = percentComplete / dashPercent;
    for (int i = numPrinted; i < numNeeded; i++)
    {
      mCount = (mCount + 1) % sharedData.numHashes;
      if (mCount == 0)
      {
        std::cout << "#";
      }
      else
      {
        std::cout << "-";
      }
      numPrinted += 1;
    }
    if (*sharedData.processedTextLines == *sharedData.numTextLines)
    {
      std::cout << "\nThere are " << *sharedData.numTextLines << " lines in " << sharedData.textName << std::endl;
      break;
    }
  }

  // joining text thread and setting read flag to false so the count thread knows no more values are being pushed to text queue
  pthread_join(textThread, nullptr);
  readingFlag = false;
  pthread_join(countThread, nullptr);

  outFile.close();

  return 0;
}
