#include "data_item.hpp"

DataItem::DataItem()
	: QObject( NULL )
	, mStatus()
	, mUrl()
	, mErrorCode()
{
}

QString DataItem::status()
{
	return mStatus;
}

QString DataItem::url()
{
	return mUrl;
}

QString DataItem::errorCode()
{
	return mErrorCode;
}

void DataItem::setStatus(const QString& value)
{
	if (value != mStatus)
	{
		mStatus = value;
		emit changed();
	}
}

void DataItem::setUrl(const QString& value)
{
	if (value != mUrl)
	{
		mUrl = value;
		emit changed();
	}
}

void DataItem::setErrorCode(const QString& value)
{
	if (value != mErrorCode)
	{
		mErrorCode = value;
		emit changed();
	}
}