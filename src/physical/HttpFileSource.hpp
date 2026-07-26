#pragma once

#include "IFileSource.hpp"

namespace physical {

  class HttpFileSource : public IFileSource {
  public:
      ByteBuffer load(const std::string& uri) override {
          // Placeholder implementation. Specialize or extend this class to support HTTP/HTTPS transports.
          // A real implementation would use an HTTP client library to fetch the resource and return the bytes.
          throw std::runtime_error("HttpFileSource is not implemented: " + uri);
      }
  };

}