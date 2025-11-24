#include "ollama_embeddings.h"

std::vector<float> ollama_embeddings::embed_text(const std::string& text)
{
	// Prompt lama to geenrate a JSON array as embedding
	std::ostringstream prompt;

	prompt << "Provide a " << dim_
		<< "-dimensional embedding vector for the text: \n\""
		<< text
		<< "\"\n"
		<< "Return only a JASON array of numbers, e.g. [0.1,0.2, ...]";
	const std::string& model = "llama3.2";
	std::string output = llm_->generate(model,prompt.str(), 1000);
	// Parse JSON array into vector<float>
	try
	{
		auto j = nlohmann::json::parse(output);
		if (!j.is_array()) throw std::runtime_error("Invalid embedding JSON");
		std::vector<float> vec;
		for (auto& v : j) vec.push_back(v.get<float>());
		return vec;
	}
	catch (const std::exception&e)
	{
		throw std::runtime_error(std::string("Failed to parse embedding: ") + e.what() + "\nOutput: " + output);
	}


	return std::vector<float>();
}
