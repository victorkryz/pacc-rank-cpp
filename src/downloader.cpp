#include "downloader.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>

namespace downloader
{

    FileDownloader::FileDownloader() {
        init();
    }

    FileDownloader::~FileDownloader() {
        uninit();
    }

    bool FileDownloader::init()
    {
        curl_ = curl_easy_init();
        return (nullptr != curl_);
    }

    void FileDownloader::uninit()
    {
        if (nullptr != curl_)
            curl_easy_cleanup(curl_);
    }

    bool FileDownloader::download_file(const std::string& reference, std::ofstream& file)
    {
        auto write_callback_ptr = +[](char* contents, size_t size, size_t nmemb, void* userp) -> size_t
        {
            std::ofstream* stream = reinterpret_cast<std::ofstream*>(userp);
            const size_t chunk_sz = size * nmemb;
            stream->write(contents, chunk_sz);
            return chunk_sz;
        };

        bool result = false;
        if (nullptr != curl_)
        {
            curl_easy_setopt(curl_, CURLOPT_URL, reference.c_str());
            curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "GET");
            curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback_ptr);
            curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &file);

            CURLcode res = curl_easy_perform(curl_);
            if (res == CURLE_OK)
                result = true;
            else
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res);
        }

        return result;
    }

} // namespace