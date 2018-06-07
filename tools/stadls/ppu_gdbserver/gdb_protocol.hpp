#pragma once
#include <sstream>
#include <string>
#include <vector>

#define HEX_IN_BYTE 2

template <class T>
std::string to_hex(T t, size_t width)
{
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(HEX_IN_BYTE * width) << std::hex << t;
	return ss.str();
}

std::string strip_request(std::string s)
{
	// remove $, #checksum
	size_t const pos_r = s.find("#");
	s = s.substr(0, pos_r);
	return s.substr(1);
}

std::string make_response(std::string s)
{
	// calc checksum
	int sum = 0;
	for (auto const x : s) {
		sum += static_cast<uint8_t>(x);
	}

	// prepend $, append #checksum
	return "$" + s + "#" + to_hex(sum % 256, sizeof(char));
}

std::string erase_escape_sequences_from_binary_string(std::string s)
{
	std::vector<size_t> poss;
	size_t pos;
	std::string s1 = s;
	while ((pos = s.find(0x7d)) != std::string::npos) {
		s1.at(pos + 1) = s1.at(pos + 1) ^ 0x20;
		poss.push_back(pos);
		s.at(pos) = '0';
	}
	for (size_t i = 0; i < poss.size(); ++i) {
		s1.erase(poss[i], 1);
	}
	return s1;
}
