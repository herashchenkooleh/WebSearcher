#pragma once

#include "qobject_list_model.hpp"
#include <QDebug>
#include <QObject>
#include <QString>

class DataItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString status READ status WRITE setStatus NOTIFY changed )
	Q_PROPERTY( QString url READ url WRITE setUrl NOTIFY changed )
	Q_PROPERTY( QString errorCode READ errorCode WRITE setErrorCode NOTIFY changed )

public:
    explicit DataItem();

    QString status();
	QString url();
	QString errorCode();

    void setStatus(const QString& value);
	void setUrl(const QString& value);
	void setErrorCode(const QString& value);

signals:
    void changed();

private:
    QString mStatus;
	QString mUrl;
	QString mErrorCode;
};
