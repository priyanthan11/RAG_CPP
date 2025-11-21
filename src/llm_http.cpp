#include "llm_http.h"



llm_http::llm_http(const std::string& url):
	url_(url)
{
}

std::string llm_http::generate(const std::string& prompt, int max_tokens)
{

	CURL* curl = curl_easy_init();
	if (!curl) throw std::runtime_error("curl init failed");
	std::string readBuffer;
	nlohmann::json req = { {"prompt",prompt},{"max_tokens",max_tokens} };

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	std::string body = req.dump();
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
	// Use a lambda **without capture** -> converts to C function pointer
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
		[](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
			std::string* out = static_cast<std::string*>(userp);
			out->append(static_cast<char*>(contents), size * nmemb);
			return size * nmemb;
		});
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	CURLcode res = curl_easy_perform(curl);
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	if (res != CURLE_OK)
		throw std::runtime_error(std::string("curl error: ") + curl_easy_strerror(res));
	auto j = nlohmann::json::parse(readBuffer);
	if (j.contains("text")) return j["text"].get<std::string>();
	if (j.contains("outputs") && j["outputs"].is_array() && !j["outputs"].empty())
		return j["outputs"][0].get<std::string>();
	throw std::runtime_error("invalid LLM response");


	
}
