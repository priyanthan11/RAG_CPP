#include "config.h"
#include "chunker.h"
#include "embedding_client.h"
#include "embedding_http.h"
#include "ollama_embeddings.h"
#include "vector_store.h"
#include "retriever.h"
#include "llm_client.h"
#include "rag_engine.h"
#include "llm_http.h"
#include "util.h"
#include <memory>
#include<fstream>
#include <iostream>
namespace fs = std::filesystem;
int main() 
{
   
    // Load config and corpus
    std::string config_path = "C:/___C++/AI ML Projects/RAG_v.2/data/config.json";
    std::string corpus_path = "C:/___C++/AI ML Projects/RAG_v.2/data/corpus.txt";
    std::cout << "[SYS] RAG Engine starting...\n";

    // Validate files exist
    if (!fs::exists(config_path)) {
        std::cerr << "[ERROR] Config file does not exist: " << config_path << "\n";
        return 1;
    }
    if (!fs::exists(corpus_path)) {
        std::cerr << "[ERROR] Corpus file does not exist: " << corpus_path << "\n";
        return 1;
    }

    std::cout << "[SYS] RAG Engine starting...\n";

    // Load config
    config cfg = config::from_json_file(config_path);
    std::cout << "[SYS] JSON Loaded\n";

    // Load corpus text
    std::ifstream ifs(corpus_path);
    std::string text((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
    std::cout << "[SYS] Corpus Loaded, size: " << text.size() << " chars\n";

    // Chunking
    DocumentChunker chunker(cfg.getChunk_size(), cfg.getChunk_overlap());
    auto chunks = chunker.chunk_text(text, corpus_path);
    std::cout << "[SYS] Chunking complete, " << chunks.size() << " chunks generated\n";

    // Vector embeddings
    auto llmClient = std::make_shared<llm_http>(cfg.getLlm_endpoint());
    auto embClient = std::make_shared<embedding_http>(cfg.getEmbed_endpoint(), 512);// 512 is embedding dimension
    auto vs = std::make_shared<vector_store>();
    std::cout << "[SYS] Vector embeddings complete, total vectors: " << vs->size() << "\n";

    util u;
    for (auto& c : chunks)
    {
        auto vec = embClient->embed_text(c.text);
        std::cout << "[DEBUG] chunk: " << c.text << ", embedding size: " << vec.size() << "\n";
        u.normalize_inplace(vec);
        VectorRecord r; 
        r.id = c.id;
        r.vec = vec;
        r.chunk = c;
        vs->add(r);
    }
    std::cout << "[SYS] Vector embeddings complete, total vectors: " << vs->size() << "\n";
    // Save vector store
    vs->save_to_file("Vectors.json");

    // Retrever + LLM engine
    auto retvr = std::make_shared<retriever>(embClient, vs, cfg.getTop_k());
    auto llm = std::make_shared<llm_http>(cfg.getLlm_endpoint());
    rag_engine engine(retvr, llm, cfg.getMax_context_tokens());

    // Interactive query loop
    std::string q;
    std::cout << "Question: ";
    while (std::getline(std::cin,q))
    {
        if (q.empty()) { std::cout << "question: "; continue; }
        try
        {
            std::string ans = engine.answer(q);
            std::cout << "answer> " << ans << "\n";

        }
        catch (const std::exception&e)
        {
            std::cerr << "error: " << e.what() << "\n";
        }
        std::cout << "question: ";
    }





    return 0;
}