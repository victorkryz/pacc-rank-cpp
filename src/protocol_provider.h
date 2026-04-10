#pragma once

#include "content_provider.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

class CurlHolder;

class ProtocolProvider : public ContentProvider
{
public:
    ProtocolProvider(const std::string& remote_reference, std::ostream& output_dev);
    std::ifstream obtain_content() override;

private:
    bool download_protocol(std::filesystem::path& cache_file_path);

private:
    std::shared_ptr<CurlHolder> curl_;
    std::string remote_reference_;
    std::ostream& output_dev_;
};
