#include <iostream>
#include <fstream>
#include <unordered_set>
#include <sstream>

using namespace std;

class wordCounter {
 public:
  wordCounter(const string& filename) {
    file_.open(filename);

    if (!file_) {
      throw runtime_error("File could not be opened");
    }
  }

  ~wordCounter() {
    file_.close();
  }

  unsigned int countUniqueWords() {
    unordered_set<string> unique_words; //using an unordered set because there can be no repetitions in our buffer with unique words.
    
    #pragma omp parallel for //using this compiler directive to parallelize the for loop between threads.
    for (string line; getline(file_, line);) {
      istringstream stream(line); //converting a line string to an istringstream object. This allows us to read words from the string one by one.
      for (string word; stream >> word;) {
        //checking if there is already a word in our buffer, if there is such a word, the find() method will return iterator that points to the ement after last in our set.
        if (unique_words.find(word) == unique_words.end()) {
          unique_words.insert(word);
        }
      }
    }
    return unique_words.size();
  }

 private:
  ifstream file_; //file data container.
};

int main(int argc, char* argv[]) {

  //checking whether command line arguments were passed.
  if (argc < 2) {
    cout << "You must specify the file name as a command line argument" << endl;
    return 1;
  }

  wordCounter counter(argv[1]);
  unsigned int num_of_unique_words = counter.countUniqueWords();
  cout << "The number of unique words in your file: " << num_of_unique_words << endl;
  return 0;
}
