#pragma once

#include <string>
#include "curl/curl.h"
#include <future>

class UrlLoadTask
{
public:
	UrlLoadTask();

	UrlLoadTask(const UrlLoadTask&) = delete;
	UrlLoadTask(UrlLoadTask&&) = delete;

	UrlLoadTask& operator=(UrlLoadTask&&) = delete;
	UrlLoadTask& operator=(const UrlLoadTask&) = delete;

	~UrlLoadTask();

	static int writer(char *data, size_t size, size_t nmemb, std::string* writerData);

	void load(const std::string& url, std::string& data);

	static void init();

protected:
	void init(const std::string& url, std::string* buffer);

private:
	CURL* mConnect = nullptr;

	char mErrorBuffer[CURL_ERROR_SIZE];
};