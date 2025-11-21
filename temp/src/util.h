#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

class util
{
public:
    /*
     * compare cosine similarity between two embedding vectors.
     *
     * Cosine similarity = (A.B) / (||A|| * ||B||)
     *
     * This measures the angle between two vectors and is widely used in
     * NLP/IR systems to compare embeddings (e.g. document vs query).
     *
     * @param a First embedding vector
     * @param b Second embedding vector
     * @return Cosine similarity value in [-1,1]. Returns 0.0 on error.
     */
    inline double cosine_similarity(const std::vector<float>& a, const std::vector<float>& b);

    /*
     * Normalize a vector to unit length (L2-normalization)
     *
     * v <- v / ||V||
     *
     *  This improves retrieval relevance and ensures cosine similarity
     *  behaves as intended (pure angular distance)
     *
     * @param v Embedding vector to be normalized in-place
     *
     */
    inline void normalize_inplace(std::vector<float>& v);

};
