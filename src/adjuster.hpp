#ifndef TRJADJUST_ADJUSTER_HPP
#define TRJADJUST_ADJUSTER_HPP

#include "src/coordinate.hpp"
#include "src/topology.hpp"
#include <vector>
#include <array>

class Adjuster
{
	using range_type = std::vector<std::pair<std::size_t, std::size_t>>;
	public:
		Adjuster()  = default;
		~Adjuster() = default;

		Coordinate run(const Coordinate& coordinate, const Topology& topology, const bool is_centered) const;
	private:
		float calculate_shift_number(const float& center, const float& wall) const;
};

#endif // TRJADJUST_ADJUSTER_HPP
