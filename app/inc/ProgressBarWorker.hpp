#pragma once

#include <QObject>

class ProgressBarWorker : public QObject
{
	Q_OBJECT

	Q_PROPERTY(float progress READ progress NOTIFY onProgressChanged)
public:
	explicit ProgressBarWorker(QObject* parent = nullptr);

	Q_INVOKABLE float progress();

	void setProgress(const float progress);

	float& getCount();

signals:
	void onProgressChanged(float progress);

private:
	float mProgress;
	float mCount;
};