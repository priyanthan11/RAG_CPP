#pragma once
#include "llm_client.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

/*
* @class llm_http
* @brief concrete implementation of llm_client using HTTP (via libcurl).
*
* This class sends JSON POST request to a remore LLM inference server and returns the genrated text.
* It is intentionally backend-agnositc:
* any server that accepts:
*	{
*		"prompt": "<text>"
*		"max_tokens": N
*	}
*
* and returns:
*
*	{"text": "<generated_output>"}
*
* or:
*
*	{"outputs": ["<generated_output>"]}
*
* is compatible with this client.
*
* Typical usage:
*
* @code:
*
* llm_http llm("http://localhost:11434/api/generate");
* std::string out = llm.generate("Tell me a joke",64);
*
* @endcode
*
* This design allows the RAG pipeline to swtich LLM providers without modifying higher-level code.
* llm_http can be replaced with a specilized backed like openAIClient or LocalGGUFClient.
*
*/




class llm_http : public llm_client
{
public:
	llm_http(const std::string& url);
	
	virtual std::string generate(const std::string& prompt, int max_tokens) override;

private:
	std::string url_;
};

