#include "embedding_client.h"
#include "embedding_http.h"  // Make sure this includes HttpEmbeddingClient
#include <iostream>
#include <memory>
#include <vector>

int main() {
    try {
        std::string url = "http://127.0.0.1:5000/embed";

        // Instantiate the concrete derived class
        std::unique_ptr<embedding_client> client = std::make_unique<embedding_http>(url);

        std::string text = "Hello world! This is a test.";
        std::vector<float> embedding = client->embed_text(text);

        std::cout << "Embedding size: " << embedding.size() << "\n";
        for (size_t i = 0; i < std::min(static_cast<size_t>(10), embedding.size()); ++i)
            std::cout << embedding[i] << " ";
        std::cout << "\n";

        std::cout << "Press ENTER to exit...";
        std::cin.get(); // keeps console open
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        std::cin.get();
    }
}
