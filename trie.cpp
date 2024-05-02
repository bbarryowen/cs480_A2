// 825730146
#include "trie.h"
#include <cstring>
#include <vector>
#include <fstream>

Trie::Trie()
{
  root = new Node('\0');
}

char *Trie::slice(char *array, int start)
{
  size_t arrayLen = strlen(array) + 1;
  char *outArray = (char *)malloc(arrayLen - start);
  for (int i = start; i < arrayLen; i++)
  {
    outArray[i - start] = array[i];
  }
  return outArray;
}

bool Trie::insertWord(char *word, Trie trie)
{
  Node *currNode;
  char *slicedWord;

  // outer for loop iterates over each index of the word and sets sliced word to the suffix
  for (size_t i = 0; i < strlen(word) + 1; i++)
  {
    slicedWord = slice(word, i);
    currNode = trie.root;
    // iterates over each index of the sliced word and adds it to the trie
    for (size_t j = 0; j < strlen(slicedWord); j++)
    {
      int loc;
      char letter = slicedWord[j];

      if (tolower(letter) >= 97 && tolower(letter) <= 122)
      {
        loc = int(tolower(letter)) - 97;
      }
      else if (letter == 0)
      {
        currNode->nextList[30] = new Node('\0');
        return true;
      }
      else if (letter == 39)
      {
        loc = 26;
      }
      else if (letter == 45)
      {
        loc = 27;
      }
      else if (letter == 95)
      {
        loc = 28;
      }
      else if (letter == 32)
      {
        loc = 29;
      }
      else
      {
        loc = -1;
      }

      // if a node doesnt exist at the location in nextList then a node with the letter is created, else currNode is set to that node
      if (loc != -1)
      {
        if (currNode->nextList[loc] == nullptr)
        {
          currNode->nextList[loc] = new Node(tolower(letter));
        }
        currNode = currNode->nextList[loc];
      }
    }
  }
  return false;
}

bool Trie::findVocab(char *vocab, Trie trie)
{
  int loc;
  Node *currNode = trie.root;
  char letter;
  for (size_t i = 0; i < strlen(vocab) + 1; i++)
  {
    letter = vocab[i];
    if (tolower(letter) >= 97 && tolower(letter) <= 122)
    {
      loc = int(tolower(letter)) - 97;
    }
    else if (letter == 0)
    {
      return true;
    }
    else if (letter == 39)
    {
      loc = 26;
    }
    else if (letter == 45)
    {
      loc = 27;
    }
    else if (letter == 95)
    {
      loc = 28;
    }
    else if (letter == 32)
    {
      loc = 29;
    }
    else
    {
      return false;
    }

    // checks if nextList where the letter would be is not null and moves to that node, else the vocab word is not in that line
    if (loc != -1)
    {
      if (currNode->nextList[loc] != nullptr)
      {
        currNode = currNode->nextList[loc];
      }
      else
      {
        return false;
      }
    }
  }
  return true;
}

void Trie::destroyTrie(Node *node)
{
  if (!node)
    return;

  for (int i = 0; i < 31; ++i)
  {
    destroyTrie(node->nextList[i]);
  }

  delete node;
}
