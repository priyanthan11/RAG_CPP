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
    static inline double cosine_similarity(const std::vector<float>& a,
        const std::vector<float>& b)
    {
        // Mismatched vector size -> undefined similarity
        if (a.size() != b.size()) return 0.0f;

        // Accumulators
        double dot_product = 0.0; // A. A
        double magnitude_a_sq = 0.0; // ||A|| ^2
        double magnitude_b_sq = 0.0; // ||B|| ^2

        // Compute dot product and squared magnitudes.
        for (size_t i = 0; i < a.size(); i++)
        {
            dot_product += static_cast<double>(a[i]) * b[i];
            magnitude_a_sq += static_cast<double>(a[i]) * a[i];
            magnitude_b_sq += static_cast<double>(b[i]) * b[i];
        }

        // Avoid division by zero.
        if (magnitude_a_sq == 0.0 || magnitude_b_sq == 0.0) return 0.0;

        return dot_product / (std::sqrt(magnitude_a_sq) * std::sqrt(magnitude_b_sq));
    }
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
    static inline void normalize_inplace(std::vector<float>& v)
    {
        double sum_squares = 0.0;
        // Compute squared magnitude.
        for (float x : v)
            sum_squares += static_cast<double>(x) * x;
        if (sum_squares <= 0.0)
            return; // cannot normalize zero-vector.
        double norm = std::sqrt(sum_squares);

        // Divide each component by the magnitude.
        for (float& x : v)
            x = static_cast<float>(static_cast<double>(x) / norm);
    }

};
