#pragma once

#include <iterator>
#include <fstream>
#include <sstream>
#include <ctime>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Request.hpp"
#include "Response.hpp"
#include "Protocol.hpp"
#include "SocketManager.hpp"
#include "Configuration.hpp"

enum ResponseError
{
	kNoError,
	kFileOpenError,
	kFilestreamError
};

struct Client
{
	status_code	status_code;
	struct ClientSocket	*client_socket;
	const struct ConfigurationQueryResult	*config;
	struct stat	stat_buff;
	std::string path;
	std::vector<std::string> cgi_argv; //path to cgi executable and path to cgi script
	std::vector<std::string> cgi_env;
	bool	keepAlive;
	std::string location_created;
	std::string cgi_content_type;
	std::string cgi_content_length;
	int chunked_body_size = -1;
	Request	req;
	Response	res;
};

namespace client_lifespan
{
	void	InitClient(struct Client &client, struct ClientSocket *client_socket);
	void	DeleteClientFromVector(std::vector<struct Client> &clients, int client_fd);
	void	ResetClient(struct Client &client);

	//update functions
	void	UpdateStatusCode(struct Client &clt, status_code statuscode);
	void	CheckHeaderBeforeProcess(struct Client *clt);
	bool	IsClientAlive(struct Client &clt);

	//helper functions
	struct Client	*GetClientByFd(std::vector<struct Client> &clients, int client_fd);
};

namespace process
{

	void	ProcessRequest(struct Client *clt);
	void	ProcessGetRequest(struct Client *clt);
	void	ProcessPostRequest(struct Client *clt);
	void	ProcessDeleteRequest(struct Client *clt);



	//file and path and content-type related functions
	std::string GetExactPath(const std::string root, std::string match_path, const struct Uri uri);
	bool		IsCgi(std::vector<std::string> &cgi_executable, std::string path, cache::LocationQuery *location);
	std::string	GetReqExtension(std::string path);
	bool		IsAcceptable(std::string content_type, HeaderValue *accept, cache::LocationQuery *location);
	std::string	GetIndexPath(std::string path, cache::LocationQuery *location);
	bool		IsSupportedMediaType(std::string req_content_type, const directive::MimeTypes* mime_types);
	bool		IsDirFormat(std::string path);

	namespace file
	{
		bool	ModifyFile(struct Client *clt);
		bool	UploadFile(struct Client *clt);
		bool	DeleteFile(struct Client *clt);

		//helper functions
		std::string	GenerateFileName(std::string path); //base on timestamp
		std::string GenerateFileExtension(std::string content_type, const directive::MimeTypes* mime_types);
		bool CreateDir(std::string dir);
		bool CreateDirRecurs(std::string path);
	}
}

namespace cgi
{
	void	ProcessGetRequestCgi(struct Client *clt);
	void	ProcessPostRequestCgi(struct Client *clt);

	enum PipeEnd
	{
		kRead,
		kWrite
	};
	struct CgiOutput
	{
		std::string content_type;
		std::string	content_body;
	};
	int		SetPipes(int *cgi_input, int *cgi_output, const Method method);
	void	SetCgiEnv(struct Client *clt);
	int		ReadAll(int fd, std::string &response_tmp);
	int		WriteAll(int fd, char *cstr_buf, int size);
	bool	ParseCgiOutput(struct CgiOutput &cgi_output, std::string &response_tmp);

	//helper functions
	// std::vector<char *>	ConstructExecArray(std::vector<std::string> &cgi_params);
	char**	StringVecToTwoDimArray(const std::vector<std::string> &strings);
}

namespace res_builder
{
	void	GenerateErrorResponse(struct Client *clt);
	void	GenerateRedirectResponse(struct Client *clt);
	void	GenerateAutoindexResponse(struct Client *clt);
	void	GenerateSuccessResponse(struct Client *clt);

	// error page related helper functions
	void	BuildErrorHeaders(struct Client *clt);
	const std::string &BuildErrorPage(enum status_code code);

	// redirect related helper functions

	// autoindex related helper functions

	// success related helper functions

	// general utility functions
	void	BuildContentHeaders(struct Client *clt);
	void	ServerError(struct Client *clt);
	std::string	GetTimeGMT();
	void	BuildBasicHeaders(Response *res);
	void	BuildStatusLine(enum status_code status_code, std::string &response);
	enum ResponseError	ReadFileToString(const std::string &path, std::string &body);
	const std::string	&StatusCodeAsString(enum status_code code);
}
