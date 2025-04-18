#pragma once

#include <fstream>
#include <string>

using CURL = void;

namespace downloader
{
    class FileDownloader
    {
    public:
        FileDownloader();
        ~FileDownloader();

        bool download_file(const std::string& reference, std::ofstream& file);

    protected:
        bool init();
        void uninit();

    private:
        CURL* curl_ = nullptr;
    };

} // namespace