#pragma once
#include <string>

/*
* @brief Abstract interface for a LLM text-genration client.
* 
* This class defines a clean, backend-agnostic API for genrating text from a prompt.
* It allows applications to use any LLM provider (Ollama, OpenAI, Local models, custom inference server, etc.)
* without changing the rest of the RAG pipeline.
* 
* 
* Derive from llmcient to implement a specific backend, e.g.:
* 1- OllamaLLMClient
* 2- OpenAIClient
* 3- LocalGGUFClient
* 
* This interface is used by the RAG "answer genrator" statge to turn retrieved chunks into a natual-langue
* response.
* 
*/

class llm_client
{
public:
	virtual ~llm_client() {}

	/*
	* @brief Generate text from a prompt using the LLM backend.
	* 
	* @param prompt The input prompt or instruction for the LLM.
	* @param max_tokens Maximum number of tokens to genrate.
	* 
	* @return A string containing the model's generated output.
	* 
	* @NOTE: This function is pure virual. Classes implementing llmclient must provide the 
	* backend-specific logic (HTTP request, local inference call, streaming, etc.)
	*/
	virtual std::string generate(const std::string& model,const std::string& prompt, int max_tokens) = 0;
};

