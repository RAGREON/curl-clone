#include <url.h>

namespace Url {
  UrlMeta parseUrl(std::string url) {
    UrlMeta _meta;
    
    // Scheme
    size_t pos = url.find("://");
    if (pos != std::string::npos) {
      _meta.scheme = url.substr(0, pos).c_str();
      url.erase(0, pos + 3);
    }

    // Path
    pos = url.find("/");
    if (pos != std::string::npos) {
      _meta.path = url.substr(pos);
      url.erase(pos);
    } else {
      _meta.path = "/";
    }

    // Port
    pos = url.find(":");
    if (pos != std::string::npos) {
      _meta.host = url.substr(0, pos);
      _meta.port = std::stoi(url.substr(pos + 1));
    }
    else {
      _meta.host = url;
    }

    return _meta;
  }
}