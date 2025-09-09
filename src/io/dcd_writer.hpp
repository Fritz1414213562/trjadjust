#ifndef TRJADJUST_DCD_WRITER_HPP
#define TRJADJUST_DCD_WRITER_HPP

#include "src/coordinate.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <array>
#include <utility>

class DCDWriter
{
	public:
		DCDWriter(
			const std::string& file_name, const std::size_t step_interval,
			const float delta_t, const bool use_periodic)
			: file_name_(file_name), step_interval_(step_interval),
			  delta_t_(delta_t), use_periodic_(use_periodic) {}

		void dump(const std::vector<Coordinate>& trajectory);

	private:

		void write_1stblock(std::ofstream& ofs, const std::size_t total_step);
		void write_2ndblock(std::ofstream& ofs);
		void write_3rdblock(std::ofstream& ofs, const std::size_t atom_num);

		void write_unitcell(
			std::ofstream& ofs,
			const std::array<double, 3>& pbc_length,
			const std::array<double, 3>& pbc_angle);

		void write_frame(std::ofstream& ofs, const Coordinate& coordinate);

		std::string file_name_;
		std::size_t step_interval_;
		float       delta_t_;
		bool        use_periodic_;

};

#endif // TRJADJUST_DCD_WRITER_HPP
