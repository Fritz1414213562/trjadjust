#ifndef TRJADJUST_TOPOLOGY_PARSER_HPP
#define TRJADJUST_TOPOLOGY_PARSER_HPP

#include "src/topology.hpp"

#include <toml.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <utility>

class TopologyParser
{
	using range_type = std::vector<std::pair<std::size_t, std::size_t>>;
	public:
		TopologyParser()  = default;
		~TopologyParser() = default;

		Topology read(const std::string& filename) const;

	private:
};

#endif // TRJADJUST_TOPOLOGY_PARSER_HPP
