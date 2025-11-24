#include "embedding_http.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    std::string* out = static_cast<std::string*>(userp);
    size_t total_size = size * nmemb;
    out->append(static_cast<char*>(contents), total_size);
    return total_size;
}



embedding_http::embedding_http(const std::string& url,int embedding_dim):
	url_(url),
    embedding_dim_(embedding_dim)
{
}

std::vector<float> embedding_http::embed_text(const std::string& text)
{
    

    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("curl init failed");

    std::string readbuffer;
    

    nlohmann::json req = { {"text", text} };
    std::string body = req.dump();

    


    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // prevent hang
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L); // keep connection alive

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_callback);
    //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rawBuffer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(&readbuffer));

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        throw std::runtime_error(std::string("curl error: ") + curl_easy_strerror(res));
    }


    // Debug output
    //std::cout << "[DEBUG] Server returned: " << readbuffer << "\n";

    // Parse JSON
    nlohmann::json j;
    try {
        j = nlohmann::json::parse(readbuffer);
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("JSON parse error: ") + e.what());
    }
    if (!j.contains("embedding") || !j["embedding"].is_array()) {
        throw std::runtime_error("invalid embedding response");
    }

    std::vector<float> vec;
    //for (auto& v : j["embedding"]) std::cout << v << std::endl;
    std::cout << "Embedding size: " << j["embedding"].size() << std::endl;

    vec.reserve(j["embedding"].size());

    for (auto& v : j["embedding"]) 
    {
        if (!v.is_number()) {
            std::cerr << "[ERROR] Non-number in embedding: " << v << std::endl;
            continue;
        }
        vec.push_back(static_cast<float>(v.get<double>()));
    }
    std::cout << "Embedding size: " << vec.size() << std::endl;
    return vec;
}
