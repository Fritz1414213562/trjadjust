#include "dcd_writer.hpp"
#include "src/util/utility.hpp"

void DCDWriter::dump(const std::vector<Coordinate>& trajectory)
{
	std::ofstream ofs(file_name_, std::ios::binary | std::ios::out);
	write_1stblock(ofs, trajectory.size());
	write_2ndblock(ofs);
	write_3rdblock(ofs, trajectory[0].atom_num());
	for (const auto& coordinate : trajectory)
	{
		write_frame(ofs, coordinate);
	}
	ofs.close();
}

void DCDWriter::write_1stblock(std::ofstream& ofs, const std::size_t total_frame)
{
	const std::int32_t block_size(84);
	Utility::write_as_bytes(ofs, block_size);
	
	ofs.write("CORD", 4);
	
	const std::int32_t total_frame_i32(total_frame);
	Utility::write_as_bytes(ofs, total_frame_i32);
	
	const std::int32_t index_of_first(0);
	Utility::write_as_bytes(ofs, index_of_first);
	
	const std::int32_t save_interval_i32(step_interval_);
	Utility::write_as_bytes(ofs, save_interval_i32);
	
	const std::int32_t total_step_i32(total_frame * step_interval_);
	Utility::write_as_bytes(ofs, total_step_i32);
	
	const std::int32_t total_chains(0);
	Utility::write_as_bytes(ofs, total_chains);
	
	const std::int32_t zero(0);
	// 4 * integers with null flag
	for(std::size_t i=0; i<4; ++i) {Utility::write_as_bytes(ofs, zero);}
	
	Utility::write_as_bytes(ofs, delta_t_);
	
	const std::int32_t has_unitcell = use_periodic_;
	Utility::write_as_bytes(ofs, has_unitcell);
	
	// 8 * integers with null flag
	for(std::size_t i=0; i<8; ++i) {Utility::write_as_bytes(ofs, zero);}
	
	const std::int32_t version(24);
	Utility::write_as_bytes(ofs, version);
	
	Utility::write_as_bytes(ofs, block_size);
}

void DCDWriter::write_2ndblock(std::ofstream& ofs)
{
	const std::int32_t block_size(84);
	Utility::write_as_bytes(ofs, block_size);
	const std::int32_t number_of_lines(1);
	Utility::write_as_bytes(ofs, number_of_lines);
	const char comment[80] = "converted by trjadjust -- by Fritz Nagae 2025";
	ofs.write(comment, 80);
	Utility::write_as_bytes(ofs, block_size);
}

void DCDWriter::write_3rdblock(std::ofstream& ofs, const std::size_t atom_num)
{
	const std::int32_t block_size(4);
	Utility::write_as_bytes(ofs, block_size);
	const std::int32_t atom_num_i32(atom_num);
	Utility::write_as_bytes(ofs, atom_num_i32);
	Utility::write_as_bytes(ofs, block_size);
}

void DCDWriter::write_unitcell(std::ofstream& ofs,
	const std::array<double, 3>& pbc_length, const std::array<double, 3>& pbc_angle)
{
	const std::int32_t block_size(sizeof(double) * 6);
	Utility::write_as_bytes(ofs, block_size);
	Utility::write_as_bytes(ofs, pbc_length[0]);
	Utility::write_as_bytes(ofs, pbc_angle [2]);
	Utility::write_as_bytes(ofs, pbc_length[1]);
	Utility::write_as_bytes(ofs, pbc_angle [1]);
	Utility::write_as_bytes(ofs, pbc_angle [0]);
	Utility::write_as_bytes(ofs, pbc_length[2]);
	Utility::write_as_bytes(ofs, block_size);
}

void DCDWriter::write_frame(std::ofstream& ofs, const Coordinate& coordinate)
{
	if (use_periodic_) { write_unitcell(ofs, coordinate.pbc_length(), coordinate.pbc_angle()); }
	const std::int32_t block_size(sizeof(float) * coordinate.atom_num());
	Utility::write_as_bytes(ofs, block_size);
	ofs.write(reinterpret_cast<const char*>(coordinate.xyz()[0].data()), block_size);
	Utility::write_as_bytes(ofs, block_size);
	Utility::write_as_bytes(ofs, block_size);
	ofs.write(reinterpret_cast<const char*>(coordinate.xyz()[1].data()), block_size);
	Utility::write_as_bytes(ofs, block_size);
	Utility::write_as_bytes(ofs, block_size);
	ofs.write(reinterpret_cast<const char*>(coordinate.xyz()[2].data()), block_size);
	Utility::write_as_bytes(ofs, block_size);
}
