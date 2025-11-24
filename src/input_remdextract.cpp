#include "input_remdextract.hpp"
#include <boost/program_options.hpp>

namespace boost_po = boost::program_options;

Input4REMDExtract::Input4REMDExtract(
	const boost::program_options::variables_map& vm,
	const boost::program_options::options_description& desc)
{
	if (vm.count("help") || !vm.count("input") || !vm.count("output") || !vm.count("rep"))
	{
		std::cerr << desc << std::endl;
		std::exit(1);
	}
	this->input_traj_names_    = vm["input"].as<std::vector<std::string>>();
	this->output_traj_name_    = vm["output"].as<std::string>();
	this->replica_record_name_ = vm["rep"].as<std::string>();
}
