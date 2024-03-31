#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "misc/Maybe.hpp"
#include "Configuration/Cache/LocationQuery.hpp"
#include "Configuration/Cache/ServerQuery.hpp"
#include "Configuration/Directive/Socket.hpp"
#include "Configuration/Directive/Block/Server.hpp"
#include "Configuration/Directive/Block/Location.hpp"

class ConfigurationDatabase;
extern ConfigurationDatabase ws_database;

struct ConfigurationQueryResult
{
  const directive::ServerBlock*   server_block;
  const directive::LocationBlock* location_block;
  cache::LocationQuery*           location_property;

  ConfigurationQueryResult();
  ConfigurationQueryResult(const directive::ServerBlock* server_block,
                           const directive::LocationBlock* location_block,
                           cache::LocationQuery* location_property);

  bool  is_empty() const;
};

class ConfigurationDatabase
{
  public:
    typedef Maybe<const std::vector<const directive::ServerBlock*>*>  ServerBlocksQueryResult;

    ConfigurationDatabase();
    ConfigurationDatabase(int cache_size);
    ConfigurationDatabase(const ConfigurationDatabase &other);
    ConfigurationDatabase &operator=(const ConfigurationDatabase &other);
    ~ConfigurationDatabase();

    size_t                                worker_connections() const;
    std::vector<const directive::Socket*> all_server_sockets();
    void                                  register_server_socket(int server_socket_fd,
                                                                 const directive::Socket& socket);

    void                                  set_location_cache_size(int size);

    const ConfigurationQueryResult        query(int server_socket_fd,
                                                const std::string& server_name,
                                                const std::string& path);

    const directive::LocationBlock*       query_location_block(const directive::ServerBlock* server_block,
                                                               const std::string& path) const;
    const directive::ServerBlock*         query_server_block(int server_socket_fd,
                                                             const std::string& server_name) const;
    ServerBlocksQueryResult               query_server_blocks(int server_socket_fd) const;

  private:
    std::vector<cache::ServerQuery>       server_cache_;
    std::vector<cache::LocationQuery>     location_cache_;
    int                                   location_insertion_index_;

    void                                  generate_server_cache();
};
