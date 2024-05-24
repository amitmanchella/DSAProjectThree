#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include "app.hpp"
#include "hash_map.hpp"
using namespace cs251;

/*
* This code is provided to be built as an executable for grading.
* You can modify the code based on your needs, but the original copy of this
* file will be used for testing.
*/
template <typename K, typename V> void run_test();
template <typename K, typename V> void print_table(const hash_map<K,V>& hm);

int main() {
	try {
		std::string key_type, value_type;
		std::cin >> key_type >> value_type;
		if (key_type == "int") {
			if (value_type == "int")
				run_test<int,int>();
			else if (value_type == "float")
				run_test<int,float>();
			else if (value_type == "string")
				run_test<int,std::string>();
			else if (value_type == "name")
				run_test<int,name>();
		} else if (key_type == "string") {
			if (value_type == "int")
				run_test<std::string,int>();
			else if (value_type == "float")
				run_test<std::string,float>();
			else if (value_type == "string")
				run_test<std::string,std::string>();
			else if (value_type == "name")
				run_test<std::string,name>();
		} else if (key_type == "name") {
			if (value_type == "int")
				run_test<name,int>();
			else if (value_type == "float")
				run_test<name,float>();
			else if (value_type == "string")
				run_test<name,std::string>();
			else if (value_type == "name")
				run_test<name,name>();
		}
	} catch (const std::exception& e) {
		std::cerr << "Unhandled exception: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

template <typename K, typename V> void run_test() {
	// Read the initial capacity
	size_t initial_capacity;
	std::cin >> initial_capacity;

	hash_map<K,V> hm = ([&]() {
		if (initial_capacity == 1)
			return hash_map<K,V>();
		else
			return hash_map<K,V>(initial_capacity); })();

	// Read each command and execute until quit
	while (true) {
		std::string command;
		std::cin >> command;
		try {
			if (command == "insert") {
				K key;
				std::unique_ptr<V> value = std::make_unique<V>();
				std::cin >> key >> *value;
				std::cout << command << " " << key << " " << *value << std::endl;

				hm.insert(key, std::move(value));

			} else if (command == "peek") {
				K key;
				std::cin >> key;
				std::cout << command << " " << key << std::endl;

				const auto& value = hm.peek(key);
				std::cout << *value << std::endl;

			} else if (command == "extract") {
				K key;
				std::cin >> key;
				std::cout << command << " " << key << std::endl;

				auto value = hm.extract(key);
				std::cout << *value << std::endl;

			} else if (command == "size") {
				std::cout << command << std::endl;

				size_t size = hm.size();
				std::cout << size << std::endl;

			} else if (command == "empty") {
				std::cout << command << std::endl;

				bool empty = hm.empty();
				std::cout << (empty ? "true" : "false") << std::endl;

			} else if (command == "print") {
				std::cout << command << std::endl;

				print_table<K,V>(hm);

			} else if (command == "hash_code") {
				K key;
				std::cin >> key;
				std::cout << command << " " << key << std::endl;

				size_t hash = hm.hash_code(key);
				std::cout << hash << std::endl;

			} else if (command == "bucket_count") {
				std::cout << command << std::endl;

				size_t buckets = hm.bucket_count();
				std::cout << buckets << std::endl;

			} else if (command == "resize") {
				size_t capacity;
				std::cin >> capacity;
				std::cout << command << " " << capacity << std::endl;

				hm.resize(capacity);

			} else if (command == "quit") {
				std::cout << command << std::endl;

				break;
			}
		} catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

template <typename K, typename V> void print_table(const hash_map<K,V>& hm) {
	const auto& data = hm.get_data();

	for (size_t i = 0; i < data.size(); i++) {
		std::cout << std::setw(3) << i << ": ";

		auto node_p = data[i];
		if (!node_p) {
			std::cout << "[empty]" << std::endl;
			continue;
		}

		if (!node_p->m_value) {
			std::stringstream ss;
			ss << "Error in hash table index " << i << ", key "
				<< node_p->m_key << ": value is null!";
			throw std::runtime_error(ss.str());
		}

		std::cout << "(" << hm.hash_code(node_p->m_key) << ") ";
		std::cout << node_p->m_key << " -> " << *node_p->m_value;
		std::cout << std::endl;
	}
}
