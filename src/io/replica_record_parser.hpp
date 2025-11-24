#ifndef TRJADJUST_REPLICA_RECORD_PARSER_HPP
#define TRJADJUST_REPLICA_RECORD_PARSER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

class ReplicaRecordParser
{
	public:
		ReplicaRecordParser()  = default;
		~ReplicaRecordParser() = default;
		std::vector<std::vector<std::size_t>> read(const std::string& filename) const;
	private:
};

#endif // TRJADJUST_REPLICA_RECORD_PARSER_HPP
