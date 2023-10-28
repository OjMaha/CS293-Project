#include <iostream>
#include <vector>

/* CustomMap implementation
    1. void insert(const K& key, const V& value)
    2. void set(const K& key, const V& value)
    3. V get(const K& key)
    4. void remove(const K& key)
    5. bool contains(const K& key)
    6. size_t getSize()
*/


template <typename K, typename V>
struct KeyValuePair {
    K key;
    V value;
};

template <typename K, typename V>
class CustomMap {
private:
    std::vector<std::vector<KeyValuePair<K, V>>> table;
    size_t size;

    // Custom hash function for strings
    size_t hash(const K& key) {
        size_t hash = 0;
        for (char c : key) {
            hash = (hash * 31) + c; // A basic hash function for strings
        }
        return hash % table.size();
    }

public:
    CustomMap(size_t tableSize = 100) : table(tableSize), size(0) {}

    // Insert a key-value pair into the map
    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        for (auto& pair : table[index]) {
            if (pair.key == key) {
                pair.value = value; // Update existing key
                return;
            }
        }
        table[index].push_back({key, value}); // Insert a new key
        size++;
    }

    // Set a key-value pair in the map (checks if the key exists, updates if it does, inserts if it doesn't)
    void set(const K& key, const V& value) {
        size_t index = hash(key);
        for (auto& pair : table[index]) {
            if (pair.key == key) {
                pair.value = value; // Update existing key
                return;
            }
        }
        table[index].push_back({key, value}); // Insert a new key
        size++;
    }

    // Retrieve the value associated with a key
    V get(const K& key) {
        size_t index = hash(key);
        for (const auto& pair : table[index]) {
            if (pair.key == key) {
                return pair.value;
            }
        }
        throw std::out_of_range("Key not found");
    }

    // Remove a key-value pair from the map
    void remove(const K& key) {
        size_t index = hash(key);
        auto& keyList = table[index];
        for (auto it = keyList.begin(); it != keyList.end(); ++it) {
            if (it->key == key) {
                keyList.erase(it);
                size--;
                return;
            }
        }
    }

    // Check if a key exists in the map
    bool contains(const K& key) {
        size_t index = hash(key);
        for (const auto& pair : table[index]) {
            if (pair.key == key) {
                return true;
            }
        }
        return false;
    }

    // Get the number of key-value pairs in the map
    size_t getSize() {
        return size;
    }
};


/*int main() {
    CustomMap<std::string, int> myMap;

    myMap.insert("one", 1);
    myMap.insert("two", 2);
    myMap.insert("three", 3);

    std::cout << "Size of map: " << myMap.getSize() << std::endl;

    std::cout << "Value associated with 'two': " << myMap.get("two") << std::endl;

    myMap.remove("two");
    std::cout << "Size of map after removing 'two': " << myMap.getSize() << std::endl;

    std::cout << "Does 'two' exist in the map? " << (myMap.contains("two") ? "Yes" : "No") << std::endl;

    return 0;
}*/
