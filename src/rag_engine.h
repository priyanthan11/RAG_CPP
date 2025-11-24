#pragma once
#include <memory>
#include <string>
#include "retriever.h"
#include "llm_http.h"

class rag_engine
{
public:
	rag_engine(std::shared_ptr<retriever> retriever, std::shared_ptr<llm_http> llm, int max_context_tokens);
	std::string answer(const std::string& questions);

private:
	std::shared_ptr<retriever> retriever_;
	std::shared_ptr<llm_http> llm_;
	
	int max_context_tokens_;
	std::string assemble_prompt(const std::string& q, const std::vector<VectorRecord>& ctx);
};