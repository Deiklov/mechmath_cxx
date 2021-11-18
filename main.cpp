#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

using namespace std;

void bubbleSort(
    vector<double>::iterator first,  // Beginning of array segment to sort
    vector<double>::iterator last    // After-end of array segment to sort
);
void directSort(vector<double>::iterator first, vector<double>::iterator last);

void heapSort(vector<double>::iterator first, vector<double>::iterator last);

void bubbleDown(vector<double>::iterator first,  // Beginning of array segment
                size_t i,  // index of element to sieve in the array segment
                size_t n   // size of the array segment
);

void merge(vector<double>::iterator a0,  // Beginning of first array
           vector<double>::iterator a1,  // After-end of first array
           vector<double>::iterator b0,  // Beginning of second array
           vector<double>::iterator b1,  // After-end of second array
           vector<double>::iterator c    // Beginning of resulting array
);

void mergeSortRecursive(
    vector<double>::iterator first,     // Beginning of array segment to sort
    vector<double>::iterator last,      // After-end of array segment to sort
    std::vector<double>* tmpMemory = 0  // Temporary memory to use
);

void mergeSortBottomTop(
    vector<double>::iterator first,     // Beginning of array segment to sort
    vector<double>::iterator last,      // After-end of array segment to sort
    std::vector<double>* tmpMemory = 0  // Temporary memory to use
);

void printArray(vector<double>::iterator first, vector<double>::iterator last);

// int main() {
//   time_t tim;
//   tim = time(&tim);
//   srand(int(tim));
//   vector<double> a;
//   while (true) {
//     int n;
//     cout << "n: ";
//     cin >> n;
//     if (n <= 0) break;
//
//     cout << "Enter number of sorting algorithm:" << endl;
//     cout << "1 BubbleSort, 2 DirectSort, 3 HeapSort," << endl;
//     cout << "4 Recursive merge sort, 5 Bottom-Top merge sort" << endl;
//     int sortAlg;
//     cin >> sortAlg;
//
//     // Generate a random array of size n
//     a.resize(n);
//     for (int i = 0; i < n; ++i) {
//       double x = (double(rand()) / double(RAND_MAX)) * 1000.;
//       a[i] = x;
//     }
//     cout << "Random array:" << endl;
//     printArray(a.begin(), a.end());
//     cout << "-------------" << endl;
//
//     if (sortAlg == 1)
//       bubbleSort(a.begin(), a.end());
//     else if (sortAlg == 2)
//       directSort(a.begin(), a.end());
//     else if (sortAlg == 3)
//       heapSort(a.begin(), a.end());
//     else if (sortAlg == 4)
//       mergeSortRecursive(a.begin(), a.end());
//     else if (sortAlg == 5)
//       mergeSortBottomTop(a.begin(), a.end());
//     else
//       continue;
//     cout << "Sorted array:" << endl;
//     printArray(a.begin(), a.end());
//     cout << "-------------" << endl;
//   }
//   return 0;
// }

void printArray(vector<double>::iterator a, vector<double>::iterator last) {
  size_t n = last - a;
  if (n <= 1000) {
    // Print all array
    for (size_t i = 0; i < n; ++i) {
      if (i > 0) {
        // Print a delimiter
        //        if (i % 5 == 0)
        //          cout << endl;
        //        else
        cout << " ";
      }
      cout << a[i];
    }
  } else {
    // The array is too big, print its beginning and end only
    // (100 first and 100 last elements)
    for (size_t i = 0; i < 100; ++i) {
      if (i > 0) {
        // Print a delimiter
        if (i % 5 == 0)
          cout << endl;
        else
          cout << " ";
      }
      cout << a[i];
    }
    cout << endl << ". . ." << endl;
    for (size_t i = n - 100; i < n; ++i) {
      if (i > n - 100) {
        // Print a delimiter
        if (i % 5 == 0)
          cout << endl;
        else
          cout << " ";
      }
      cout << a[i];
    }
  }
  cout << endl;
}

int main() {
  vector<double> arr{1, 12, 53,  -2,  -11, 45,  89, 32,
                     8, 19, -32, -67, 100, 100, 12, -11};
  vector<double> tmpMemory(arr.size());
  //
  mergeSortRecursive(arr.begin(), arr.end(), &tmpMemory);
  printArray(tmpMemory.begin(), tmpMemory.end());
  cout << "kek" << endl;
  return 0;
}

void mergeSortRecursive(vector<double>::iterator a,
                        vector<double>::iterator last,
                        vector<double>* tmpMemory) {
  size_t n = distance(a, last);
  if (n <= 3) {
    directSort(a, last);
    return;
  }

  size_t k = n / 2;

    mergeSortRecursive(a, a+k, tmpMemory);
    mergeSortRecursive(a+k, last, tmpMemory);
    ::merge(a, a+k, a+k, last, tmpMemory->begin());
    cout << "kek" << endl;

}

void merge(vector<double>::iterator a0,  // Beginning of first array
           vector<double>::iterator a1,  // After-end of first array
           vector<double>::iterator b0,  // Beginning of second array
           vector<double>::iterator b1,  // After-end of second array
           vector<double>::iterator c    // Beginning of resulting array
) {
  while (a0 != a1 && b0 != b1) {
    if (*a0 <= *b0) {
      *c = *a0;
      ++a0;
    } else {
      *c = *b0;
      ++b0;
    }
    ++c;
  }

  while (a0 != a1) {
    *c = *a0;
    ++a0;
    ++c;
  }

  while (b0 != b1) {
    *c = *b0;
    ++b0;
    ++c;
  }
}

void mergeSortBottomTop(vector<double>::iterator first,
                        vector<double>::iterator last,
                        std::vector<double>* tmpMemory) {
  //... to be implemented...
  cout << "mergeSortBottomTop is not implemented yet..." << endl;
}
void directSort(
    vector<double>::iterator a,    // Beginning of array segment to sort
    vector<double>::iterator last  // After-end of array segment
) {
  size_t n = last - a;
  for (size_t i = 0; i < n - 1; ++i) {
    size_t indMin = i;
    for (size_t j = i + 1; j < n; ++j) {
      if (a[j] < a[indMin]) indMin = j;
    }
    if (indMin != i) swap(a[i], a[indMin]);
  }
}