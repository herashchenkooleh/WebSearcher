#include "search_worker.hpp"

SearchWorker::SearchWorker(const QString& url,
							const QString& searchText,
							const std::uint32_t searchDepth,
							const std::uint32_t numThreads)
	: mUrl(url)
	, mSearchText(searchText)
	, mSearchDepth(searchDepth)
	, mNumThreads(numThreads)
{
	dispatcher.setProgressCallback([&](const float value)
	{
		emit progressSignal(value);
	});
}

SearchWorker::~SearchWorker()
{
}

void SearchWorker::process()
{
	auto createCallback = [&](const std::string& url,
							const WebSite::WebSiteStatus status,
							const std::string& error)
	{
		switch (status)
		{
		case WebSite::WebSiteStatus::CREATED:
			emit createSingal(url.c_str());
			break;
		case WebSite::WebSiteStatus::LOAD:
		case WebSite::WebSiteStatus::LOADED:
		case WebSite::WebSiteStatus::PARSE:
		case WebSite::WebSiteStatus::PARSED:
		case WebSite::WebSiteStatus::SEARCH:
		case WebSite::WebSiteStatus::FOUND:
		case WebSite::WebSiteStatus::NOT_FOUND:
		case WebSite::WebSiteStatus::FAILED:
			emit changeSingal(url.c_str(), status, error.c_str());
			break;
		default:
			break;
		}
	};

	dispatcher.executeSearch(mNumThreads, mSearchDepth,
							mUrl.toStdString(),
							mSearchText.toStdString(),
							createCallback);
	emit finished();
}