#include "retriever.h"
#include "util.h"
#include <iostream>

retriever::retriever(std::shared_ptr<embedding_http> emb, std::shared_ptr<vector_store> vs, int top_k):
	emb_(emb),
	vs_(vs),
	top_k_(top_k)
{
}

std::vector<VectorRecord> retriever::retrieve(const std::string& query)
{
	std::vector<VectorRecord> out;
	auto qvec = emb_->embed_text(query);
	std::cout << "[SYSTEM] Successfully embeded text\n";
	util u;
	u.normalize_inplace(qvec);
	std::cout << "[SYSTEM] Successfully Normalized the text\n";
	auto scored = vs_->search(qvec, top_k_);
	std::cout << "[SYSTEM] Successfully searched the cosine (matching word) and the socre is: " << scored.size() << " \n";
	for (auto& p : scored)
	{
		std::cout << p.first.id << std::endl;
		out.push_back(p.first);
	}
		

	return out;
}
