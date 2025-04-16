#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include "content_provider.h"

class ProtocolProvider : public ContentProvider
{
public:
    ProtocolProvider() {};
    ProtocolProvider(const std::string &remote_reference, std::ostream &output_dev) : 
                                                    remote_reference_(remote_reference),
                                                    output_dev_(output_dev) {};
    ~ProtocolProvider() override = default;

    std::ifstream obtain_content() override;

private:
    bool download_protocol(std::filesystem::path& cache_file_path);

private:
    std::string remote_reference_;
    std::ostream& output_dev_ = std::cout;
};
