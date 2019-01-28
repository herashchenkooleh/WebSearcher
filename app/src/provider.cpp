#include "provider.hpp"

Provider::Provider( QObject* parent )
    : QObject( parent )
{
}

void Provider::addItem(const QString& url)
{
    auto item = QSharedPointer<DataItem>(new DataItem());

	item->setStatus("");
	item->setUrl(url);
	item->setErrorCode("0");

	{
		m_items << item;
	}
}

void Provider::removeItem(const int index)
{
	if (m_items.count() == 0 || index >= m_items.size())
	{
		return;
	}

	{
		m_items.removeAt(index);
	}
}

void Provider::removeAllItems()
{
	{
		m_items.clear();
	}
}

QSharedPointer<DataItem> Provider::getItem(const int index)
{
	if (m_items.count() == 0 || index >= m_items.size())
	{
		return nullptr;
	}

	QSharedPointer<DataItem> item = nullptr;
	{
		item = m_items.at(index);
	}

	return item;
}

QSharedPointer<DataItem> Provider::getItemByUrl(const std::string& url)
{
	for (auto& item: m_items)
	{
		if (item->url().toStdString() == url)
		{
			return item;
		}
	}

	return nullptr;
}

QObjectListModel_DataItem* Provider::items()
{
    return &m_items;
}
