#include "search_manager.hpp"
#include "WebSite.hpp"

/*static*/ SearchManager& SearchManager::instance()
{
	static SearchManager sInstance;

	return sInstance;
}

SearchManager::~SearchManager()
{
}

Provider& SearchManager::getProvider()
{
	return mProvider;
}

void SearchManager::startNewSearch(const QString& url,
									const QString& searchText,
									const std::uint32_t searchDepth,
									const std::uint32_t numThreads)
{
	mProvider.removeAllItems();

	if (mSearchThread != nullptr)
	{
		mSearchThread->terminate();
		mSearchThread->wait();
		delete mSearchThread;
		mSearchThread = nullptr;
		delete mWorker;
	}
	
	mSearchThread = new QThread();
	mWorker = new SearchWorker(url, searchText, searchDepth, numThreads);
	mWorker->moveToThread(mSearchThread);

	QObject::connect(mSearchThread, SIGNAL(started()), mWorker, SLOT(process()));
	QObject::connect(mWorker, &SearchWorker::createSingal, this, &SearchManager::createSlot);
	QObject::connect(mWorker, &SearchWorker::changeSingal, this, &SearchManager::changeSlot);
	QObject::connect(mWorker, &SearchWorker::progressSignal, this, &SearchManager::progressSlot);

	mSearchThread->start();
}

ProgressBarWorker& SearchManager::getProgressBarWorker()
{
	return mProgressBarWorker;
}

/*explicit*/ SearchManager::SearchManager(QObject* parent /*= nullptr*/)
	: QObject(parent)
	, mSearchThread(nullptr)
{
}

void SearchManager::createSlot(const QString& url)
{
	mProvider.addItem(url);
}

void SearchManager::changeSlot(const QString& url,
								const int status,
								const QString& error)
{
	auto item = mProvider.getItemByUrl(url.toStdString());
	if (item == nullptr)
	{
		return;
	}

	switch (status)
	{
	case WebSite::WebSiteStatus::LOAD:
		item->setStatus("load");
		break;
	case WebSite::WebSiteStatus::PARSE:
		item->setStatus("parse");
		break;
	case WebSite::WebSiteStatus::PARSED:
		item->setStatus("parsed");
		break;
	case WebSite::WebSiteStatus::SEARCH:
		item->setStatus("search");
		break;
	case WebSite::WebSiteStatus::FOUND:
		item->setStatus("found");
		break;
	case WebSite::WebSiteStatus::NOT_FOUND:
		item->setStatus("not found");
		break;
	case WebSite::WebSiteStatus::FAILED:
		item->setStatus("failed");
		item->setErrorCode(error);
	default:
		break;
	}
}

void SearchManager::progressSlot(const float value)
{
	mProgressBarWorker.setProgress(value);
}
