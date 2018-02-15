#include <iostream>

using namespace std;

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

int main( int argc, char *argv[] ) {
  // uProcessor p[ (1 << depth) - 1 ] __attribute__(( unused )); // 2^depth-1 kernel threads
  
  int *arr = new int[1];
  arr[0] = 1;
  arr[1] = 4;
  arr[2] = 0;
  arr[3] = 9;

  printArr(arr, 0, 3); // prints unsorted

  {
    Mergesort<int> sorted(arr, 0, 3, 6); // try depth = 2
  }

  printArr(arr, 0, 3); // prints sorted

  delete[] arr;
}
