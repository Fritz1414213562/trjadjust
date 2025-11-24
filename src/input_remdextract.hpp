#ifndef TRJADJUST_INPUT_REMDExtract_HPP
#define TRJADJUST_INPUT_REMDExtract_HPP

#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <array>

class Input4REMDExtract
{
	public:
		Input4REMDExtract(const boost::program_options::variables_map& vm,
						  const boost::program_options::options_description& desc);
		std::vector<std::string> input_traj_names() const { return input_traj_names_; }
		std::string output_traj_name()              const { return output_traj_name_; }
		std::string replica_record_name()           const { return replica_record_name_; }
	private:
		std::vector<std::string> input_traj_names_;
		std::string output_traj_name_;
		std::string replica_record_name_;
};

#endif // TRJADJUST_INPUT_REMDExtract_HPP
