#include <bits/stdc++.h>
#include "../headers/bst.cpp"
using namespace std;

int main(){
    map <string, BST> m;
    m["a"] = BST();
    m["a"].insert(3);
    m["a"].insert(2);
    m["a"].insert(1);
    m["a"].insert(4);
    m["a"].inorder();
    cout<<m["a"].search(3)<<endl;
    cout<<m["a"].root->price<<endl;
}