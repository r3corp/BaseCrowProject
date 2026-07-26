#pragma once

#include "FileIngestor.hpp"

namespace physical {
  class IFileSource {
  public:
      virtual ~IFileSource() = default;
      virtual ByteBuffer load(const std::string& uri) = 0;
  };
}