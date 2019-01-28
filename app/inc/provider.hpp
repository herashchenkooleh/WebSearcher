#pragma once

#include "data_item.hpp"
#include "qobject_list_model.hpp"
#include <QList>
#include <QObject>
#include <QSharedPointer>

DECLARE_Q_OBJECT_LIST_MODEL( DataItem )

class Provider : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QObjectListModel_DataItem* items READ items CONSTANT )

public:
    explicit Provider( QObject* parent = Q_NULLPTR );

    Q_INVOKABLE void addItem(const QString& url);
    Q_INVOKABLE void removeItem(const int index);
	Q_INVOKABLE void removeAllItems();
	Q_INVOKABLE QSharedPointer<DataItem> getItem(const int index);
	QSharedPointer<DataItem> getItemByUrl(const std::string& url);

	QObjectListModel_DataItem* items();

private:
    QObjectListModel_DataItem  m_items;
};