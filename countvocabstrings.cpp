#include <queue>
#include <mutex>
#include <vector>
#include <iostream>
#include <fstream>

#include "trie.h"
#include "SHARED_DATA.h"

void *countVocabStrings(void *arg)
{
  sharedDataStructure *data = reinterpret_cast<sharedDataStructure *>(arg);

  while (*data->readingTextFlag || !data->textLines->empty())
  {
    if (!data->textLines->empty())
    {
      std::unique_lock<std::mutex> lock(*data->textMutex);
      char *line = data->textLines->front();
      data->textLines->pop();
      lock.unlock();

      Trie wordsTrie = Trie();

      wordsTrie.insertWord(line, wordsTrie);
      int count = 0;
      bool isFound;
      // searches the created trie for each vocab word
      for (std::vector<char *>::size_type j = 0; j < data->vocabWords->size(); j++)
      {
        isFound = wordsTrie.findVocab(data->vocabWords->at(j), wordsTrie);
        if (isFound)
        {
          count += 1;
        }
      }
      if (count > data->countLimit)
      {
        *data->outFile << count << std::endl;
      }
      *data->processedTextLines += 1;

      wordsTrie.destroyTrie(wordsTrie.root);
    }
  }
  return NULL;
}
