#include <iostream>
#include <fstream>

using namespace std;

// initialize streams to stdin and stdout
istream *in = &cin;
ostream *out = &cout;

// debugging function
template<typename T>
void printArr(T values[], int low, int high) {
  for (int i = low; i <= high; ++i) {
    cout << values[i] << ", ";
  }
  cout << endl;
}

template<typename T> _Task Mergesort {
    bool isRoot; // denotes whether this mergesort is at the top level or not
    T *values;
    int low;
    int high;
    int depth;

    void mergeSort(T values[], int low, int high) {
      if (high <= low) {
        return;
      }

      int middle = low + (high - low)/2;

      // call mergesort on left partition
      if (depth > 0) { // create task
        Mergesort<T> left(values, low, middle, depth - 1, false);
      } else { // mergesort recursively
        mergeSort(values, low, middle);
      }

      // call mergesort on right partition
      mergeSort(values, middle + 1, high);

      // merge left and right partitions together
      merge(values, low, middle, high);
    }

    // function to sort and merge 2 partitions 
    void merge(T values[], int low, int middle, int high) {
      cout << "merge" << endl;
      cout << "low: " << low << ", middle: " << middle << ", high: " << high << endl;
      int leftSize = middle - low + 1;
      int rightSize = high - middle;

      T* left = new T[leftSize];
      T* right = new T[rightSize];

      memcpy(left, &values[low], leftSize * sizeof(T));
      memcpy(right, &values[middle + 1], rightSize * sizeof(T));

      // pointers to beginning of left and right partitions
      int i = 0;
      int j = 0;

      // temp
      printArr(left, 0, leftSize - 1);
      printArr(right, 0, rightSize - 1);

      // place sorted values into values array
      while (i != leftSize && j != rightSize) {
        if (left[i] < right[j]) {
          values[low + i + j] = left[i];
          i++;
        } else {
          values[low + i + j] = right[j];
          j++;
        }
      }

      if (i == leftSize) { // fill remaining from right partition
        for (; j < rightSize; j++) {
          values[low + i + j] = right[j];
        }
      } else { // fill remaining from left partition
        for (; i < leftSize; i++) {
          values[low + i + j] = left[i];
        }
      }

      printArr(values, 0, 3);

      // clean up temporary variables
      delete[] left;
      delete[] right;

      return;
    }

    void main() {
      verify();

      mergeSort(values, low, high);
    }

    // prob don't need?
    // private constructor that is used to mark a task as non-top-level
    Mergesort(T values[], unsigned int low, unsigned int high, unsigned int depth, bool isRoot) {
      this->values = values; 
      this->low = low;
      this->high = high;
      this->depth = depth;
      this->isRoot = isRoot;
    }
  public:
    Mergesort( T values[], unsigned int low, unsigned int high, unsigned int depth ) {
      this->values = values; 
      this->low = low;
      this->high = high;
      this->depth = depth;
      this->isRoot = true;
    }
};

unsigned int uDefaultStackSize() {
    return 512 * 1000;        // set task stack-size to 512K
}

// prints usage information
void usage(char *arg) {
  cerr << "Usage: " << arg
    << " ( -s unsorted-file [ sorted-file ] |"
    << " -t size (>= 0) [ depth (>= 0) ] )" << endl;
  exit(EXIT_FAILURE);
}

int tryParseStringToNumber(char* arg) {
  char* endptr;

  long val = strtol(arg, &endptr, 10);

  if (*endptr == 0) { // successfully parsed string
    return int(val);
  } else {
    return -1; // will never get here, but gets rid of compiler warning
  }
}

int main( int argc, char *argv[] ) {
  // -t command arguments
  int depth = 0;
  int size;

  if (argc < 3) {
    usage(argv[0]); // exit early if insufficient arguments
  }

  // parse command line args
  if (argv[1][1] == 's') {
    switch(argc) {
      case 4:
				try {
					out = new ofstream(argv[3]);
				} catch(uFile::Failure) {
					cerr << "Error! Could not open sorted output file \"" << argv[3] << "\"" << endl;
          usage(argv[0]);
				}
      case 3:
				try {
					in = new ifstream(argv[2]);
				} catch(uFile::Failure) {
					cerr << "Error! Could not open unsorted input file \"" << argv[2] << "\"" << endl;
          usage(argv[0]);
				}
        break;
      default:
        usage(argv[0]);
    }
  } else if (argv[1][1] == 't') {
    switch(argc) {
      case 4:
				depth = tryParseStringToNumber(argv[3]);
        if (depth < 0) {
          usage(argv[0]);
        }
      case 3:
        size = tryParseStringToNumber(argv[2]);
        if (size < 0) {
          usage(argv[0]);
        }
        break;
      default:
        usage(argv[0]);
    }
  } else {
    usage(argv[0]);
  }

  uProcessor p[ (1 << depth) - 1 ] __attribute__(( unused )); // 2^depth-1 kernel threads

  // execute command
  if (argv[1][1] == 's') {
    int size;
    int val;
    for (;;) {
      if (in->fail()) { // might need to do read to trigger this shit? maybe move this to top of loop?
        break;
      }

      *in >> size; // read size of array
      int *arr = new int[size];

      // read values for unsorted array
      for (int i = 0; i < size; ++i) {
        *in >> val;
        cout << val << endl;
        arr[i] = val;  
      }

      printArr(arr, 0, size - 1);

      {
        Mergesort<int> sorted(arr, 0, size - 1, depth);
      }

      printArr(arr, 0, size - 1);

      delete[] arr; // clean up array
    }
  } else if (argv[1][1] == 't') {
    int *arr = new int[size]; // dimension integer array to size
    for (int i = 0; i < size; ++i) { // initialize array in descending order
      arr[i] = size - i - 1;
    }

    printArr(arr, 0, size - 1);

    {
      Mergesort<int> sorted(arr, 0, size - 1, depth);
    }

    printArr(arr, 0, size - 1); // prints sorted

    delete[] arr;
  }

	// close file streams
  if (in != &cin) {
    delete in;
  }
  if (out != &cout) {
    delete out;
  }
}
