#include "rag_engine.h"
#include <sstream>
#include <iostream>

rag_engine::rag_engine(std::shared_ptr<retriever> retriever, std::shared_ptr<llm_http> llm, int max_context_tokens):
	retriever_(retriever),
	llm_(llm),
	max_context_tokens_(max_context_tokens)
{
}

std::string rag_engine::answer(const std::string& questions)
{
	auto ctx = retriever_->retrieve(questions);
	std::cout << "[SYSTEM] Successfully Retreived \n";
	std::string prompt = assemble_prompt(questions, ctx);
	const std::string& model = "llama3.2";
	int max_tokens = 512;
	return llm_->generate(model,prompt, max_tokens);
}

std::string rag_engine::assemble_prompt(const std::string& q, const std::vector<VectorRecord>& ctx)
{
	std::ostringstream ss;
	ss << "SYSTEM:\n"
        "You are an OpenAI internal assistant. Use ONLY the provided context to answer.\n"
        "If the answer is not found in the context, reply:\n"
        "\"I don't know based on the provided information.\"\n"
        "You MUST cite using the format: [source:start-end].\n"
		"Do not use external knowledge.\n\n";
	ss << "CONTEXT:\n";

	// Context Chunks
	int tokenEstimate = 0;
	for (auto& r : ctx)
	{
		// Simple token budgeting by character count
		int estTokens = (int)r.chunk.text.size() / 4;
		if (tokenEstimate + estTokens > max_context_tokens_) break;
		
		/*ss << "[COURCE: " << r.chunk.source << " START: " << r.chunk.start_char << " END:" << r.chunk.end_char << "]\n";
		ss << r.chunk.text << "\n\n";*/

		ss << "=========== CHUNK ============";
		ss << "SOURCE: " << r.chunk.source << "\n";
		ss << "RANGE: " << r.chunk.start_char << "-" << r.chunk.end_char << "\n";
		ss << "TEXT: \n" << r.chunk.text << "\n";
		ss << "==============================\n\n";



		tokenEstimate += estTokens;
	}
	// ==== QUESTION
	ss << "QUESTION: " << q << "\n\n";
	// ==== ANSWER
	ss << "ANSWER: \n";
	return ss.str();
}
