#pragma once
#include <cstdint>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace physical {

using ByteBuffer = std::vector<std::uint8_t>;

struct FilePayload {
    std::string uri;
    ByteBuffer data;
};

template <typename Source>
class FileIngestor {
    static_assert(std::is_base_of<IFileSource, Source>::value, "Source must derive from IFileSource");

public:
    explicit FileIngestor(Source source = Source())
        : source_(std::move(source)) {
    }

    FilePayload ingest(const std::string& uri) {
        return FilePayload{uri, source_.load(uri)};
    }

private:
    Source source_;
};

} // namespace physical
