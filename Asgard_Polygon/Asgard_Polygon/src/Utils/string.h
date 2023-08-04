#pragma once

#include <cstdint>

const char* va(const char* fmt, ...);

std::vector<std::string> split(const std::string& s, char delim);
std::vector<std::string> split_lines(const std::string& s);

std::string to_lower(std::string text);
std::string to_upper(std::string text);
bool starts_with(const std::string& text, const std::string& substring);
bool ends_with(const std::string& text, const std::string& substring);

std::string dump_hex(const std::string& data, const std::string& separator = " ");

std::string convert(const std::wstring& wstr);
std::wstring convert(const std::string& str);

std::string replace(std::string str, const std::string& from, const std::string& to);

bool find_lower(const std::string& a, const std::string& b);