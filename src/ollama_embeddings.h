#pragma once
#include "embedding_client.h"
#include "llm_http.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>

class ollama_embeddings : public embedding_client
{
public:
	ollama_embeddings(const std::shared_ptr<llm_http>& llm, int dim = 512)
		: llm_(llm), dim_(dim) {}

	// Conver text into a numeric vector embedding
	std::vector<float> embed_text(const std::string& text) override;
private:
	std::shared_ptr<llm_http> llm_;
	int dim_;
};