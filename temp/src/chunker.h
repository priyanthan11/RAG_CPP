#pragma once
#include <string>
#include <vector>
#include <nlohman/json.hpp>

struct Chunk
{
	std::string id; // unique ID of the chunk (generated via stable hash)
	std::string text; // the actual text content of the chunk
	std::string source; // The source document name or path;
	size_t start_char; // Start the character index of the chunk in the orighnal document.
	size_t end_char; // end character index of the chunk in the original document;
	nlohmann::json meta; // Metadata about the chunk (source,start,end,etc)
};

/*
* @brief Splits text documents into overlapping chunks suitable for RAG/enbedding pipelines.
* 
* DocumentChunker allows you to specify the chunk size and overlap between chunks.
* Each chunk has a stable ID, source info,text,and metadata for further processing.
*/

class chunker
{
	/*
	* @brief Construct a new DocumentChunker object
	* 
	* @param chunk_size Maximum number of characters per chunk (default 500)
	* @param overlap Number of characters to overlap between consecutive chunks (default 100)
	*/
	Chunker(int chunk_size = 500, int overlap = 100);
	

	/*
	* @brief: Split a text string into chunks.
	* 
	* @param text The document to split.
	* @param source The source identifier (e.g, filename) used for chunk IDs and metadata.
	* @return std::vector<chunk> List of generated chunks.
	*/
	std::vector<Chunk> chunk_text(const std::string& text, const std::string& source);

private:
	int chunk_size_; // Maximum chunk size in characters.
	int overlap_; // Overlap between consecutive chunks in characters.

	/*
	* @brief Generate a stable unique ID for a chunk
	* 
	* Combines the source string and start index, then hases it to produce a string ID.
	* 
	* @param source Source doucment indentifier.
	* @param start Start chracter index of the chunk.
	* @return std::string Unique chunk ID
	*/
	std::string make_id(const std::string& source, size_t start);
};

