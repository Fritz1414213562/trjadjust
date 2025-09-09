#ifndef TRAJ2ENERGY_COORDINATE_HPP
#define TRAJ2ENERGY_COORDINATE_HPP

#include <vector>
#include <array>

class Coordinate
{
	public:
		Coordinate(const std::array<std::vector<float>, 3>& xyz);
		Coordinate(
			const std::array<std::vector<float>, 3>& xyz,
			const std::array<double, 3>& pbc_length,
			const std::array<double, 3>& pbc_angle);
		std::array<std::vector<float>, 3>  xyz() const   {return xyz_;}
		std::array<std::vector<float>, 3>& xyz()         {return xyz_;}
		std::size_t                   atom_num() const   {return atom_num_;}
		std::size_t&                  atom_num()         {return atom_num_;}
		std::array<double, 3>         pbc_length() const {return pbc_length_;}
		std::array<double, 3>&        pbc_length()       {return pbc_length_;}
		std::array<double, 3>         pbc_angle() const  {return pbc_angle_;}
		std::array<double, 3>&        pbc_angle()        {return pbc_angle_;}
	
		float distance(const std::size_t idx, const std::size_t jdx) const;
		float angle(const std::size_t idx, const std::size_t jdx, const std::size_t kdx) const;
		float dihedral(const std::size_t idx, const std::size_t jdx,
					   const std::size_t kdx, const std::size_t ldx) const;

	private:
		std::size_t atom_num_;
		std::array<std::vector<float>, 3> xyz_;
		std::array<double, 3> pbc_length_;
		std::array<double, 3> pbc_angle_;
};

#endif // TRAJ2ENERGY_COORDINATE_HPP
