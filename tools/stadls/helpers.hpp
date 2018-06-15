#include <cctype>
#include <fstream>
#include <iomanip>
#include <netinet/in.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/v2/ppu.h"

namespace helpers {

using namespace haldls::v2;
using namespace halco::hicann_dls::v2;

std::string print_PPUMemoryBlock_as_string(PPUMemoryBlock& block)
{
	std::stringstream ss;
	auto words = block.get_words();
	for (auto x : words) {
		uint32_t w = ntohl(static_cast<uint32_t>((x.get())));
		char* c = reinterpret_cast<char*>(&w);
		for (size_t i = 0; i < sizeof(uint32_t); ++i) {
			if (isprint(c[i]) or isspace(c[i])) {
				ss << c[i];
			}
		}
	}
	return ss.str();
}

PPUMemoryBlock load_PPUMemoryBlock_from_file(std::string filename)
{
	std::vector<char> program_bytes;
	// read file
	try {
		std::ifstream in;
		in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		in.open(filename, std::ios::binary);

		std::istreambuf_iterator iter(in);
		std::copy(iter, std::istreambuf_iterator<char>(), std::back_inserter(program_bytes));
	} catch (std::ifstream::failure e) {
		std::stringstream ss;
		ss << "Error reading from file " << filename << ": " << e.what() << ".";
		throw std::runtime_error(ss.str());
	}

	// pad to multiple of word size
	while ((program_bytes.size() % sizeof(uint32_t)) != 0) {
		program_bytes.push_back(0);
	}

	// convert to words
	uint32_t* iter = reinterpret_cast<uint32_t*>(program_bytes.data());
	std::vector<uint32_t> words(iter, iter + program_bytes.size() / sizeof(uint32_t));

	// correct endianness
	std::transform(words.begin(), words.end(), words.begin(), ntohl);

	// convert to PPUMemoryWords
	std::vector<PPUMemoryWord> ppu_memory_words(words.size());
	std::transform(words.begin(), words.end(), ppu_memory_words.begin(), [](uint32_t const x) {
		return PPUMemoryWord(PPUMemoryWord::Value(x));
	});

	// create and set PPUMemoryBlock
	auto size = PPUMemoryBlock::Size(ppu_memory_words.size());
	PPUMemoryBlock block(size);
	block.set_words(ppu_memory_words);

	return block;
}

} // namespace helpers
