#include "replica_record_parser.hpp"
#include <src/util/utility.hpp>
#include <iostream>

std::vector<std::vector<std::size_t>> ReplicaRecordParser::read(const std::string& filename) const
{
	bool is_skipped = true;
	std::ifstream ifs(filename, std::ios::out);
	std::string line;
	std::vector<std::vector<std::size_t>> table;
	std::size_t n_replica = 0;
	while (std::getline(ifs, line))
	{
		if (Utility::startswith(line, "# History of replica exchange"))
		{
			is_skipped = false;
			continue;
		}
		if (is_skipped) continue;
		else if (Utility::is_blank_line(line)) break;
		else
		{
			const std::vector<std::string>& tokens = Utility::split_string(line, ' ');
			std::vector<std::size_t> replica_ids;
			for (std::size_t idx = 1; idx < tokens.size(); ++idx)
			{
				try {
					replica_ids.push_back(std::stoi(tokens[idx]));
				}
				catch (const std::invalid_argument& e)
				{
					std::cerr << "The .rep file might be broken. The conversion of '" << tokens[idx] << "' was failed." << std::endl;
					std::exit(1);
				}
			}
			table.push_back(replica_ids);
			n_replica = replica_ids.size();
		}
	}
	ifs.close();

	std::vector<std::vector<std::size_t>> retval(n_replica);
	for (const auto& replica_ids : table)
	{
		for (std::size_t irep = 0; irep < n_replica; ++irep)
		{
			const auto& it = std::find(replica_ids.begin(), replica_ids.end(), irep + 1);
			if (it == replica_ids.end())
				throw std::runtime_error("[error] Replica id " + std::to_string(irep + 1) + " is out of range");
			const std::size_t index = std::distance(replica_ids.begin(), it);
			retval[irep].push_back(index);
		}
	}
	return retval;
}
