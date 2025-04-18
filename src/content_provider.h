#pragma once

#include <fstream>

struct ContentProvider
{
    virtual ~ContentProvider() = default;
    virtual std::ifstream obtain_content() = 0;
};