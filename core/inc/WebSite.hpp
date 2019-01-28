#pragma once

#include <functional>
#include <map>
#include "ThreadPool.hpp"

class WebSite
{
public:
	enum WebSiteStatus
	{
		LOAD,
		LOADED,
		PARSE,
		PARSED,
		SEARCH,
		FOUND,
		NOT_FOUND,
		FAILED
	};

	using Ptr = std::shared_ptr<WebSite>;

public:
	WebSite(ThreadPool::Ptr threadPool, const std::string& url, const float progress);

	using CreateCallback = std::function<void(const std::string&)>;
	using ChangeStatusCallback = std::function<void(const std::string&,
													const WebSiteStatus,
													const std::string&)>;
	using LoadedCallback = std::function<void(const float)>;
	using FoundCallback = std::function<void(const float)>;

	static bool isValidHRef(const std::string& str);

	static void setCreateCallback(CreateCallback);
	static void setChangeStatusCallback(ChangeStatusCallback);

	static void callCreateCallback(const std::string&);
	static void callChangeStatusCallback(const std::string&, const WebSiteStatus, const std::string&);

	void load(LoadedCallback callback);

	std::vector<std::string> getChildUrls() const;

	void searchText(const std::string& searchText, FoundCallback callback);

private:
	static CreateCallback sCreateCallback;
	static ChangeStatusCallback sChangeStatusCallback;

	LoadedCallback mLoadedCallback;
	FoundCallback mFoundCallback;

	ThreadPool::Ptr mThreadPool;
	std::string mUrl;
	mutable std::future<std::string> result;
	mutable std::string mData;

	mutable bool mParsed;
	mutable WebSiteStatus mStatus;

	mutable std::string mErrorDescripton;

	float mProgress;
};