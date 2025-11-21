#include "comfig.h"
#include "comfig.h"
#include <fstream>

comfig comfig::from_json_file(const std::string& path)
{
	std::ifstream ifs(path);
	nlohmann::json j;
	ifs >> j;
	comfig c;

	if (j.contains("embed_endpoint")) c.embed_endpoint = j["embed_endpoint"].get<std::string>();
	if (j.contains("llm_endpoint")) c.llm_endpoint = j["llm_endpoint"].get<std::string>();
	if (j.contains("chunk_size")) c.chunck_size = j["chunk_size"].get<int>();
	if (j.contains("chunk_overlap")) c.chunck_overlap = j["chunk_overlap"].get<int>();
	if (j.contains("top_k")) c.top_k = j["top_k"].get<int>();
	if (j.contains("max_context_token")) c.max_context_tokens = j["max_context_token"].get<int>();

	return c;
}
