#include <iostream>
#include <vector>
#include <string>
using namespace std;



// sell_priority_queue class
class sell_priority_queue
{
public:
  // global root for the entire tree
  std::vector<order> store;
  unsigned sz = 0;
  // Constructor
  sell_priority_queue() {};

  // store utilities
  int  size();        // returns the size
  void append(order k); // appends key in store
  // sell_priority_queue Navigation
  int parent(int);
  int left(int);
  int right(int);

  // sell_priority_queue interface
  void swap(int i, int j);
  order max();          // read max
  void push(order k); // insert key in sell_priority_queue
  void heapify(int i);// heapify a node
  void pop();   // deletes max
  void buildHeap();   // builds sell_priority_queue
  void heapSort();    // sorts conentens of sell_priority_queue and does not physically delete
                      // the content
  bool compare(order a, order b);

};

bool sell_priority_queue::compare(order a, order b){  //if b > a then return true 
  // Compare prices in descending order
        if (a.price != b.price) {
            return a.price > b.price;
        }

        if(a.expiry == -1 && b.expiry == -1) return a.broker > b.broker;
        if(a.expiry == -1) return true;
        if(b.expiry == -1) return false;
        
        // If prices are equal, compare expiry times in ascending order
        if (a.expiry != b.expiry) {
            return a.expiry > b.expiry;
        }

        // If both price and expiry time are equal, compare broker names alphabetically
        return a.broker > b.broker;
}

int sell_priority_queue::parent(int i) {
  if (i == 0) return -1;
  return int((i-1)/2); 
}

int sell_priority_queue::left(int i) {
  if (2*i + 1 > sz - 1) return -1;
  return 2*i + 1; 
}

int sell_priority_queue::right(int i) {
  if (2*i + 2 > sz - 1) return -1;
  return 2*i + 2; 
}

order sell_priority_queue::max() {
  if (sz == 0) return order();
  return store[0]; 
}

void sell_priority_queue::swap(int i, int j) {
  order temp = store[i];
  store[i] = store[j];
  store[j] = temp;
}

void sell_priority_queue::push(order v) {
  append(v);
  int temp = sz - 1;
  while (compare(store[parent(temp)],store[temp])){
    if (temp == 0) break;
    swap(temp, int((temp - 1)/2));
    temp = parent(temp);
  }
}

void sell_priority_queue::heapify(int i) {
  if (left(i) >= sz && right(i) >= sz) {return;}

  if (right(i) >= sz){
    if (compare(store[i], store[left(i)])) swap(i, left(i));
    return;
  }

  if (compare(store[right(i)], store[left(i)])) {
    swap(i, left(i));
    heapify(left(i));
  }

  else{
    swap(i, right(i));
    heapify(right(i));
  }
}


void sell_priority_queue::pop() {
  swap(0, sz-1);
  store[sz - 1] = order();
  sz -= 1;
  heapify(0);
}

void sell_priority_queue::buildHeap() {
  for (int i = sz - 1; i > 0; i--) heapify(i);
}

void sell_priority_queue::heapSort() {
  while (sz > 0){
    swap(0, sz - 1);
    sz-=1;
    heapify(0);
  }
}

int sell_priority_queue::size() {
  return sz;
}

void sell_priority_queue::append(order k) {
  if( sz == store.size() )
    store.push_back(k); // expand if more storage is needed!
  else
    store[sz] = k;
  sz = sz + 1;
}


// int main(){
//   sell_priority_queue m;
//   order a;
//   a.price = 100;
//   a.expiry = 1;
//   a.broker = "a";
//   m.push(a);
//   a.price = 130;
//   a.expiry = 1;
//   a.broker = "b";
//   m.push(a);
//   a.price = 130;
//   a.expiry = 1;
//   a.broker = "c";
//   m.push(a);
 
//   for(int i = 0; i< 3; i++){
//     print_order(m.max());
//     m.pop();
//   }
// }
