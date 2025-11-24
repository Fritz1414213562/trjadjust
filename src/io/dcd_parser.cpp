#include "dcd_parser.hpp"
#include "src/util/utility.hpp"


std::vector<Coordinate> DCDParser::read(const std::string& filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
		throw std::runtime_error("[error] File or directory not found: " + filename);

	// 1st block
	const std::string& first_block = read_block(ifs);
	const int frame_num = read_frame_num(first_block);
	has_unitcell_ = read_unitcell_flag(first_block);
	// 2nd block (not used)
	read_block(ifs);
	// 3rd block
	const std::string& third_block = read_block(ifs);
	const int atom_num  = read_atom_num(third_block);

	std::vector<Coordinate> traj;

	for (std::size_t iframe = 0; iframe < frame_num; ++iframe)
	{
		traj.push_back(Coordinate(read_xyz(ifs, atom_num)));
	}
	ifs.close();

	return traj;
}

std::string DCDParser::read_block(std::ifstream& ifs)
{
	std::int32_t block_size;
	std::vector<char> buffer;

	constexpr int int32_size = sizeof(int32_t);
	ifs.read(reinterpret_cast<char*>(&block_size), int32_size);
	buffer.resize(block_size);
	ifs.read(buffer.data(), block_size);

	std::int32_t check_block_size;
	ifs.read(reinterpret_cast<char*>(&check_block_size), int32_size);

	if (block_size != check_block_size)
		throw std::runtime_error(
			"[error] Inconsistent block size:"
				+ std::to_string(block_size)
				+ " != " + std::to_string(check_block_size));
	return std::string(buffer.begin(), buffer.end());
}

Coordinate DCDParser::read_xyz(
	std::ifstream& ifs, const int atom_num)
{
	std::string pbc_block;
	if (has_unitcell_) pbc_block = read_block(ifs);
	const std::string&  x_block = read_block(ifs);
	const std::string&  y_block = read_block(ifs);
	const std::string&  z_block = read_block(ifs);
	std::array<std::vector<float>, 3> xyz = {
		read_coordinates(x_block, atom_num),
		read_coordinates(y_block, atom_num),
		read_coordinates(z_block, atom_num)};
	if (has_unitcell_)
	{
		const DCDParser::pbc_box_type& pbc_param = read_pbc_box(pbc_block);
		return Coordinate(xyz, pbc_param.first, pbc_param.second);
	}
	else return Coordinate(xyz);
}

int DCDParser::read_frame_num(const std::string& first_block)
{
	return Utility::read_binary_as<int>(&first_block.at(frame_num_index_));
}

int DCDParser::read_atom_num(const std::string& third_block)
{
	return Utility::read_binary_as<int>(&third_block.at(atom_num_index_));
}

std::vector<float> DCDParser::read_coordinates(const std::string& block, const int atom_num) const
{
	std::vector<float> coordinates(atom_num);
	constexpr int float_size = sizeof(float);
	for (std::size_t iatom = 0; iatom < atom_num; ++iatom)
	{
		std::size_t pos_in_block = iatom * float_size;
		coordinates[iatom] = Utility::read_binary_as<float>(&block.at(pos_in_block));
	}
	return coordinates;
}

DCDParser::pbc_box_type DCDParser::read_pbc_box(const std::string& block) const
{
	constexpr int double_size = sizeof(double);
	const double     X = Utility::read_binary_as<double>(&block.at(0 * double_size));
	const double gamma = Utility::read_binary_as<double>(&block.at(1 * double_size));
	const double     Y = Utility::read_binary_as<double>(&block.at(2 * double_size));
	const double  beta = Utility::read_binary_as<double>(&block.at(3 * double_size));
	const double alpha = Utility::read_binary_as<double>(&block.at(4 * double_size));
	const double     Z = Utility::read_binary_as<double>(&block.at(5 * double_size));
	return std::make_pair(std::array<double, 3>{X, Y, Z}, std::array<double, 3>{alpha, beta, gamma});
}

bool DCDParser::read_unitcell_flag(const std::string& first_block)
{
	return static_cast<bool>(Utility::read_binary_as<int>(&first_block.at(unit_cell_index_)));
}

