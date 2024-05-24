#pragma once
#include <iostream>

// Custom name class
class name {
public:
	std::string m_first;
	std::string m_last;
	name() {}
	name(std::string first, std::string last) : m_first(first), m_last(last) {}

	// Operator overloads
	bool operator==(const name& other) const;
	bool operator!=(const name& other) const;
	bool operator<(const name& other) const;
	bool operator>(const name& other) const;
	bool operator<=(const name& other) const;
	bool operator>=(const name& other) const;
	size_t operator%(size_t m) const;
};

// Hash function - combines hashes of underlying strings
namespace std {
template <> struct hash<name> {
	size_t operator()(const name& n) const noexcept {
		size_t h1 = std::hash<std::string>{}(n.m_first);
		size_t h2 = std::hash<std::string>{}(n.m_last);
		return h1 ^ (h2 << 1);
	}
};
}

// Comparison operators
bool name::operator==(const name& other) const {
	return m_first == other.m_first && m_last == other.m_last;
}
bool name::operator!=(const name& other) const {
	return !(*this == other);
}
// Sort by last name first
bool name::operator<(const name& other) const {
	if (m_last == other.m_last)
		return m_first < other.m_first;
	else
		return m_last < other.m_last;
}
bool name::operator>(const name& other) const {
	return other < *this;
}
bool name::operator<=(const name& other) const {
	return !(*this > other);
}
bool name::operator>=(const name& other) const {
	return !(*this < other);
}

// Modulus operator
size_t name::operator%(size_t m) const {
	return std::hash<name>{}(*this) % m;
}

// Output redirection operator
std::ostream& operator<<(std::ostream& ostr, const name& n) {
	return ostr << n.m_first << " " << n.m_last;
}

// Stream extraction operator
std::istream& operator>>(std::istream& istr, name& n) {
	return istr >> n.m_first >> n.m_last;
}

// Modulus between std::string and size_t
size_t operator%(const std::string& s, size_t m) {
	return std::hash<std::string>{}(s) % m;
}
