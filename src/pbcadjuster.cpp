#include "pbcadjuster.hpp"
#include <iostream>

Coordinate PBCAdjuster::run(const Coordinate& coordinate, const Topology& topology, const bool is_centered) const
{
	if (coordinate.atom_num() != topology.atom_num())
		throw std::runtime_error(
			"[error] The system size of trajectory is inconsistent with that of topology");
	const std::array<double, 3>& pbc = coordinate.pbc_length();
	const std::array<std::vector<float>, 3>& xyz = coordinate.xyz();
	std::array<std::vector<float>, 3> moved_xyz;
	const std::array<float, 3>& box_center = {
		static_cast<float>(0.5 * pbc[0]),
		static_cast<float>(0.5 * pbc[1]),
		static_cast<float>(0.5 * pbc[2])};
	for (const auto& begin_end : topology.residue_range())
	{
		std::array<float, 3> center = {0.0, 0.0, 0.0};
		const float chain_size_f32 = static_cast<float>(begin_end.second - begin_end.first + 1);
		for (std::size_t iatom = begin_end.first; iatom <= begin_end.second; ++iatom)
		{
			for (std::size_t idim = 0; idim < 3; ++idim)
				center[idim] += xyz[idim][iatom] / chain_size_f32;
		}
		if (is_centered) for (std::size_t idim = 0; idim < 3; ++idim) center[idim] += box_center[idim];
		const float x_shift_num = calculate_shift_number(center[0], static_cast<float>(pbc[0]));
		const float y_shift_num = calculate_shift_number(center[1], static_cast<float>(pbc[1]));
		const float z_shift_num = calculate_shift_number(center[2], static_cast<float>(pbc[2]));
		for (std::size_t iatom = begin_end.first; iatom <= begin_end.second; ++iatom)
		{
			if (is_centered)
			{
				moved_xyz[0].push_back(xyz[0][iatom] + box_center[0] - x_shift_num * pbc[0]);
				moved_xyz[1].push_back(xyz[1][iatom] + box_center[1] - y_shift_num * pbc[1]);
				moved_xyz[2].push_back(xyz[2][iatom] + box_center[2] - z_shift_num * pbc[2]);
			}
			else
			{
				moved_xyz[0].push_back(xyz[0][iatom] - x_shift_num * pbc[0]);
				moved_xyz[1].push_back(xyz[1][iatom] - y_shift_num * pbc[1]);
				moved_xyz[2].push_back(xyz[2][iatom] - z_shift_num * pbc[2]);
			}
		}
	}
	return Coordinate(moved_xyz, pbc, coordinate.pbc_angle());
}

float PBCAdjuster::calculate_shift_number(const float& center, const float& wall) const
{
	if (center < 0.0) return calculate_shift_number(center + wall, wall) - 1.0;
	else if (center > wall) return calculate_shift_number(center - wall, wall) + 1.0;
	else return 0.0;
}
