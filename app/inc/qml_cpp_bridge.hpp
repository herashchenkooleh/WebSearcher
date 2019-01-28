#pragma once

#include <QObject>

class QmlCppBridge : public QObject
{
	Q_OBJECT

public:
	explicit QmlCppBridge(QObject* parent = 0);

	Q_INVOKABLE void startSearch(const QString& url,
								const QString& searchText,
								const QString& searchDepth,
								const QString& numThreads);
};