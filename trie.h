// 825730146
class Trie
{

public:
  struct Node
  {
    char data;
    Node *nextList[31];
    Node(char letter) : data(letter)
    {
      for (int i = 0; i < 31; i++)
      {
        nextList[i] = nullptr;
      }
    }
  };
  bool insertWord(char *word, Trie trie);
  bool findVocab(char *vocab, Trie trie);
  char *slice(char *array, int start);

  Trie();

  Node *root;

  // deconstructor
  void destroyTrie(Node *node);
};