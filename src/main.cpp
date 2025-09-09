#include "input.hpp"
#include "src/io/dcd_parser.hpp"
#include "coordinate.hpp"
#include "src/io/topology_parser.hpp"
#include "topology.hpp"
#include "adjuster.hpp"
#include "src/io/dcd_writer.hpp"

#include <vector>
#include <utility>
#include <chrono>

#include <boost/timer/progress_display.hpp>

int main(int argc, char* argv[])
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	Input input = Input(argc, argv);

	// Step 1. Read trajectory
	DCDParser dcd_parser = DCDParser();
	const std::vector<Coordinate>& trajectory = dcd_parser.read(input.input_traj_name());

	// Step 2. Read topology
	TopologyParser top_parser = TopologyParser();
	const Topology& topology  = top_parser.read(input.topology_name());

	// Step 3. Move molecules under periodic boundary condition
	Adjuster adjuster = Adjuster();
	std::vector<Coordinate> moved_traj;
	boost::timer::progress_display progress(trajectory.size());
	for (const Coordinate& coordinate : trajectory)
	{
		moved_traj.push_back(adjuster.run(coordinate, topology));
		++progress;
	}

	// Step 4. Dump adjusted trajectory
	DCDWriter dcd_writer = DCDWriter(
		input.output_traj_name(), input.step_interval(), input.delta_t(), true);
	dcd_writer.dump(moved_traj);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Elapsed Time : ";
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
	std::cout << " [sec]" << std::endl;

	return 0;
}
