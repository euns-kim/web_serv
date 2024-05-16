#include "Client.hpp"

void	res_builder::BuildErrorHeaders(struct Client *clt)
{
	// different headers for different status codes
	switch (clt->status_code)
	{
		case k405:
			// clt->config->query->allowed_methods
			// clt->res->addNewPair("Allow", new HeaderSomething());
			break ;
		case k406:
			//not sure what to do here
			break ;
		case k408:
			clt->res->addNewPair("Connection", new HeaderString("close"));
			break ;
		case k413:
			clt->res->addNewPair("Connection", new HeaderString("close"));
		case k415:
			// unsupported media types
			// clt->res->addNewPair("Accept", new HeaderSomething());
			// AND/OR unsupported encoding
			// clt->res->addNewPair("Accept-Encoding", new HeaderSomething());
			// ? should I simply add both headers or find a way to check them seperately
			break ;
	}
}

const std::string &res_builder::BuildErrorPage(enum status_code status_code)
{
	std::string response;

	response = "<html>\r\n";
	response += "<head><title>";
	response += StatusCodeAsString(status_code);
	response += "</title></head>\r\n";
	response += "<body>\r\n";
	response += "<center><h1>";
	response += StatusCodeAsString(status_code);
	response += "</h1></center>\r\n";
	response += "<hr><center>Webserv</center>\r\n";
	response += "</body>\r\n";
	response += "</html>\r\n";

	return (response);
}

void	res_builder::GenerateErrorResponse(struct Client *clt)
{
	// build the status line
	std::string response = clt->client_socket->res_buf;
	BuildStatusLine(clt->status_code, response);

	// build basic and error headers
	BuildBasicHeaders(clt->res); // add basic headers
	// BuildErrorHeaders(clt); // add additional headers according to the error code

	// build the body

	// first search for error pages in the configuration
	// if not found, generate a default error page
	std::vector<const directive::ErrorPage *>	errorPages = clt->config->query->error_pages;
	std::vector<const directive::ErrorPage *>::const_iterator	it;

	Maybe<std::string> result;
	std::string	pathErrorPage;
	std::string body;

	for (it = errorPages.begin(); it != errorPages.end(); ++it)
	{
		result = (*it)->match(clt->status_code);
		if (result.is_ok())
			break ;
	}
	if (it != errorPages.end())
	{
		pathErrorPage = (*it)->file_path();
		if (ReadFileToString(pathErrorPage, body) != kNoError);
		{
			ServerError(clt);
			return ;
		}
	}
	else
	{
		body = BuildErrorPage(clt->status_code);
	}

	// set the body
	clt->res->setResponseBody(body);

	// build content headers
	BuildContentHeaders(clt);

	// add headers to the response
	std::string	headers = clt->res->returnMapAsString();
	if (headers.empty()) // stream error occurred
	{
		ServerError(clt);
		return ;
	}
	response += headers;

	// add body to response
	response += clt->res->getResponseBody();
}
