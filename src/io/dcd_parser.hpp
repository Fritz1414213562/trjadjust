#ifndef TRJADJUST_DCD_PARSER_HPP
#define TRJADJUST_DCD_PARSER_HPP

#include "src/coordinate.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <array>
#include <utility>

class DCDParser {

	using pbc_box_type = std::pair<std::array<double, 3>, std::array<double, 3>>;

	public:
		DCDParser()  = default;
		~DCDParser() = default;

		std::vector<Coordinate> read(const std::string& filename);
	private:
		std::string read_block(std::ifstream& ifs);
		Coordinate read_xyz(
			std::ifstream& ifs, const int atom_num, const bool has_unitcell);

		int read_frame_num(const std::string& first_block);
		int read_atom_num(const std::string& third_block);
		bool read_unitcell_flag(const std::string& first_block);
		std::vector<float> read_coordinates(const std::string& block, const int atom_num) const;
		pbc_box_type read_pbc_box(const std::string& block) const;

	private:
		const int frame_num_index_ = 4;
		const int unit_cell_index_ = 44;
		const int atom_num_index_  = 0;

};

#endif // TRJADJUST_DCD_PARSER_HPP
