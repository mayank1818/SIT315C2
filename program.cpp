#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;
using namespace chrono;

// Generate a vector with random values
vector<int> generateVector(size_t length) {
    srand(static_cast<unsigned int>(time(0)));
    vector<int> vec;
    for (size_t p = 0; p < length; ++p) {
        vec.push_back(rand());
    }
    return vec;
}

// Perform divide and return the pivot index
int divide(vector<int>& array, int min, int max) {
    int pivot = array[max];
    int i = min - 1;

    for (int j = min; j < max; ++j) {
        if (array[j] <= pivot) {
            i++;
            swap(array[i], array[j]);
        }
    }
    swap(array[i + 1], array[max]);
    return i + 1;
}

// Implement parallel quicksort algorithm using OpenMP tasks
void parallelQuicksort(vector<int>& array, int min, int max) {
    if (min < max) {
        int pivotIndex = divide(array, min, max);

        // Use task-based parallelism instead of sections
        #pragma omp task
        parallelQuicksort(array, min, pivotIndex - 1);

        // Recurse on the right partition in parallel
        parallelQuicksort(array, pivotIndex + 1, max);
    }
}

int main() {
    // Set the size of the vector
    size_t vectorSize = 20000000;

    // Generate a random vector
    vector<int> array = generateVector(vectorSize);

    // Measure time before sorting
    auto start = high_resolution_clock::now();

    #pragma omp parallel
    #pragma omp single
    // Perform parallel quicksort
    parallelQuicksort(array, 0, array.size() - 1);

    // Measure time after sorting
    auto end = high_resolution_clock::now();

    // Calculate time based on input size
    auto elapsed = duration_cast<milliseconds>(end - start);

    cout << "Vector Size: " << array.size() << endl;
    cout << "Time taken by parallel program is " << elapsed.count() << " milliseconds" << endl;

    return 0;
}
