# rag_app executable
# src/CMakeLists.txt
add_executable(rag_app
    main.cpp
    chunker.cpp
    chunker.h
    config.cpp
    config.h
    embedding_client.cpp
    embedding_client.h
    embedding_http.cpp
    embedding_http.h
    util.cpp
    util.h
    vector_store.cpp
    vector_store.h
    retriever.cpp
    retriever.h
    llm_client.cpp
    llm_client.h
    llm_http.cpp
    llm_http.cpp
    rag_engine.h
    rag_engine.cpp
    ollama_embeddings.h
    ollama_embeddings.cpp
)

# Link libraries
target_link_libraries(rag_app
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE CURL::libcurl
)
