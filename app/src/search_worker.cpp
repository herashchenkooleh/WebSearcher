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
	WebSite::setCreateCallback([&](const std::string& url)
	{
		emit createSingal(url.c_str());
	});

	WebSite::setChangeStatusCallback([&](const std::string& url,
		const WebSite::WebSiteStatus status,
		const std::string& error)
	{
		emit changeSingal(url.c_str(), status, error.c_str());
	});

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
	dispatcher.executeSearch(mNumThreads, mSearchDepth,
							 mUrl.toStdString(),
							 mSearchText.toStdString());
	emit finished();
}