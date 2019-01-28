#pragma once

#include<string>
#include <map>
#include <vector>
#include <memory>

#include "WebSite.hpp"

class Dispatcher
{
public:
	using ProgressCallback = std::function<void(const float)>;

public:
	Dispatcher();

	Dispatcher(Dispatcher&&) = delete;
	Dispatcher(const Dispatcher&) = delete;

	Dispatcher& operator=(Dispatcher&&) = delete;
	Dispatcher& operator=(const Dispatcher&) = delete;

	~Dispatcher();

	void executeSearch(const size_t numThread, const size_t searchDepth,
						const std::string& url, const std::string& searchText);

	void setProgressCallback(ProgressCallback callback);

protected:
	void  addProgress(const float value);
	void  updateProgress(const float value);

	void loadWebSites(size_t& searchDepth, std::queue<WebSite::Ptr>& urls);
	void searchOnWebSites(const std::string& searchText);

private:
	ThreadPool::Ptr mThreadPool;

	std::map<std::string, WebSite::Ptr> mWebSites;

	ProgressCallback mProgressCallback;

	float mTotalProgress;
	float mProgressStep;

	std::mutex mAddProgressMutex;
};