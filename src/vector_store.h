#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "chunker.h"

struct VectorRecord
{
	std::string id;
	std::vector<float> vec; // Normalized
	Chunk chunk;

};

class vector_store
{
public:
	vector_store() {}
	void add(const VectorRecord& rec);
	// returns top k records sorted by core desc

	std::vector<std::pair<VectorRecord, double>> search(const std::vector<float>& query, int k);
	void save_to_file(const std::string& path);
	void load_from_file(const std::string& path);
	size_t size() const { return db_.size(); }
private:
	std::vector<VectorRecord> db_;
	
};

