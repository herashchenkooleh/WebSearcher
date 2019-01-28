#include "WebSite.hpp"
#include "UrlLoadTask.hpp"
#include <iostream>
#include <regex>
#include "rapidxml.hpp"
#include <chrono>

static constexpr const char* HREF_SIGNATURE = "href";

std::vector<std::string> walk(const rapidxml::xml_node<>* node, std::string& data)
{
	if (node == nullptr)
	{
		return {};
	}

	if (node->value_size() != 0)
	{
		data.append(node->value());
	}

	std::vector<std::string> result;
	for (const rapidxml::xml_attribute<>* a = node->first_attribute()
		; a
		; a = a->next_attribute()
		)
	{
		if (std::strcmp(a->name(), HREF_SIGNATURE) == 0)
		{
			auto value = std::string(a->value());
			result.push_back(value);
		}
	}

	for (const rapidxml::xml_node<>* n = node->first_node()
		; n
		; n = n->next_sibling()
		)
	{
		auto localResult = std::move(walk(n, data));
		std::copy(localResult.begin(), localResult.end(),
			std::back_inserter(result));
	}

	return result;
}

bool search(const std::string& data, const std::string& searchText)
{
	if (data.find(searchText) != std::string::npos)
	{
		return true;
	}

	return false;
}

/*static*/ WebSite::CreateCallback WebSite::sCreateCallback = nullptr;
/*static*/ WebSite::ChangeStatusCallback WebSite::sChangeStatusCallback = nullptr;

/*static*/ bool WebSite::isValidHRef(const std::string& str)
{
	auto result = std::regex_match(str, std::regex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)"));
	return result;
}

/*static*/ void WebSite::setCreateCallback(CreateCallback callback)
{
	sCreateCallback = callback;
}

/*static*/ void WebSite::setChangeStatusCallback(ChangeStatusCallback callback)
{
	sChangeStatusCallback = callback;
}

/*static*/ void WebSite::callCreateCallback(const std::string& url)
{
	if (sCreateCallback != nullptr)
	{
		sCreateCallback(url);
	}
}

/*static*/ void WebSite::callChangeStatusCallback(const std::string& url,
												const WebSiteStatus status,
												const std::string& error)
{
	if (sChangeStatusCallback != nullptr)
	{
		sChangeStatusCallback(url, status, error);
	}
}

std::vector<std::string> WebSite::getChildUrls() const
{
	if (mStatus == WebSiteStatus::FAILED)
	{
		return {};
	}

	mParsed = true;
	result.wait();

	if (result.valid())
	{
		auto str = result.get();
		mStatus = WebSiteStatus::PARSE;
		WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);

		try
		{
			rapidxml::xml_document<> xmlDoc;
			xmlDoc.parse<rapidxml::parse_default>(const_cast<char*>(str.c_str()));

			auto children = std::move(walk(xmlDoc.first_node(), mData));

			mStatus = WebSiteStatus::PARSED;
			WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);

			return children;
		}
		catch (const std::exception& e)
		{
			mStatus = WebSiteStatus::FAILED;
			mErrorDescripton = e.what();
			WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);

			return {};
		}
	}

	mStatus = WebSiteStatus::FAILED;
	WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);

	return {};
}

void WebSite::searchText(const std::string& searchText, FoundCallback callback)
{
	mFoundCallback = callback;
	if (mStatus == WebSiteStatus::FAILED)
	{
		return;
	}

	if (!mParsed)
	{
		getChildUrls();
		mParsed = true;
	}

	auto result = mThreadPool->enqueue([&, searchText]()
	{
		try
		{
			mStatus = WebSiteStatus::SEARCH;
			WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);
			if (search(mData, searchText))
			{
				mStatus = WebSiteStatus::FOUND;
				WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);
			}
			else
			{
				mStatus = WebSiteStatus::NOT_FOUND;
				WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);
			}

			mFoundCallback(mProgress);
		}
		catch (const std::exception& e)
		{
			mStatus = WebSiteStatus::FAILED;
			mErrorDescripton = e.what();
			WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);

			return;
		}
	});
}

void WebSite::load(LoadedCallback callback)
{
	mLoadedCallback = callback;
	WebSite::callCreateCallback(mUrl);
	result = mThreadPool->enqueue([&] {
		try
		{
			mStatus = WebSiteStatus::LOAD;
			WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);
			UrlLoadTask task;
			std::string data;
			data = std::move(task.load(mUrl));
			mStatus = WebSiteStatus::LOADED;
			WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);

			mLoadedCallback(mProgress);

			return data;
		}
		catch (const std::exception&)
		{
			mStatus = WebSiteStatus::FAILED;
			WebSite::callChangeStatusCallback(mUrl, mStatus, mErrorDescripton);
		}
	});
}

WebSite::WebSite(ThreadPool::Ptr threadPool,
				const std::string& url,
				const float progress)
	: mThreadPool(threadPool)
	, mUrl(url)
	, mParsed(false)
	, mProgress(progress)
{
}