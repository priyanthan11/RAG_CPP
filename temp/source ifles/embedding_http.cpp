#include "embedding_http.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <stdexcept>



embedding_http::embedding_http(const std::string& url):
	url_(url)
{
}

std::vector<float> embedding_http::embed_text(const std::string& text)
{

	// Initialize curl
	CURL* curl = curl_ease_init();
	if (!curl) throw std::runtime_error("curl init failed");

	std::string readbuffer;

	// Prepare JSON request body
	nlohmann::json req = { {"text",text} };

	// set HTTP headers
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_URL, irl_.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	
	std::string body = req.dump();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readbuffer);

	// Perform HTTP request
	CURLcode res = curl_easy_perform(curl);

	// Cleanup
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);

	if (res != CURLE_OK)
	{
		throw std::runtime_error(std::string("curl error: ") + curl_easy_strerror(res));
	}

	// Parse JSON response
	auto j = nlohmann::json::parse(readbuffer);
	std::vector<float>vec;
	if (j.contains("embedding") && j["embedding"].is_array())
	{
		for (auto& x : j["embedding"]) vec.push_back(x.get<float>());
	}
	else
	{
		throw std::runtime_error("invalid response: no embedding");
	}


	return vec;
}
