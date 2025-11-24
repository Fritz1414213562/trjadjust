#include "src/io/dcd_parser.hpp"
#include "coordinate.hpp"
#include "src/io/topology_parser.hpp"
#include "topology.hpp"
#include "src/io/replica_record_parser.hpp"
#include "pbcadjuster.hpp"
#include "src/io/dcd_writer.hpp"
#include "input_pbcadjust.hpp"
#include "input_remdextract.hpp"

#include <vector>
#include <utility>
#include <chrono>
#include <sstream>

#include <boost/timer/progress_display.hpp>
#include <boost/program_options.hpp>

namespace boost_po = boost::program_options;

int main(int argc, char* argv[])
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	boost_po::options_description global("Allowed options");
	global.add_options()
		("command", boost_po::value<std::string>(), "subcommand")
		("subargs", boost_po::value<std::vector<std::string>>(), "Arguments for subcommand")
		("help, h", "produce help message");
	boost_po::variables_map vm;
	boost_po::positional_options_description pos;
	pos.add("command", 1).add("subargs", -1);
	boost_po::parsed_options parsed = boost_po::command_line_parser(argc, argv).
			options(global).
			positional(pos).
			allow_unregistered().
			run();
	try
	{
		boost_po::store(parsed, vm);
	}
	catch (const boost_po::error_with_option_name& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << global   << std::endl;
		std::exit(1);
	}

	std::string subcmd = vm["command"].as<std::string>();
	if (subcmd == "pbc")
	{
		boost_po::options_description desc("pbc options");
		desc.add_options()
			("traj, j"  , boost_po::value<std::string>(), "path to an input trajectory (.dcd)")
			("topo, p"  , boost_po::value<std::string>(), "path to an input topology (.toml)")
			("output, o", boost_po::value<std::string>(), "path to an output trajectory")
			("delta, d",  boost_po::value<float>()->default_value(0.3), "time increment")
			("step, s",   boost_po::value<std::size_t>()->default_value(10000), "interval for recording")
			("center, c", "Move molecules specified in topology file onto a center")
			("help, h", "produce help message");
		std::vector<std::string> opts = boost_po::collect_unrecognized(parsed.options, boost_po::include_positional);
		opts.erase(opts.begin());
		try
		{
			boost_po::store(boost_po::command_line_parser(opts).options(desc).run(), vm);
		}
		catch (const boost_po::error_with_option_name& e)
		{
			std::cerr << e.what() << std::endl;
			std::exit(1);
		}
		boost_po::notify(vm);
		Input4PBCAdjust input = Input4PBCAdjust(vm, desc);
	
		// Step 1. Read trajectory
		DCDParser dcd_parser = DCDParser();
		const std::vector<Coordinate>& trajectory = dcd_parser.read(input.input_traj_name());
	
		// Step 2. Read topology
		TopologyParser top_parser = TopologyParser();
		const Topology& topology  = top_parser.read(input.topology_name());
	
		// Step 3. Move molecules under periodic boundary condition
		PBCAdjuster adjuster = PBCAdjuster();
		std::vector<Coordinate> moved_traj;
		boost::timer::progress_display progress(trajectory.size());
		for (const Coordinate& coordinate : trajectory)
		{
			moved_traj.push_back(adjuster.run(coordinate, topology, input.is_moved_to_center()));
			++progress;
		}
	
		// Step 4. Dump adjusted trajectory
		DCDWriter dcd_writer = DCDWriter(
			input.output_traj_name(), input.step_interval(), input.delta_t(), true);
		dcd_writer.dump(moved_traj);
	
	}
	else if (subcmd == "remd")
	{
		boost_po::options_description desc("remd options");
		desc.add_options()
			("input, i"  , boost_po::value<std::vector<std::string>>()->multitoken(), "path to input replica trajectories (.dcd)")
			("rep, r"  ,   boost_po::value<std::string>(), "path to an input replica-recording file (.rep)")
			("output, o",  boost_po::value<std::string>(), "prefix of path to output trajectories")
			("help, h", "produce help message");
		std::vector<std::string> opts = boost_po::collect_unrecognized(parsed.options, boost_po::include_positional);
		opts.erase(opts.begin());
		try
		{
			boost_po::store(boost_po::command_line_parser(opts).options(desc).run(), vm);
		}
		catch (const boost_po::error_with_option_name& e)
		{
			std::cerr << e.what() << std::endl;
			std::exit(1);
		}
		boost_po::notify(vm);
		Input4REMDExtract input = Input4REMDExtract(vm, desc);

		// Step 1. Read trajectory
		DCDParser dcd_parser = DCDParser();
		std::vector<std::vector<Coordinate>> replicas;
		bool is_periodic = false;
		for (const auto& fname : input.input_traj_names())
		{
			const std::vector<Coordinate>& traj = dcd_parser.read(fname);
			is_periodic = (is_periodic or dcd_parser.has_unitcell());
			replicas.push_back(traj);
		}
	
		// Step 2. Read replica-exchanging records
		ReplicaRecordParser rep_parser = ReplicaRecordParser();
		const std::vector<std::vector<std::size_t>>& replica_table  = rep_parser.read(input.replica_record_name());
	
		// Step 3. Extract snapshot in each replica
		boost::timer::progress_display progress(replica_table.size());
		for (std::size_t irep = 0; irep < replica_table.size(); ++irep)
		{
			std::vector<Coordinate> replica;
			const std::vector<std::size_t>& replica_id_series = replica_table[irep];
			std::size_t iframe = 0;
			for (const std::size_t& replica_id : replica_id_series)
			{
				replica.push_back(replicas[replica_id][iframe]);
				++iframe;
			}
			std::stringstream output_name;
			output_name << input.output_traj_name() << std::setfill('0') << std::setw(4) << irep + 1 << ".dcd";
			DCDWriter dcd_writer = DCDWriter(output_name.str(), 10000, 0.3, is_periodic);
			dcd_writer.dump(replica);
			++progress;
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Elapsed Time : ";
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
	std::cout << " [sec]" << std::endl;

	return 0;
}
