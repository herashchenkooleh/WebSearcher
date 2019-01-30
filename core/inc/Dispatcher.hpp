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
	using SitesMap = std::map<std::string, WebSite::Ptr>;
	using SitesList = std::list<WebSite::Ptr>;

public:
	Dispatcher();

	Dispatcher(Dispatcher&&) = delete;
	Dispatcher(const Dispatcher&) = delete;

	Dispatcher& operator=(Dispatcher&&) = delete;
	Dispatcher& operator=(const Dispatcher&) = delete;

	~Dispatcher();

	void executeSearch(const size_t numThread, const size_t searchDepth,
						const std::string& url, const std::string& searchText,
						WebSite::ChangeStatusCallback createCallback);

	void setProgressCallback(ProgressCallback callback);

protected:
	void loadWebSites(size_t& searchDepth, SitesList& urls,
					const std::string& searchText,
					WebSite::ChangeStatusCallback callback);

	void  addProgress(const float value);
	void  updateProgress(const float value);

private:
	float mTotalProgress;
	float mProgressStep;

	ThreadPool::Ptr mThreadPool;

	SitesMap mWebSites;

	ProgressCallback mProgressCallback;
};