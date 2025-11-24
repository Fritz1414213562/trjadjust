#ifndef TRJADJUST_PBCADJUSTER_HPP
#define TRJADJUST_PBCADJUSTER_HPP

#include "src/coordinate.hpp"
#include "src/topology.hpp"
#include <vector>
#include <array>

class PBCAdjuster
{
	using range_type = std::vector<std::pair<std::size_t, std::size_t>>;
	public:
		PBCAdjuster()  = default;
		~PBCAdjuster() = default;

		Coordinate run(const Coordinate& coordinate, const Topology& topology, const bool is_centered) const;
	private:
		float calculate_shift_number(const float& center, const float& wall) const;
};

#endif // TRJADJUST_ADJUSTER_HPP
