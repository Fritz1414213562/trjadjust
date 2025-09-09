#include "utility.hpp"
#include <cstring>
#include <memory>
#include <random>
#include <unordered_map>
#include <stdexcept>

namespace Utility
{

bool startswith(const std::string& s, const std::string& prefix)
{
	if (s.size() < prefix.size()) return false;
	return std::equal(std::begin(prefix), std::end(prefix), std::begin(s));
}

std::vector<std::string> split_string(const std::string& s, const char delimiter)
{
	std::vector<std::string> retval;
	std::string buffer;
	for (const char& chr : s)
	{
		if ((chr == delimiter) && (buffer.empty())) continue;
		else if ((chr == delimiter) && (!buffer.empty()))
		{
			retval.push_back(buffer);
			buffer.clear();
		}
		else buffer.push_back(chr);
	}
	if (!buffer.empty()) retval.push_back(buffer);
	return retval;
}

}
