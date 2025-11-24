#ifndef TRJADJUST_UTILITY_HPP
#define TRJADJUST_UTILITY_HPP
#include <random>
#include <cstring>
#include <fstream>
#include <array>

namespace Utility
{
template<typename T>
T read_binary_as(const char *str)
{
	T result;
	std::memcpy(std::addressof(result), str, sizeof(T));
	return result;
}

template<typename T>
void write_as_bytes(std::ofstream& os, const T& v) noexcept
{
	using Type = typename std::remove_reference<T>::type;
	os.write(reinterpret_cast<const char*>(std::addressof(v)), sizeof(Type));
	return;
}

template<typename T>
T inner_product(const std::array<T, 3>& lhs, const std::array<T, 3>& rhs)
{
	return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

template<typename T>
std::array<T, 3> cross_product(const std::array<T, 3>& lhs, const std::array<T, 3>& rhs)
{
	return std::array<T, 3>({
		lhs[1] * rhs[2] - lhs[2] * rhs[1],
		lhs[2] * rhs[0] - lhs[0] * rhs[2],
		lhs[0] * rhs[1] - lhs[1] * rhs[0],
	});
}

bool startswith(const std::string& s, const std::string& prefix);

std::vector<std::string> split_string(const std::string& s, const char delimiter);

bool is_blank_line(const std::string& s);

}


#endif // TRJADJUST_UTILITY_HPP
