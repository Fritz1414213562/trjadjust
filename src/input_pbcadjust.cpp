#include "input_pbcadjust.hpp"
#include <boost/program_options.hpp>

namespace boost_po = boost::program_options;

Input4PBCAdjust::Input4PBCAdjust(
	const boost::program_options::variables_map& vm,
	const boost::program_options::options_description& desc)
{
	if (vm.count("help") || !vm.count("traj") || !vm.count("topo") || !vm.count("output"))
	{
		std::cerr << desc << std::endl;
		std::exit(1);
	}
	this->input_traj_name_        = vm["traj"].as<std::string>();
	this->topology_name_          = vm["topo"].as<std::string>();
	this->output_traj_name_       = vm["output"].as<std::string>();
	this->delta_t_                = vm["delta"].as<float>();
	this->step_interval_          = vm["step"].as<std::size_t>();
	this->is_moved_to_center_     = vm.count("center");
}
