#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtWebView/QtWebView>
#include "qml_cpp_bridge.hpp"
#include "search_manager.hpp"
#include "data_item.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
	qmlRegisterType<QmlCppBridge>("com.qml_cpp_bridge", 1, 0, "QmlCppBridge");
	qmlRegisterUncreatableType<DataItem>("com.data_item", 1, 0, "DataItem", "interface");

	QtWebView::initialize();
    QQmlApplicationEngine engine;
	QQmlContext* context = engine.rootContext();
	context->setContextProperty(QStringLiteral("searchItemList"),
								SearchManager::instance().getProvider().items());

	context->setContextProperty(QStringLiteral("worker"),
								&SearchManager::instance().getProgressBarWorker());

	context->setContextProperty(QStringLiteral("Count"),
								SearchManager::instance().getProgressBarWorker().getCount());

	engine.addImportPath(QStringLiteral("qrc:/"));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}