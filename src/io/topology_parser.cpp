#include "topology_parser.hpp"

Topology TopologyParser::read(const std::string& filename) const
{
	const toml::value& data = toml::parse(filename);
	const auto& topology    = toml::find(data, "topology");
	const auto& ranges      = toml::find<toml::array>(topology, "ranges");
	TopologyParser::range_type retval;
	for (std::size_t idx = 0; idx < ranges.size(); ++idx)
	{
		const auto& range = ranges.at(idx);
		if (!range.contains("begin"))
			throw std::runtime_error("The key 'begin' is not found.");
		else if (!range.contains("end"))
			throw std::runtime_error("The key 'end' is not found.");

		const std::size_t begin = toml::get<std::size_t>(range.at("begin"));
		const std::size_t end   = toml::get<std::size_t>(range.at("end"));
		retval.push_back(std::make_pair(begin, end));
	}
	return Topology(retval);
}
