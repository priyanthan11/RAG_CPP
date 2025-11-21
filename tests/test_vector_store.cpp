// #include <iostream>
// #include <vector>
// #include "../src/vector_store.h"
// #include "../src/util.h"  // for normalize_inplace and cosine_similarity if needed

// int main() {
//     vector_store store;

//     // Add VectorRecords
//     store.add(VectorRecord{"id1", {1.0f, 0.0f, 0.0f}, Chunk{"", "Hello world", "", 0, 11, {}}});
//     store.add(VectorRecord{"id2", {0.0f, 1.0f, 0.0f}, Chunk{"", "Quick brown fox", "", 0, 15, {}}});
//     store.add(VectorRecord{"id3", {0.0f, 0.0f, 1.0f}, Chunk{"", "Lazy dog", "", 0, 8, {}}});

//     std::cout << "Added 3 documents.\n";

//     // Query vector
//     std::vector<float> query = {0.9f, 0.1f, 0.0f};
//     auto results = store.search(query, 2);

//     std::cout << "\nSearch results (top 2):\n";
//     for (auto& r : results) {
//         std::cout << "Score: " << r.second << "\n";
//         std::cout << "Text:  " << r.first.chunk.text << "\n";
//         std::cout << "-------------------\n";
//     }

//     // Test correctness
//     if (!results.empty() && results[0].first.chunk.text == "Hello world") {
//         std::cout << "\nVectorStore tests passed!\n";
//     } else {
//         std::cout << "\nVectorStore tests FAILED!\n";
//     }

//     return 0;
// }
