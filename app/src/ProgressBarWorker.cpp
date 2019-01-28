#include "ProgressBarWorker.hpp"

/*explicit*/ ProgressBarWorker::ProgressBarWorker(QObject* parent/* = nullptr*/)
	: QObject(parent)
	, mCount(100.f)
{
}

float ProgressBarWorker::progress()
{
	return mProgress;
}

void ProgressBarWorker::setProgress(const float progress)
{
	mProgress = progress * mCount;

	emit onProgressChanged(mProgress);
}

float& ProgressBarWorker::getCount()
{
	return mCount;
}