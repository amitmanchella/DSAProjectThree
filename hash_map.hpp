#pragma once
#include <sstream>
#include <exception>
#include <vector>
#include <memory>
namespace cs251 {

// Custom exception classes
    class duplicate_key : public std::runtime_error {
    public: duplicate_key() : std::runtime_error("Duplicate key!") {} };
    class nonexistent_key : public std::runtime_error {
    public: nonexistent_key() : std::runtime_error("Key does not exist!") {} };

    template <typename K, typename V>
    class hash_map {
    public:
        class hash_map_node {
        public:
            // The key of current node.
            K m_key = {};
            // The value of current node.
            std::unique_ptr<V> m_value{};
        };

        // Return a constant reference to the hash table vector
        const std::vector<std::shared_ptr<hash_map_node>>& get_data() const;

        // Default constructor - create a hash map with an initial capacity of 1
        hash_map();
        // Constructor - create a hash map with an initial capacity of bucketCount
        hash_map(size_t bucketCount);

        // Get the hash code for a given key
        size_t hash_code(K key) const;

        // Change the size of the table to bucketCount, re-hashing all existing elements
        // bucketCount will never be 0 or less than the current number of elements
        void resize(size_t bucketCount);

        // Insert the key/value pair into the table, if the key doesn't already exist
        // Throw duplicate_key if the key already exists
        void insert(const K& key, std::unique_ptr<V> value);
        // Return a const reference to the value associated with the given key
        // Throw nonexistent_key if the key is not in the hash table
        const std::unique_ptr<V>& peek(const K& key);
        // Remove and return the key-value pair associated with the given key
        // Throw nonexistent_key if the key is not in the hash table
        std::unique_ptr<V> extract(const K& key);

        // Return the current number of elements in the hash table
        size_t size() const;
        // Return the current capacity of the hash table
        size_t bucket_count() const;
        // Return whether the hash table is currently empty
        bool empty() const;

    private:
        // The array that holds key-value pairs
        std::vector<std::shared_ptr<hash_map_node>> m_data = {};

        // TODO: Add any additional methods or variables here
    };

    template <typename K, typename V>
    const std::vector<std::shared_ptr<typename hash_map<K,V>::hash_map_node>>& hash_map<K,V>::get_data() const {
        return m_data;
    }

    template <typename K, typename V>
    hash_map<K,V>::hash_map() {
        m_data.resize(1);
    }

    template <typename K, typename V>
    hash_map<K,V>::hash_map(const size_t bucketCount) {
        m_data.resize(bucketCount);
    }

    template <typename K, typename V>
    size_t hash_map<K,V>::hash_code(K key) const {
        size_t hash = key % bucket_count();
        return hash;
    }



/*
template <typename K, typename V>
void hash_map<K,V>::resize(const size_t bucketCount) {
    if (bucket_count() > bucketCount) {
        return;
    }

    std::vector<std::shared_ptr<hash_map_node>> new_data(bucketCount);
    m_data.resize(bucketCount);

    for (const auto& node : m_data) {
        if (node) {
            size_t newIndex = hash_code(node->m_key);
            while (new_data[newIndex] != nullptr) {
                newIndex = (newIndex + 1) % bucketCount;
            }
            new_data[newIndex] = node;
        }
    }
    m_data.swap(new_data);
}*/



    template <typename K, typename V>
    void hash_map<K,V>::resize(const size_t bucketCount) {
        std::vector<std::shared_ptr<hash_map_node>> temp = m_data;
        m_data.clear();
        m_data.resize(bucketCount, nullptr);
        for (const auto& node : temp) {
            if (node) {
                size_t newIndex = hash_code(node->m_key);
                while (m_data[newIndex] != nullptr) {
                    newIndex = (newIndex + 1) % bucketCount;
                }
                m_data[newIndex] = node;
            }
        }
    }


    template <typename K, typename V>
    void hash_map<K,V>::insert(const K& key, std::unique_ptr<V> value) {
        size_t index = hash_code(key);
        size_t start_index = index;
        for (size_t i = 0; i < bucket_count(); i++) {
            if (m_data[i] != nullptr) {
                if (m_data[i]->m_key == key) {
                    throw duplicate_key();
                }
            }
        }
        size_t bucketCounter = bucket_count();
        for (size_t h = 0; h < bucketCounter; h++) {
            if (m_data[index] == nullptr) {
                m_data[index] = std::make_shared<hash_map_node>();
                m_data[index]->m_key = key;
                m_data[index]->m_value = std::move(value);
                return;
            }
            else {
                index = (index + 1) % bucket_count();
                if (index == start_index) {
                    size_t resizing = bucket_count() * 2;
                    resize(resizing);
                    insert(key, std::move(value));
                }
            }
        }
    }


    template <typename K, typename V>
    const std::unique_ptr<V>& hash_map<K,V>::peek(const K& key) {
        size_t index = hash_code(key);
        size_t start_index = index;
        /*if (m_data[index] == nullptr) {
            index = (index + 1) % bucket_count();
        }*/
        for (size_t i = 0; i < bucket_count(); i++) {
            if (m_data[index] != nullptr) {
                if (m_data[index]->m_key == key) {
                    return m_data[index]->m_value;
                }
            }
            index = (index + 1) % bucket_count();
            if (index == start_index) {
                break;
            }
        }
        throw nonexistent_key();
    }

    template <typename K, typename V>
    std::unique_ptr<V> hash_map<K,V>::extract(const K& key) {
        size_t index = hash_code(key);
        size_t start_index = index;
        /*if (m_data[index] == nullptr) {
            index = (index + 1) % bucket_count();
        }*/
        for (size_t i = 0; i < bucket_count(); i++) {
            if (m_data[index] != nullptr) {
                if (m_data[index]->m_key == key) {
                    std::unique_ptr <V> returnValue = std::move(m_data[index]->m_value);
                    m_data[index] = nullptr;
                    return returnValue;
                }
            }
            index = (index + 1) % bucket_count();
            if (index == start_index) {
                throw nonexistent_key();
            }
        }

        throw nonexistent_key();
    }


    template <typename K, typename V>
    size_t hash_map<K,V>::size() const {
        size_t counter = 0; //does this need to be size_t
        for (const auto& node : m_data) {
            if (node != nullptr) {
                counter++;
            }
        }
        return counter;
    }

    template <typename K, typename V>
    size_t hash_map<K,V>::bucket_count() const {
        return m_data.size();
    }

    template <typename K, typename V>
    bool hash_map<K,V>::empty() const {
        if (size() == 0) {
            return true;
        }
        return false;
    }

}
