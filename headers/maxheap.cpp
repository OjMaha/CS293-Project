#include <iostream>
#include "maxheap.h"

class maxheap
{
public:
  // global root for the entire tree
  std::vector<int> store;
  unsigned sz = 0;
  // Constructor
  maxheap() {};

  // store utilities
  int  size();        // returns the size
  void append(int k); // appends key in store
  // maxheap Navigation
  int parent(int);
  int left(int);
  int right(int);

  // maxheap interface
  void swap(int i, int j);
  int max();          // read max
  void insert(int k); // insert key in maxheap
  void heapify(int i);// heapify a node
  void deleteMax();   // deletes max
  void buildHeap();   // builds maxheap
  void heapSort();    // sorts conentens of maxheap and does not physically delete
                      // the content

};

int maxheap::parent(int i) {
  if (i == 0) return -1;
  return int((i-1)/2); 
}

int maxheap::left(int i) {
  if (2*i + 1 > sz - 1) return -1;
  return 2*i + 1; 
}

int maxheap::right(int i) {
  if (2*i + 2 > sz - 1) return -1;
  return 2*i + 2; 
}

int maxheap::max() {
  if (sz == 0) return -1;
  return store[0]; 
}

void maxheap::swap(int i, int j) {
  int temp = store[i];
  store[i] = store[j];
  store[j] = temp;
}

void maxheap::insert(int v) {
  append(v);
  int temp = sz - 1;
  while (store[parent(temp)] < store[temp]){
    if (temp == 0) break;
    swap(temp, int((temp - 1)/2));
    temp = parent(temp);
  }
}

void maxheap::heapify(int i) {
  if (left(i) >= sz && right(i) >= sz) {return;}

  if (right(i) >= sz){
    if (store[left(i)] > store[i]) swap(i, left(i));
    return;
  }

  if (store[left(i)] >= store[right(i)]) {
    swap(i, left(i));
    heapify(left(i));
  }

  else{
    swap(i, right(i));
    heapify(right(i));
  }
}


void maxheap::deleteMax() {
  swap(0, sz-1);
  store[sz - 1] = 0;
  sz -= 1;
  heapify(0);
}

void maxheap::buildHeap() {
  for (int i = sz - 1; i > 0; i--) heapify(i);
}

void maxheap::heapSort() {
  while (sz > 0){
    swap(0, sz - 1);
    sz-=1;
    heapify(0);
  }
}

int maxheap::size() {
  return sz;
}

void maxheap::append(int k) {
  if( sz == store.size() )
    store.push_back(k); // expand if more storage is needed!
  else
    store[sz] = k;
  sz = sz + 1;
}


