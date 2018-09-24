#pragma once

#include <string>

namespace toni
{
std::string base64_encode(char const* bytes_to_encode,
                          size_t len);
std::string base64_decode(std::string const& s);
}