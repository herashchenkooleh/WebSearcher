#include "qml_cpp_bridge.hpp"
#include "search_manager.hpp"

/*explicit*/ QmlCppBridge::QmlCppBridge(QObject* parent /*= 0*/)
	: QObject(parent)
{
}


/*Q_INVOKABLE*/ void QmlCppBridge::startSearch(const QString& url,
												const QString& searchText,
												const QString& searchDepth,
												const QString& numThreads)
{
	SearchManager::instance().startNewSearch(url, searchText,
											searchDepth.toUInt(), numThreads.toUInt());
}
