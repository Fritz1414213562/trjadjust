#include "input.hpp"
#include <boost/program_options.hpp>

namespace boost_po = boost::program_options;

Input::Input(int argc, char* argv[])
{
	boost_po::options_description desc("Allowed options");
	desc.add_options()
		("traj, j"  , boost_po::value<std::string>(), "path to an input trajectory (.dcd)")
		("topo, p"  , boost_po::value<std::string>(), "path to an input topology (.toml)")
		("output, o", boost_po::value<std::string>(), "path to an output trajectory")
		("delta, d",  boost_po::value<float>()->default_value(0.3), "time increment")
		("step, s",   boost_po::value<std::size_t>()->default_value(10000), "interval for recording")
		("center, c", "Move molecules specified in topology file onto a center")
		("help, h", "produce help message");
	boost_po::variables_map vm;
	try
	{
		boost_po::store(boost_po::parse_command_line(argc, argv, desc), vm);
	}
	catch (const boost_po::error_with_option_name& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(1);
	}
	boost_po::notify(vm);
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
