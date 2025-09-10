#ifndef TRJADJUST_INPUT_HPP
#define TRJADJUST_INPUT_HPP

#include <string>
#include <iostream>
#include <vector>
#include <array>

class Input
{
	public:
		Input(int argc, char* argv[]);
		std::string input_traj_name()    const { return input_traj_name_; }
		std::string output_traj_name()   const { return output_traj_name_; }
		std::string topology_name()      const { return topology_name_; }
		float       delta_t()            const { return delta_t_; }
		std::size_t step_interval()      const { return step_interval_; }
		bool        is_moved_to_center() const { return is_moved_to_center_; }
	private:
		std::string input_traj_name_;
		std::string output_traj_name_;
		std::string topology_name_;
		float       delta_t_;
		std::size_t step_interval_;
		bool        is_moved_to_center_;
};

#endif // TRJADJUST_INPUT_HPP
