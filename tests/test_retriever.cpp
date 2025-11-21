#include <iostream>
#include <memory>
#include <vector>

#include "../src/retriever.h"
#include "../src/vector_store.h"
#include "../src/embedding_client.h"
#include "../src/chunker.h"

// ---- Fake Embedding Client for testing ----
// Instead of calling Ollama / API, we simulate embeddings.
class FakeEmbeddingClient : public embedding_client {
public:
    // Return deterministic vectors based on input
    std::vector<float> embed_text(const std::string& text) override {
        if (text == "Hello world") return {1, 0, 0};
        if (text == "Quick brown fox") return {0, 1, 0};
        if (text == "Lazy dog") return {0, 0, 1};
        if (text == "hello") return {1, 0.1f, 0};
        return {0, 0, 0};
    }
};

int main() {
    // ---- Setup ----
    auto emb = std::make_shared<FakeEmbeddingClient>();
    auto vs  = std::make_shared<vector_store>();

    // Add test documents
    vs->add(VectorRecord{"doc1", {1,0,0}, Chunk{"Hello world"}});
    vs->add(VectorRecord{"doc2", {0,1,0}, Chunk{"Quick brown fox"}});
    vs->add(VectorRecord{"doc3", {0,0,1}, Chunk{"Lazy dog"}});

    retriever retriever(emb, vs, 2); // top_k = 2

    // ---- Run ----
    auto results = retriever.retrieve("hello");

    std::cout << "Retriever returned " << results.size() << " chunks.\n";
    for (auto& r : results) {
        std::cout << "ID: " << r.id << " | Text: " << r.chunk.text << "\n";
    }

    // ---- Verify ----
    if (results.size() == 2 && results[0].id == "doc1") {
        std::cout << "\nRetriever tests passed!\n";
    } else {
        std::cout << "\nRetriever tests FAILED!\n";
    }

    return 0;
}
