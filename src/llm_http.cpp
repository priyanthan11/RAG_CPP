#include "llm_http.h"
#include <iostream>

// Callback to capture HTTP response into a string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) 
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

llm_http::llm_http(const std::string& url):
	url_(url)
{
}

std::string llm_http::generate(const std::string& model,const std::string& prompt, int max_tokens)
{

	CURL* curl = curl_easy_init();
	if (!curl) throw std::runtime_error("curl init failed");
	std::string readBuffer;
	nlohmann::json req = { {"model", model}, { "prompt",prompt },{"max_tokens",max_tokens}};

	try
	{
		struct curl_slist* headers = nullptr;
		headers = curl_slist_append(headers, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());

		std::string body = req.dump();
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		// Timeout
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			throw std::runtime_error(std::string("curl error: ") + curl_easy_strerror(res));
		
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);


		std::istringstream ss(readBuffer);
		std::string line;
		std::string final_response;
		while (std::getline(ss, line))
		{
			if (line.empty())continue;
			try
			{
				auto j = nlohmann::json::parse(line);
				if (j.contains("response"))
				{
					final_response += j["response"].get<std::string>();
				}
				if (j.contains("done") && j["done"].get<bool>())
				{
					break;
				}

			}
			catch (const std::exception& e)
			{
				std::cout << "[ERROR] Failed to parse line: " << line << "\n";
			}
		}
		return final_response;
		/*auto j = nlohmann::json::parse(readBuffer);

		if (j.contains("text")) return j["text"].get<std::string>();
		if (j.contains("outputs") && j["outputs"].is_array() && !j["outputs"].empty())
			return j["outputs"][0].get<std::string>();
		throw std::runtime_error("invalid LLM response");*/

	}
	catch (const std::exception& e)
	{
		std::cout << "[SYS]: Exectipi at Generate method:" << e.what() << std::endl;
	}
	return readBuffer;
	//struct curl_slist* headers = nullptr;
	//headers = curl_slist_append(headers, "Content-Type: application/json");
	//
	//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	//curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
	//
	//std::string body = req.dump();
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
	//// Use a lambda **without capture** -> converts to C function pointer
	//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
	//	[](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
	//		std::string* out = static_cast<std::string*>(userp);
	//		out->append(static_cast<char*>(contents), size * nmemb);
	//		return size * nmemb;
	//	});
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
	//CURLcode res = curl_easy_perform(curl);
	//curl_slist_free_all(headers);
	//curl_easy_cleanup(curl);
	//if (res != CURLE_OK)
	//	throw std::runtime_error(std::string("curl error: ") + curl_easy_strerror(res));
	//auto j = nlohmann::json::parse(readBuffer);
	//if (j.contains("text")) return j["text"].get<std::string>();
	//if (j.contains("outputs") && j["outputs"].is_array() && !j["outputs"].empty())
	//	return j["outputs"][0].get<std::string>();
	//throw std::runtime_error("invalid LLM response");


	
}
