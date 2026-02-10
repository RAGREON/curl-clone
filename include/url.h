#pragma  once

#include <string>

namespace Url {
  struct UrlMeta {
    int             port;
    std::string     scheme;
    std::string     host;
    std::string     path;

    UrlMeta() : port(80), scheme("http") { }
  };

  UrlMeta parseUrl(std::string url);
}