#pragma once

#include <map>
#include <iterator>
#include <sstream>
#include "Protocol.hpp"
#include "HeaderValue.hpp"
#include "HeaderValue/HeaderString.hpp"
#include "HeaderValue/HeaderInt.hpp"
#include "HeaderValue/HeaderStringVector.hpp"
#include "Uri/Uri.hpp"

typedef std::pair<std::string, HeaderValue *>	HeaderPair;
typedef std::map<std::string, HeaderValue *>	HeaderMap;
typedef HeaderMap::const_iterator HeaderMapIt;

/*
	An abstract class having member variables and functions
	shared by Request and Response classes */

class HTTPMessage
{
	public:
		HTTPMessage();
		HTTPMessage(const HTTPMessage &obj);
		virtual ~HTTPMessage();

		HTTPMessage &operator=(const HTTPMessage &obj);

		void	addNewPair(std::string key, HeaderValue *value);
		void	addNewPair(HeaderPair pair);

		HeaderValue	*returnValueAsPointer(std::string key) const; // memory managed by this class
		HeaderValue	*returnValueAsClonedPointer(std::string key) const; // should be freed elsewhere
		HeaderPair	returnClonedPair(std::string key) const; // should be freed elsewhere
		std::string	returnMapAsString();
		void	cleanHeaderMap();

	public:
		std::map<std::string, HeaderValue *>	headers_;
};

