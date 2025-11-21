add_executable(rag_embedding_tests
    test_chunker.cpp
    test_vector_store.cpp
    test_embedding.cpp
    test_vector_store.cpp
    test_retriever.cpp

    ../src/retriever.cpp
    ../src/embedding_http.cpp      # add the implementation file
    ../src/embedding_client.cpp    # add this if you also use the base class
    ../src/vector_store.cpp    # <-- add this
    ../src/util.cpp 
)

target_include_directories(rag_embedding_tests
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(rag_embedding_tests
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE CURL::libcurl
)
