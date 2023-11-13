#include <bits/stdc++.h>

class MinHeap {
public:
  // Store for the heap
  std::vector<int> store;
  unsigned sz = 0;
  int swap_count = 0;

  // Constructor
  MinHeap() {};

  // Store utilities
  int size();
  void append(int k);

  // Heap Navigation
  int parent(int i);
  int left(int i);
  int right(int i);

  // Heap interface
  void swap(int i, int j);
  int min();           // Get the minimum element
  void insert(int k);  // Insert a key into the min-heap
  void heapify(int i); // Heapify a node
  void deleteMin();    // Delete the minimum element
  void buildHeap();    // Build the heap
  void heapSort();     // Sort the contents of the heap (ascending order)
};


int MinHeap::parent(int i) {
  if (i == 0) return -1;
  return int((i - 1) / 2);
}

int MinHeap::left(int i) {
  if (2 * i + 1 > sz - 1) return -1;
  return 2 * i + 1;
}

int MinHeap::right(int i) {
  if (2 * i + 2 > sz - 1) return -1;
  return 2 * i + 2;
}

int MinHeap::min() {
  if (sz == 0) return -1;
  return store[0];
}

void MinHeap::swap(int i, int j) {
  int temp = store[i];
  store[i] = store[j];
  store[j] = temp;
  swap_count++;
}

void MinHeap::insert(int v) {
  append(v);
  int temp = sz - 1;
  while (temp > 0 && store[parent(temp)] > store[temp]) {
    swap(temp, parent(temp));
    temp = parent(temp);
  }
}

void MinHeap::heapify(int i) {
  int left_child = left(i);
  int right_child = right(i);
  int smallest = i;

  if (left_child < sz && store[left_child] < store[i])
    smallest = left_child;
  if (right_child < sz && store[right_child] < store[smallest])
    smallest = right_child;

  if (smallest != i) {
    swap(i, smallest);
    heapify(smallest);
  }
}

void MinHeap::deleteMin() {
  if (sz == 0) return;

  swap(0, sz - 1);
  store[sz - 1] = 0;
  sz -= 1;
  heapify(0);
}

void MinHeap::buildHeap() {
  for (int i = sz - 1; i >= 0; i--) {
    heapify(i);
  }
}

void MinHeap::heapSort() {
  while (sz > 0) {
    swap(0, sz - 1);
    sz -= 1;
    heapify(0);
  }
}

int MinHeap::size() {
  return sz;
}

void MinHeap::append(int k) {
  if (sz == store.size())
    store.push_back(k);
  else
    store[sz] = k;
  sz = sz + 1;
}