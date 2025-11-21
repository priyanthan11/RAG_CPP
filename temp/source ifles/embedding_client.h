#pragma once
#include <vector>
#include <string>

/*
* @brief Abstract base class for text embedding clients
* 
* Embedding Client defines a generic interface for genrating vector embeddings
* from input text. Subclass can implement different methods (local mode, HTTP API, etc..)
*/

class embedding_client
{
public:
	virtual ~embedding_client() {}
	/*
	* @brif Generate a vector embedding for a given text string.
	* @param text Input text to embed.
	* @return std::vector<float> the embedding vector represenatation of the text.
	* 
	*/
	virtual std::vector<float> embed_text(const std::string& text)=0;
};

