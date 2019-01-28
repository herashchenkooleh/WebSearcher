#include "Dispatcher.hpp"

Dispatcher::Dispatcher()
	: mProgressStep(0.0f)
	, mTotalProgress(0.0f)
{
}

Dispatcher::~Dispatcher()
{
}

void Dispatcher::executeSearch(const size_t numThread, const size_t searchDepth,
								const std::string& url, const std::string& searchText)
{
	if (!WebSite::isValidHRef(url))
	{
		return;
	}

	updateProgress(0.0f);
	auto numSites = searchDepth;
	mThreadPool = std::make_shared<ThreadPool>(numThread);

	mWebSites.clear();

	std::queue<WebSite::Ptr> urls;
	mProgressStep = 0.5f / (float)searchDepth;
	auto site = std::make_shared<WebSite>(mThreadPool, url, mProgressStep);
	site->load([&](const float value)
	{
		addProgress(value);
	});
	urls.push(site);
	mWebSites[url] = site;

	loadWebSites(numSites, urls);
	updateProgress(0.5f);
	searchOnWebSites(searchText);
	updateProgress(1.0f);
}

void Dispatcher::setProgressCallback(ProgressCallback callback)
{
	mProgressCallback = callback;
}

void Dispatcher::addProgress(const float value)
{
	std::lock_guard<std::mutex> lock(mAddProgressMutex);
	mTotalProgress += value;
	if (mProgressCallback != nullptr)
	{
		mProgressCallback(mTotalProgress);
	}
}

void Dispatcher::updateProgress(const float value)
{
	mTotalProgress = value;
	if (mProgressCallback != nullptr)
	{
		mProgressCallback(mTotalProgress);
	}
}

void Dispatcher::loadWebSites(size_t& searchDepth, std::queue<WebSite::Ptr>& urls)
{
	if (searchDepth == 0 || urls.empty())
	{
		return;
	}

	auto webSite = urls.front();
	urls.pop();

	auto children = webSite->getChildUrls();
	for (const auto& child : children)
	{
		auto itr = mWebSites.find(child);
		if (itr == mWebSites.end() && WebSite::isValidHRef(child))
		{
			--searchDepth;
			if (searchDepth == 0)
			{
				break;
			}

			auto site = std::make_shared<WebSite>(mThreadPool, child, mProgressStep);
			site->load([&](const float value)
			{
				addProgress(value);
			});
			urls.push(site);
			mWebSites[child] = site;
		}
	}

	loadWebSites(searchDepth, urls);
}

void Dispatcher::searchOnWebSites(const std::string& searchText)
{
	for (auto& site: mWebSites)
	{
		site.second->searchText(searchText,
			[&](const float value)
			{
				addProgress(value);
			}
		);
	}
}