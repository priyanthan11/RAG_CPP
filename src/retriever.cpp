#include "retriever.h"
#include "util.h"

retriever::retriever(std::shared_ptr<embedding_client> emb, std::shared_ptr<vector_store> vs, int top_k):
	emb_(emb),
	vs_(vs),
	top_k_(top_k)
{
}

std::vector<VectorRecord> retriever::retrieve(const std::string& query)
{
	auto qvec = emb_->embed_text(query);
	util u;
	u.normalize_inplace(qvec);
	auto scored = vs_->search(qvec, top_k_);
	std::vector<VectorRecord> out;
	for (auto& p : scored)
		out.push_back(p.first);

	return out;
}
