#include "UrlLoadTask.hpp"
#include <sstream>
#include <iostream>

UrlLoadTask::UrlLoadTask()
{
}

UrlLoadTask::~UrlLoadTask()
{
}

void UrlLoadTask::load(const std::string& url, std::string& data)
{
	if (url.empty())
	{
		return;
	}

	init(url, &data);

	auto code = curl_easy_perform(mConnect);
	curl_easy_cleanup(mConnect);
	if (code != CURLE_OK)
	{
		std::stringstream ss;
		ss << "Failed curl " << mErrorBuffer;

		throw std::runtime_error(ss.str());
	}
}

void UrlLoadTask::init(const std::string& url, std::string* buffer)
{
	mConnect = curl_easy_init();
	if (mConnect == nullptr)
	{
		throw std::runtime_error("Failed to create CURL connection");
	}

	auto code = curl_easy_setopt(mConnect, CURLOPT_ERRORBUFFER, mErrorBuffer);
	if (code != CURLE_OK)
	{
		std::stringstream ss;
		ss << "Failed to set error buffer " << code;

		throw std::runtime_error(ss.str());
	}

	code = curl_easy_setopt(mConnect, CURLOPT_URL, url.c_str());
	if (code != CURLE_OK)
	{
		std::stringstream ss;
		ss << "Failed to set URL " << std::string(mErrorBuffer);

		throw std::runtime_error(ss.str());
	}

	code = curl_easy_setopt(mConnect, CURLOPT_WRITEFUNCTION, UrlLoadTask::writer);
	if (code != CURLE_OK)
	{
		std::stringstream ss;
		ss << "Failed to set writer " << std::string(mErrorBuffer);

		throw std::runtime_error(ss.str());
	}

	code = curl_easy_setopt(mConnect, CURLOPT_WRITEDATA, buffer);
	if (code != CURLE_OK)
	{
		std::stringstream ss;
		ss << "Failed to set write data " << std::string(mErrorBuffer);

		throw std::runtime_error(ss.str());
	}
}

/*static*/ int UrlLoadTask::writer(char *data, size_t size, size_t nmemb, std::string* writerData)
{
	if (writerData == nullptr)
	{
		return 0;
	}

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

/*static*/ void UrlLoadTask::init()
{
	curl_global_init(CURL_GLOBAL_ALL);
}