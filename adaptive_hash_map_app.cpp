#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include "app.hpp"
#include "adaptive_hash_map.hpp"
using namespace cs251;

/*
* This code is provided to be built as an executable for grading.
* You can modify the code based on your needs, but the original copy of this
* file will be used for testing.
*/
template <typename K, typename V> void run_test();
template <typename K, typename V> void print_table(const adaptive_hash_map<K,V>& hm);
template <typename K, typename V>
void print_tree(std::shared_ptr<typename splay_tree<K,V>::splay_tree_node> node,
		std::string prefix = "", std::string child_prefix = "");

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

	adaptive_hash_map<K,V> hm = ([&]() {
		if (initial_capacity == 1)
			return adaptive_hash_map<K,V>();
		else
			return adaptive_hash_map<K,V>(initial_capacity); })();

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

			} else if (command == "quit") {
				std::cout << command << std::endl;

				break;
			}
		} catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

template <typename K, typename V> void print_table(const adaptive_hash_map<K,V>& hm) {
	const auto& data = hm.get_data();

	for (size_t i = 0; i < data.size(); i++) {
		std::stringstream ss;
		ss << std::setw(3) << i << ": ";
		std::string prefix = ss.str();
		std::string child_prefix = "     ";

		const auto& tree = data[i];
		if (tree.empty()) {
			std::cout << prefix << "[empty]" << std::endl;
			continue;
		}

		print_tree<K,V>(tree.get_root(), prefix, child_prefix);
	}
}

template <typename K, typename V>
void print_tree(std::shared_ptr<typename splay_tree<K,V>::splay_tree_node> node,
		std::string prefix, std::string child_prefix) {
	if (!node) return;

	std::cout << prefix << node->m_key << " -> " << *node->m_value << std::endl;
	if (node->m_left && !node->m_right) {
		print_tree<K,V>(node->m_left, child_prefix + "└L: ", child_prefix + " ");
	} else if (node->m_left && node->m_right) {
		print_tree<K,V>(node->m_left, child_prefix + "├L: ", child_prefix + "│");
	}
	if (node->m_right) {
		print_tree<K,V>(node->m_right, child_prefix + "└R: ", child_prefix + " ");
	}
}
