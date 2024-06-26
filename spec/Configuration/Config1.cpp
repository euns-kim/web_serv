#include <cstdlib>

#include "constants.hpp"
#include "Configuration.hpp"
#include "Uri/Authority.hpp"
#include "Configuration/Directive/Block/Main.hpp"
#include "Configuration/Directive/Block/Http.hpp"
#include "Configuration/Directive/Block/Server.hpp"
#include "Configuration/Directive/Block/Location.hpp"
#include "Configuration/Directive/Block/Events.hpp"
#include "Configuration/Directive/Simple.hpp"
#include "Configuration/Directive/Simple/Listen.hpp"
#include "Configuration/Directive/Simple/ServerName.hpp"
#include "Configuration/Directive/Simple/Cgi.hpp"
#include "Configuration/Directive/Simple/Return.hpp"

/**
 * events {
 *  worker_connections 512;
 * }
 * 
 * http {
 *   error_log /var/logs/error.log;
 *   access_log /var/logs/access.log;
 *   server {
 *    listen 80;
 *    server_name hi.com wtf.fr;
 *    location / {
 *      root /var/www/html;
 *      index index.html index.htm default.html;
 * 
 *      location /omg/ {
 *        root /var/www/omg;
 *        cgi js /opt/homebrew/bin/node;
 *      }
 *    }
 *  
 *    autoindex on;
 *    location /images/ {
 *      root /var/www/images;
 *      cgi php /usr/local/bin/php4.3;
 *    }
 * 
 *    location /develop {
 *      return 301 http://www.develop.com;
 *    }
 * }
*/
void  Config1(directive::MainBlock& config)
{
  {
    directive::EventsBlock*  events = new directive::EventsBlock();
    config.add_directive(events);
    directive::WorkerConnections*  worker_connections = new directive::WorkerConnections(1024);
    worker_connections->set(512);
    events->add_directive(worker_connections);
  }
  directive::HttpBlock*    http = new directive::HttpBlock();
  config.add_directive(http);

  {
    directive::ErrorLog*  error_log = new directive::ErrorLog();
    error_log->set("/var/logs/error.log");
    http->add_directive(error_log);
  }

  {
    directive::AccessLog*  access_log = new directive::AccessLog();
    access_log->set("/var/logs/access.log");
    http->add_directive(access_log);
  }

  {
    directive::ServerBlock*  server = new directive::ServerBlock();
    http->add_directive(server);

    {
      directive::Listen*  listen = new directive::Listen();
      listen->add(uri::Authority("80"));
      server->add_directive(listen);
    }

    {
      directive::ServerName*  server_name = new directive::ServerName();
      server_name->add("hi.com");
      server_name->add("wtf.fr");
      server->add_directive(server_name);
    }

    {
      directive::LocationBlock*  location = new directive::LocationBlock();
      location->set("/");
      server->add_directive(location);

      {
        directive::Root*  root = new directive::Root();
        root->set("/var/www/html");
        location->add_directive(root);
      }

      {
        // Construction of multiple indexes on the same line
        // has to be done in reverse order.
        directive::Index*  index = new directive::Index();
        index->set("default.html");
        location->add_directive(index);
        index = new directive::Index();
        index->set("index.htm");
        location->add_directive(index);
        index = new directive::Index();
        index->set("index.html");
        location->add_directive(index);
      }
        
      {
        directive::LocationBlock*  location2 = new directive::LocationBlock();
        location2->set("/omg/");
        location->add_directive(location2);

        {
          directive::Root*  root = new directive::Root();
          root->set("/var/www/omg");
          location2->add_directive(root);
        }

        {
          directive::Cgi*  cgi = new directive::Cgi();
          cgi->set("js", "/opt/homebrew/bin/node");
          location2->add_directive(cgi);
        }
      }
    }

    {
      directive::Autoindex*  autoindex = new directive::Autoindex();
      autoindex->set(true);
      server->add_directive(autoindex);
    }

    {
      directive::LocationBlock*  location = new directive::LocationBlock();
      location->set("/images/");
      server->add_directive(location);

      {
        directive::Root*  root = new directive::Root();
        root->set("/var/www/images");
        location->add_directive(root);
      }

      {
        directive::Cgi*  cgi = new directive::Cgi();
        cgi->set("php", "/usr/local/bin/php4.3");
        location->add_directive(cgi);
      }
    }

    {
      directive::LocationBlock*  location = new directive::LocationBlock();
      location->set("/develop");
      server->add_directive(location);

      {
        directive::Return*  return_ = new directive::Return();
        return_->set("http://www.develop.com", 301);
        location->add_directive(return_);
      }
    }
  }
}