// #include "embedding_http.h"
// #include <iostream>
// #include <memory>
// #include <vector>
// #include <cassert>

// int main() {
//     try {
//         std::string url = "http://127.0.0.1:5000/embed";
//         auto client = std::make_unique<embedding_http>(url);

//         std::vector<std::string> test_texts = {
//             "Hello world",
//             "The quick brown fox jumps over the lazy dog",
//             "RAG embedding test"
//         };

//         for (auto& text : test_texts) {
//             std::vector<float> embedding = client->embed_text(text);
//             std::cout << "Text: " << text << "\n";
//             std::cout << "Embedding size: " << embedding.size() << "\n";
//             assert(!embedding.empty()); // simple sanity check
//             for (auto& val : embedding) std::cout << val << " ";
//             std::cout << "\n\n";
//         }

//         std::cout << "All embedding tests passed!" << std::endl;
//         std::cin.get(); // keep console open
//     }
//     catch (const std::exception& e) {
//         std::cerr << "Test failed: " << e.what() << std::endl;
//         std::cin.get();
//     }
// }
