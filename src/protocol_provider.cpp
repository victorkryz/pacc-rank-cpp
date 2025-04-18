#include "protocol_provider.h"
#include "downloader.h"
#include <exception>

namespace fs = std::filesystem;

std::ifstream ProtocolProvider::obtain_content()
{
    static const std::string protocol_cache_directory = "cache";
    static const std::string protocol_cache_file = "protocol.txt";

    fs::path cache_directory_path(protocol_cache_directory),
        cache_full_path(protocol_cache_directory);
    cache_full_path /= protocol_cache_file;
    bool do_download(true);

    if (!fs::exists(cache_directory_path) ||
        !fs::is_directory(cache_directory_path))
    {
        if (!fs::create_directory(cache_directory_path))
            throw std::runtime_error("Cannot create cache directory");
    }

    if (fs::exists(cache_full_path) &&
        fs::is_regular_file(cache_full_path) &&
        (0 != fs::file_size(cache_full_path)))
    {
        do_download = false;
        output_dev_ << "Protocol file found in " << cache_directory_path << std::endl;
    }

    if (do_download)
    {
        if (!download_protocol(cache_full_path))
            throw std::runtime_error("Cannot download protocol file");
    }

    std::ifstream protocol_fs;
    protocol_fs.open(cache_full_path, std::ios_base::app);

    if (!protocol_fs)
        throw std::runtime_error("Cannot open protocol file");

    return protocol_fs;
}

bool ProtocolProvider::download_protocol(fs::path& cache_full_path)
{
    std::ofstream protocol_fs(cache_full_path);

    output_dev_ << "Protocol is being downloaded into "
                << cache_full_path << "..." << std::endl;

    downloader::FileDownloader fd;
    bool result = fd.download_file(remote_reference_, protocol_fs);

    protocol_fs.flush();
    protocol_fs.close();

    return result;
}