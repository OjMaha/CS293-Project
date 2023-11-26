#include <iostream>
#include <vector>

using namespace std;

template <typename K, typename V>
class CustomMap {
public:
    class Node {
    public:
        K key;
        V value;
        int height;
        Node* left;
        Node* right;

        Node(K k, V v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;
    int n;

    void insert(K key, V value) {
        root = insertUtil(root, key, value);
    }

    void remove(K key) {
        root = removeUtil(root, key);
    }

    Node* search(K key) {
        return searchUtil(root, key);
    }

    bool contains(K key) {
        return (searchUtil(root, key) != nullptr);
    }

    V get(K key) {
        Node* result = searchUtil(root, key);
        return (result != nullptr) ? result->value : V(); // Return default value if key is not found
    }

    void set(K key, V value) {
        Node* existingNode = searchUtil(root, key);
        if (existingNode != nullptr) {
            // Update the value if the key is already present
            existingNode->value = value;
        } else {
            // Insert a new key-value pair if the key is not present
            root = insertUtil(root, key, value);
        }
    }


    void inorder() {
        inorderUtil(root);
        cout << endl;
    }

    std::vector<K> getAllKeys() {
        std::vector<K> keys;
        getAllKeysUtil(root, keys);
        return keys;
    }
    

private:
    int height(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    Node* rightRotation(Node* node) {
        Node* newhead = node->left;
        node->left = newhead->right;
        newhead->right = node;
        node->height = 1 + max(height(node->left), height(node->right));
        newhead->height = 1 + max(height(newhead->left), height(newhead->right));
        return newhead;
    }

    Node* leftRotation(Node* node) {
        Node* newhead = node->right;
        node->right = newhead->left;
        newhead->left = node;
        node->height = 1 + max(height(node->left), height(node->right));
        newhead->height = 1 + max(height(newhead->left), height(newhead->right));
        return newhead;
    }

    void inorderUtil(Node* node) {
        if (node == nullptr)
            return;
        inorderUtil(node->left);
        cout << "(" << node->key << ", " << node->value << ") ";
        inorderUtil(node->right);
    }

    Node* insertUtil(Node* node, K key, V value) {
        if (node == nullptr) {
            n += 1;
            Node* temp = new Node(key, value);
            return temp;
        }

        if (key < node->key)
            node->left = insertUtil(node->left, key, value);
        else if (key > node->key)
            node->right = insertUtil(node->right, key, value);

        node->height = 1 + max(height(node->left), height(node->right));
        int bal = height(node->left) - height(node->right);

        if (bal > 1) {
            if (key < node->left->key)
                return rightRotation(node);
            else {
                node->left = leftRotation(node->left);
                return rightRotation(node);
            }
        } else if (bal < -1) {
            if (key > node->right->key)
                return leftRotation(node);
            else {
                node->right = rightRotation(node->right);
                return leftRotation(node);
            }
        }
        return node;
    }

    Node* removeUtil(Node* node, K key) {
        if (node == nullptr)
            return nullptr;

        if (key < node->key) {
            node->left = removeUtil(node->left, key);
        } else if (key > node->key) {
            node->right = removeUtil(node->right, key);
        } else {
            Node* r = node->right;
            if (node->right == nullptr) {
                Node* l = node->left;
                delete (node);
                node = l;
            } else if (node->left == nullptr) {
                delete (node);
                node = r;
            } else {
                while (r->left != nullptr)
                    r = r->left;
                node->key = r->key;
                node->value = r->value;
                node->right = removeUtil(node->right, r->key);
            }
        }

        if (node == nullptr)
            return node;

        node->height = 1 + max(height(node->left), height(node->right));
        int bal = height(node->left) - height(node->right);

        if (bal > 1) {
            if (height(node->left) >= height(node->right))
                return rightRotation(node);
            else {
                node->left = leftRotation(node->left);
                return rightRotation(node);
            }
        } else if (bal < -1) {
            if (height(node->right) >= height(node->left))
                return leftRotation(node);
            else {
                node->right = rightRotation(node->right);
                return leftRotation(node);
            }
        }
        return node;
    }

    Node* searchUtil(Node* node, K key) {
        if (node == nullptr)
            return nullptr;
        K k = node->key;
        if (k == key)
            return node;
        if (k > key)
            return searchUtil(node->left, key);
        if (k < key)
            return searchUtil(node->right, key);
    }

    void getAllKeysUtil(Node* node, std::vector<K>& keys) {
        if (node == nullptr)
            return;

        getAllKeysUtil(node->left, keys);
        keys.push_back(node->key);
        getAllKeysUtil(node->right, keys);
    }
};

// int main() {
//     CustomMap<float, string> customMap;
//     customMap.insert(1.3, "One point three");
//     customMap.insert(2.4, "Two point four");
//     customMap.insert(3.5, "Three point five");

//     // Get all keys and print them
//     std::vector<float> keys = customMap.getAllKeys();
//     cout << "All Keys: ";
//     for (const auto& key : keys) {
//         cout << key << " ";
//     }
//     cout << endl;

//     // ... (rest of your main function)
//     return 0;
// }
