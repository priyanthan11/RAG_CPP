#pragma once
#include <string>
#include <nlohmann/json.hpp>


class config
{
    std::string embed_endpoint;
    std::string llm_endpoint;
    int chunk_size = 500; // characters
    int chunck_overlap = 100;
    int top_k = 8;
    int max_context_tokens = 2048;

    
public:
    static config from_json_file(const std::string& path);
    int getChunk_size() const { return chunk_size; }
    inline int getChunk_overlap() const { return chunck_overlap; }
    inline std::string getEmbed_endpoint() const { return embed_endpoint; }
    inline int getTop_k() const { return top_k; }
    inline std::string getLlm_endpoint() const { return llm_endpoint; }
    inline int getMax_context_tokens() const { return max_context_tokens; }
};
