#include <iostream>
using namespace std;

class BST {
public:
    struct Node {
        int price;
        int quantity;
        Node* left;
        Node* right;
    };

    Node* root;

    Node* node(int p, int q)  
    {  
    Node* node = new Node;  
    node->price = p;
    node->quantity = q;  
    node->left = node->right = NULL;  
    return node;  
    }

    Node* node(int p){
    return node(p, 0);
    }

    Node* findMinimum(Node* cur) /*To find the inorder successor*/  
    {  
    while(cur->left != NULL) {  
        cur = cur->left;  
    }  
    return cur;  
    }  

    Node* insert(Node* root, int p, int q) /*Insert a node*/  
    {  
    if (root == NULL)  
        return node(p,q); /*return new node if tree is empty*/  
    if (p < root->price)  
        root->left = insert(root->left, p,q); /*insert node in left subtree*/
    else  
        root->right = insert(root->right, p,q); /*insert node in right subtree*/
    return root;  
    }  

    Node* insert(Node* root, int p) /*Insert a node*/  
    {  
    return insert(root, p, 0);
    }

   bool search(Node* &cur, int p, Node* &parent)  
    {  
    while (cur != NULL && cur->price != p)  
    {  
        parent = cur;  
        if (p < cur->price)  
            cur = cur->left;  
        else  
            cur = cur->right;  
    }

    // Check if the loop terminated because the node with price p was found
    return (cur != NULL);
    }

    bool search(int p) {
    Node* cur = root;
    Node* parent = NULL;
    
    while (cur != NULL && cur->price != p) {
        parent = cur;
        if (p < cur->price)
            cur = cur->left;
        else
            cur = cur->right;
    }

    return (cur != NULL);
    }

    void deletion(Node*& root, int p) /*function to delete a node*/  
    {  
    Node* parent = NULL;  
    Node* cur = root;  
    search(cur, p, parent); /*find the node to be deleted*/  
    if (cur == NULL)  
        return;  
    if (cur->left == NULL && cur->right == NULL) /*When node has no children*/  
    {  
        if (cur != root)  
        {  
            if (parent->left == cur)  
                parent->left = NULL;  
            else  
                parent->right = NULL;  
        }  
        else  
            root = NULL;  
        free(cur);       
    }  
    else if (cur->left && cur->right)  
    {  
        Node* succ  = findMinimum(cur->right);  
        int val = succ->price;;  
        deletion(root, succ->price);  
        cur->price = val;  
    }  
    else  
    {  
        Node* child = (cur->left)? cur->left: cur->right;  
        if (cur != root)  
        {  
            if (cur == parent->left)  
                parent->left = child;  
            else  
                parent->right = child;  
        }  
        else  
            root = child;  
        free(cur);  
    }  
    } 

    BST() : root(nullptr) {}

    void insert(int p, int q) {
        root = insert(root, p, q);
    }

    void insert(int p) {
        root = insert(root, p);
    }

    void remove(int p) {
        deletion(root, p);
    }

    void inorder() {
        inorder(root);
    }

    ~BST() {
        // Implement deletion of all nodes in the destructor
        // to avoid memory leaks.
        deleteTree(root);
    }


    /*Inorder traversal of the tree formed*/  
    void inorder(Node *root)  
    {  
        if (root == NULL)  
            return;  
        inorder(root->left); //traverse left subtree  
        cout<< root->price << "   "; //traverse root node  
        inorder(root->right); //traverse right subtree  
    }  

        void deleteTree(Node* cur) {
            if (cur == NULL) {
                return;
            }
            deleteTree(cur->left);
            deleteTree(cur->right);
            delete cur;
        }
    };

// int main() {
//     BST myBST;

//     // Example usage
//     myBST.insert(50, 5);
//     myBST.insert(30, 3);
//     myBST.insert(70, 7);
//     myBST.insert(20, 2);
//     myBST.insert(40, 4);

//     cout << "Inorder traversal before deletion:" << endl;
//     myBST.inorder();

//     myBST.remove(30);

//     cout << "\nInorder traversal after deletion:" << endl;
//     myBST.inorder();
//     cout<<myBST.search(50);

//     return 0;
// }
