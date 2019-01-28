#pragma once

#include "Dispatcher.hpp"
#include "ProgressBarWorker.hpp"
#include "search_worker.hpp"
#include "provider.hpp"
#include <QThread>

class SearchManager : public QObject
{
	Q_OBJECT

public:
	static SearchManager& instance();

	SearchManager(SearchManager&&) = delete;
	SearchManager(const SearchManager&) = delete;

	SearchManager& operator=(SearchManager&&) = delete;
	SearchManager& operator=(const SearchManager&) = delete;

	~SearchManager();

	Provider& getProvider();

	void startNewSearch(const QString& url,
						const QString& searchText,
						const std::uint32_t searchDepth,
						const std::uint32_t numThreads);

	ProgressBarWorker& getProgressBarWorker();

public slots:
	void createSlot(const QString&);
	void changeSlot(const QString& ,
					const int,
					const QString&);
	void progressSlot(const float);

protected:
	explicit SearchManager(QObject* parent = nullptr);

private:
	Provider mProvider;
	QThread* mSearchThread;
	SearchWorker* mWorker;
	ProgressBarWorker mProgressBarWorker;
};