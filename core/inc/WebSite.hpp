#pragma once

#include <functional>
#include <set>
#include <map>
#include "ThreadPool.hpp"
#include "rapidxml.hpp"

class WebSite
{
public:
	enum WebSiteStatus
	{
		CREATED,
		LOAD,
		LOADED,
		PARSE,
		PARSED,
		SEARCH,
		FOUND,
		NOT_FOUND,
		FAILED
	};

	using Ptr = std::shared_ptr<WebSite>;

	using Children = std::vector<std::string>;

	using ChangeStatusCallback = std::function<void(const std::string&,
													const WebSiteStatus,
													const std::string&)>;

public:
	WebSite(const std::string& url,
			const std::string& searchText,
			ChangeStatusCallback callback);

	static bool isValidHRef(const std::string& str);

	bool load();
	bool parse();
	void searchText();

	Children& getChildren();

	const std::string& getUrl() const;

protected:
	void parse(const rapidxml::xml_node<>* node);
	void callCallback();
	void removeHtmlComments();

private:
	ChangeStatusCallback mChangeStatusCallback;

	std::string mSearchText;

	std::string mUrl;
	std::string mData;
	std::string mParsedData;

	WebSiteStatus mStatus;
	std::string mErrorDescripton;

	Children mChildren;
};