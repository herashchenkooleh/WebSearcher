#include "WebSite.hpp"
#include "UrlLoadTask.hpp"
#include <regex>
#include <chrono>
#include <iostream>

static constexpr const char* HREF_SIGNATURE = "href";

/*static*/ bool WebSite::isValidHRef(const std::string& str)
{
	auto result = std::regex_match(str, std::regex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)"));

	return result;
}

void WebSite::parse(const rapidxml::xml_node<>* node)
{
	if (node == nullptr)
	{
		return;
	}

	if (node->value_size() != 0)
	{
		mParsedData.append(node->value());
	}

	for (const rapidxml::xml_attribute<>* a = node->first_attribute()
		; a
		; a = a->next_attribute()
		)
	{
		if (std::strcmp(a->name(), HREF_SIGNATURE) == 0)
		{
			auto value = std::string(a->value());
			if (isValidHRef(value))
			{
				mChildren.push_back(value);
			}
		}
	}

	for (const rapidxml::xml_node<>* n = node->first_node()
		; n
		; n = n->next_sibling()
		)
	{
		parse(n);
	}
}

void WebSite::callCallback()
{
	if (mChangeStatusCallback !=  nullptr)
	{
		mChangeStatusCallback(mUrl, mStatus, mErrorDescripton);
	}
}

void WebSite::removeHtmlComments()
{
	while (mData.find("<!--") != std::string::npos)
	{
		auto startpos = mData.find("<!--");
		auto endpos = mData.find("-->") + 3;

		if (endpos != std::string::npos)
		{
			mData = mData.erase(startpos, endpos - startpos);
		}
	}
}

void WebSite::searchText()
{
	if (mStatus == WebSiteStatus::FAILED)
	{
		return;
	}

	mStatus = WebSiteStatus::SEARCH;
	callCallback();
	try
	{
		if (mParsedData.find(mSearchText) != std::string::npos)
		{
			mStatus = WebSiteStatus::FOUND;
		}
		else
		{
			mStatus = WebSiteStatus::NOT_FOUND;
		}
		callCallback();
	}
	catch (const std::exception& e)
	{
		mStatus = WebSiteStatus::FAILED;
		mErrorDescripton = e.what();
		callCallback();
	}
}

WebSite::Children& WebSite::getChildren()
{
	return mChildren;
}

const std::string& WebSite::getUrl() const
{
	return mUrl;
}

bool WebSite::parse()
{
	if (mStatus == WebSiteStatus::FAILED)
	{
		return false;
	}

	try
	{
		mStatus = WebSiteStatus::PARSE;
		callCallback();
		rapidxml::xml_document<> xmlDoc;
		xmlDoc.parse<rapidxml::parse_default>(const_cast<char*>(mData.c_str()));

		parse(xmlDoc.first_node());
		mStatus = WebSiteStatus::PARSED;
		callCallback();

		return true;
	}
	catch (const std::exception& e)
	{
		mStatus = WebSiteStatus::FAILED;
		mErrorDescripton = e.what();
		callCallback();

		return false;
	}
}

bool WebSite::load()
{
	try
	{
		mStatus = WebSiteStatus::LOAD;
		callCallback();
		UrlLoadTask task;
		task.load(mUrl, mData);
		mStatus = WebSiteStatus::LOADED;
		callCallback();

		removeHtmlComments();

		return true;
	}
	catch (const std::exception& e)
	{
		mStatus = WebSiteStatus::FAILED;
		mErrorDescripton = e.what();
		callCallback();

		return false;
	}
}

WebSite::WebSite(const std::string& url,
				const std::string& searchText,
				ChangeStatusCallback callback)
	: mUrl(url)
	, mChangeStatusCallback(callback)
	, mSearchText(searchText)
{
	mStatus = WebSiteStatus::CREATED;
	callCallback();
}