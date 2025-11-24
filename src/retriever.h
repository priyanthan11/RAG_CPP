#pragma once
#include "embedding_http.h"
#include "vector_store.h"
#include "chunker.h"
#include <memory>

/*
* @class retriever
* @brief High-level component responsible for convering q query into an embedding and retriving 
*		- the most relevent VectorRecords.
* 
* This is part of the RAG pipeline. It performs three main operations:
*	1. Embeds the input texting using and embedding client.
*	2. Searches the vectorStore using the query embedding
*	3. Returns the top_k Most relevant VectorRecords
* 
* THis class abstracts away the low-levl details of:
*	- calling the embedding API
*	- performing cosine similarity search
*	- sorting and selecting top-k documents
* 
* by seperating this logic into a dedicated class, the system stays modular, testable, and easy to extend.
* 
*/

class retriever
{
public:
	/*
	* @param emb Shared embedding client used to compute query embeddings
	* @param vs Shared vector store containing document embeddings
	* @param top_k Number of search results to return
	*/
	retriever(std::shared_ptr<embedding_http> emb, std::shared_ptr<vector_store> vs, int top_k = 8);
	
	/*
	* @brief Retrieve the top_K most relevent VectorRecords for query string
	* 
	* Steps:
	*	1 - Compute embedding for the input text via embeddingClient
	*	2 - Pass the embedding to VesstorStore::search
	*	3 - Extract and return Only the VectorRecords (not the scores)
	*/
	std::vector<VectorRecord> retrieve(const std::string& query);

private:
	std::shared_ptr<embedding_http>emb_; // Generate Embeddings
	std::shared_ptr<vector_store> vs_; // Holds document vectors
	int top_k_; // Number of items to return
};

