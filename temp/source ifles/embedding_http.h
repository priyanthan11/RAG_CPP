#pragma once
#include "embedding_client.h"


/*
* @brief Concrete implementation of Embedding Client using an HTTP API.
* HttpEmbedding Client sends the imput tet to a remote embedding service.
* (e.g., hosted LLM or vector API) via HTTP POST and parse the JSON response.
*/

class embedding_http :
    public embedding_client
{
    std::string url_;
    /*
    * @brif Callback for libcurl to write response data into a string.
    * 
    * @param contents Pointer to the recived data.
    * @param size Size of one data element.
    * @param nmemb Number of elements.
    * @param userp Pointer to the user-provided string buffer.
    * @return size_t Number of bytes processed.
    */
    static size_t write_cb(void* contents, size_t size, size_t nmemb, void* userp)
    {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
public:
    embedding_http(const std::string& url);
    /*
    * @brief Generate a vector embedding via HTTP.
    * 
    * Sends a POST request to the configured URL with JSON {"text":"<input>"}.
    * Expects JSON response with an array field "embedding " containing floats.
    * 
    * @param text Input text to embed.
    * @return std::vector <float> the resulting embedding vector.
    * 
    * @throws std::runtime_error If curl initialization fails, HTTP request fails, or the response is invalid
    * 
    */
    std::vector<float> embed_text(const std::string& text) override;
};

