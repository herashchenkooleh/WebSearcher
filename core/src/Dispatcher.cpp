#include "Dispatcher.hpp"
#include "UrlLoadTask.hpp"

Dispatcher::Dispatcher()
{
	UrlLoadTask::init();
}

Dispatcher::~Dispatcher()
{
}

void Dispatcher::setProgressCallback(ProgressCallback callback)
{
	mProgressCallback = callback;
}

void Dispatcher::addProgress(const float value)
{
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

void Dispatcher::executeSearch(const size_t numThread, const size_t searchDepth,
	const std::string& url, const std::string& searchText,
	WebSite::ChangeStatusCallback callback)
{
	updateProgress(0.0f);
	if (!WebSite::isValidHRef(url))
	{
		updateProgress(1.0f);
		return;
	}

	mProgressStep = 1.0f / (searchDepth * 3);

	mThreadPool = std::make_shared<ThreadPool>(numThread);

	mWebSites[url] = std::make_shared<WebSite>(url, searchText, callback);
	SitesList sites = { mWebSites[url] };
	auto localSearchDepth = searchDepth != 0 ? searchDepth - 1 : 0;
	loadWebSites(localSearchDepth, sites, searchText, callback);

	for (auto& item: mWebSites)
	{
		auto site = item.second;
		mThreadPool->enqueue([&, site]()
		{
			site->searchText();
			addProgress(mProgressStep);
		});
	}

	updateProgress(1.0f);
}

void Dispatcher::loadWebSites(size_t& searchDepth, SitesList& sites,
							const std::string& searchText,
							WebSite::ChangeStatusCallback callback)
{
	if (searchDepth == 0)
	{
		return;
	}
	while (!sites.empty())
	{
		std::vector<std::future<std::string>> loadResults;
		for (auto& site : sites)
		{
			loadResults.push_back(mThreadPool->enqueue([&, site]()
			{
				site->load();
				addProgress(mProgressStep);
				return site->getUrl();
			}));
		}

		std::vector<std::future<std::string>> parseResults;

		do
		{
			auto removeFunc = [&](std::future<std::string>& future)
			{
				auto status = future.wait_for(std::chrono::milliseconds(100));
				if (status == std::future_status::ready)
				{
					auto item = mWebSites[future.get()];
					parseResults.push_back(mThreadPool->enqueue([&, item]()
					{
						item->parse();
						addProgress(mProgressStep);
						return item->getUrl();
					}));

					return true;
				}

				return false;
			};

			const auto& newEnd = std::remove_if(std::begin(loadResults),
												std::end(loadResults), removeFunc);
			loadResults.erase(newEnd, loadResults.end());
		} while (!loadResults.empty());

		if (searchDepth == 0)
		{
			break;
		}

		sites.clear();
		for (auto& future : parseResults)
		{
			future.wait();
			auto site = mWebSites[future.get()];
			for (auto& child : site->getChildren())
			{
				auto item = mWebSites.find(child);
				if (item == mWebSites.end())
				{
					auto webSite = std::make_shared<WebSite>(child,
															searchText,
															callback);
					mWebSites[child] = webSite;
					sites.push_back(webSite);

					searchDepth -= 1;
					if (searchDepth == 0)
					{
						break;
					}
				}
			}
		}
	}
}
