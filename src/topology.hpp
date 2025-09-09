#ifndef TRJADJUST_TOPOLOGY_HPP
#define TRJADJUST_TOPOLOGY_HPP

#include <vector>
#include <utility>

struct Topology
{
	using range_type = std::vector<std::pair<std::size_t, std::size_t>>;
	public:

		Topology(const range_type& residue_range);

		~Topology() = default;

		range_type  residue_range() const { return residue_range_; }
		range_type& residue_range()       { return residue_range_; }

		std::size_t  atom_num()     const { return atom_num_; }
		std::size_t& atom_num()           { return atom_num_; }

	private:
		range_type  residue_range_;
		std::size_t atom_num_;
};

#endif // TRJADJUST_TOPOLOGY_HPP
