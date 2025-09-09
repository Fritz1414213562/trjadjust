#include "topology.hpp"


Topology::Topology(const Topology::range_type& residue_range) : residue_range_(residue_range)
{
	this->atom_num_ = 0;
	for (const auto& begin_end : this->residue_range_)
	{
		atom_num_ += (begin_end.second - begin_end.first) + 1;
	}
}
