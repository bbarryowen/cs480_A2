#include <fstream>
#include <mutex>
#include <queue>
#include <vector>

// data structure that will be shared across threads
struct sharedDataStructure
{
    std::ifstream *vocab;
    std::ifstream *textFile;

    char *textName;
    char *vocabName;
    int numMarks;
    int numHashes;
    int countLimit;

    std::mutex *textMutex;
    std::queue<char *> *textLines;
    std::vector<char *> *vocabWords;
    std::ofstream *outFile;

    bool *readingTextFlag;
    int *numTextLines;
    int *processedTextLines;
    int *numVocabChars;
    int *processedVocabChars;
    int *numVocabLines;
};