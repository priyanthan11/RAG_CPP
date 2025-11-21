#include "util.h"

//double util::cosine_similarity(const std::vector<float>& a, const std::vector<float>& b)
//{
//    // Mismatched vector size -> undefined similarity
//    if (a.size() != b.size()) return 0.0f;
//
//    // Accumulators
//    double dot_product = 0.0; // A. A
//    double magnitude_a_sq = 0.0; // ||A|| ^2
//    double magnitude_b_sq = 0.0; // ||B|| ^2
//
//    // Compute dot product and squared magnitudes.
//    for (size_t i = 0; i < a.size(); i++)
//    {
//        dot_product += static_cast<double>(a[i]) * b[i];
//        magnitude_a_sq += static_cast<double>(a[i]) * a[i];
//        magnitude_b_sq += static_cast<double>(b[i]) * b[i];
//    }
//
//    // Avoid division by zero.
//    if (magnitude_a_sq == 0.0 || magnitude_b_sq == 0.0) return 0.0;
//
//    return dot_product / (std::sqrt(magnitude_a_sq) * std::sqrt(magnitude_b_sq));
//}
//
//void util::normalize_inplace(std::vector<float>& v)
//{
//    double sum_squares = 0.0;
//    // Compute squared magnitude.
//    for (float x : v)
//        sum_squares += static_cast<double>(x) * x;
//    if (sum_squares <= 0.0)
//        return; // cannot normalize zero-vector.
//    double norm = std::sqrt(sum_squares);
//
//    // Divide each component by the magnitude.
//    for (float& x : v)
//        x = static_cast<float>(static_cast<double>(x) / norm);
//}
