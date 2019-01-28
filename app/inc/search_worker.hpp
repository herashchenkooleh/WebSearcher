#pragma once

#include <QObject>
#include "Dispatcher.hpp"

class SearchWorker : public QObject
{
	Q_OBJECT
public:
	SearchWorker(const QString& url,
				const QString& searchText,
				const std::uint32_t searchDepth,
				const std::uint32_t numThreads);

	~SearchWorker();

public slots:
	void process();

signals:
	void finished();
	void createSingal(const QString&);
	void changeSingal(const QString&,
					const int,
					const QString&);
	void progressSignal(const float);

private:
	Dispatcher dispatcher;

	QString mUrl;
	QString mSearchText;
	std::uint32_t mSearchDepth;
	std::uint32_t mNumThreads;
};