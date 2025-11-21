#include "vector_store.h"
#include "util.h"
#include <algorithm>
#include <fstream>
void vector_store::add(const VectorRecord& rec)
{
	db_.push_back(rec);
}

std::vector<std::pair<VectorRecord, double>> vector_store::search(const std::vector<float>& query, int k)
{
	std::vector<std::pair<VectorRecord, double>> scored;
	std::vector<float> q = query;
	util u;
	u.normalize_inplace(q);

	for (auto& r : db_)
	{
		double s = u.cosine_similarity(q, r.vec);
		scored.push_back({ r,s });
	}

	std::sort(scored.begin(), scored.end(), [](auto& a, auto& b) {return a.second > b.second; });
	if ((int)scored.size() > k) scored.resize(k);


	return scored;
}

void vector_store::save_to_file(const std::string& path)
{
	nlohmann::json j;
	for (auto& r : db_)
	{
		j.push_back(
			{
				{"id",r.id},
				{"vec",r.vec},
				{"chunk",{"id",r.chunk.id},
				{"text",r.chunk.text},
				{"source",r.chunk.source},
				{"start",r.chunk.start_char},
				{"end",r.chunk.end_char} } 
			}
		);
	}
	std::ofstream ofs(path);
	ofs << j.dump(2);
}

void vector_store::load_from_file(const std::string& path)
{
	std::ifstream ifs(path);
	if (!ifs) return;
	nlohmann::json j; ifs >> j;
	db_.clear();
	for (auto& it : j)
	{
		VectorRecord r;
		r.id = it["id"].get<std::string>();
		r.vec = it["vec"].get<std::vector<float>>();
		Chunk c;
		c.id = it["chunk"]["id"].get<std::string>();
		c.text = it["chunk"]["text"].get<std::string>();
		c.source = it["chunk"]["source"].get<std::string>();
		c.start_char = it["chunk"]["start"].get<size_t>();
		c.end_char = it["chunk"]["end"].get<size_t>();
		r.chunk = c;
		db_.push_back(std::move(r));

	}
}
