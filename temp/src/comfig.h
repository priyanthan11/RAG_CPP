#pragma once
#include <string>
#include <nlohman/json.hpp>


class comfig
{
    std::string embed_endpoint;
    std::string llm_endpoint;
    int chunk_size = 500; // characters
    int chunck_overlap = 100;
    int top_k = 8;
    int max_context_tokens = 2048;

    
public:
    static comfig from_json_file(const std::string& path);
};
