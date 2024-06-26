#include "HeaderString.hpp"

HeaderString::HeaderString()
: type_(kString), content_() {}

HeaderString::HeaderString(const std::string &content)
: type_(kString), content_(content) {}

HeaderString::HeaderString(const HeaderString &obj)
: type_(obj.type_), content_(obj.content_) {}

HeaderString::~HeaderString() {}

HeaderString &HeaderString::operator=(const HeaderString &obj)
{
	type_ = obj.type_;
	content_ = obj. content_;
  return *this;
}

const HeaderString::ValueType	&HeaderString::type() const
{
	return (type_);
}

const std::string	&HeaderString::content() const
{
	return (content_);
}

HeaderString	*HeaderString::clone() const
{
	return (new HeaderString(*this));
}

std::string HeaderString::to_string() const
{
	return content_;
}

