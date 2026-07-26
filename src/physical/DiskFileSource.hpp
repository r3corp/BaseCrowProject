#pragma once
#include "FileIngestor.hpp"

namespace physical{

  class DiskFileSource : public IFileSource {
  public:
      ByteBuffer load(const std::string& uri) override {
          std::ifstream stream(uri, std::ios::binary);
          if (!stream) {
              throw std::runtime_error("Failed to open disk resource: " + uri);
          }

          stream.seekg(0, std::ios::end);
          auto size = static_cast<std::streamsize>(stream.tellg());
          stream.seekg(0, std::ios::beg);

          ByteBuffer buffer;
          buffer.reserve(size > 0 ? static_cast<size_t>(size) : 0);
          buffer.insert(buffer.end(), std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
          return buffer;
      }
  };
}