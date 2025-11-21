#include "rag_engine.h"
#include <sstream>

rag_engine::rag_engine(std::shared_ptr<retriever> retriever, std::shared_ptr<llm_client> llm, int max_context_tokens):
	retriever_(retriever),
	llm_(llm),
	max_context_tokens_(max_context_tokens)
{
}

std::string rag_engine::answer(const std::string& questions)
{
	auto ctx = retriever_->retrieve(questions);
	std::string prompt = assemble_prompt(questions, ctx);
	int max_tokens = 512;
	return llm_->generate(prompt, max_tokens);
}

std::string rag_engine::assemble_prompt(const std::string& q, const std::vector<VectorRecord>& ctx)
{
	std::ostringstream ss;
	ss << "You are a helpful assistant. Use the following context to answer the question. If context is insufficient say 'I don't know'.\n\n";
	int tokenEstimate = 0;
	for (auto& r : ctx)
	{
		// Simple token budgeting by character count
		int estTokens = (int)r.chunk.text.size() / 4;
		if (tokenEstimate + estTokens > max_context_tokens_) break;
		ss << "[COURCE: " << r.chunk.source << " START: " << r.chunk.start_char << " END:" << r.chunk.end_char << "]\n";
		ss << r.chunk.text << "\n\n";
		tokenEstimate += estTokens;
	}
	ss << "QUESTION: " << q << "\n\nAnswer: ";
	return ss.str();
}
