//
// Copyright (c) 2014 purelib - All Rights Reserved
//
#ifndef _XXFSUTILITY_H_
#define _XXFSUTILITY_H_
#include <string>
#include <vector>
#ifdef _WIN32
#include <io.h>
#include <direct.h> // for CRT function: access
#else
#include <unistd.h> // for CRT function: access
#include <sys/types.h> // for CRT function: mkdir
#include <sys/stat.h>  // for CRT function: mkdir
#endif
#include <vector>

#include "simple_ptr.h"
#include "unreal_string.h"

#ifdef _WIN32
#include <Windows.h>
#include <shellapi.h>


#endif

#include "aes.h"

namespace fsutil {

long             get_file_size(const char* filename);
long             get_file_size(FILE* fp);

std::string      read_file_data(const char* filename);
void             read_file_data(const char *filename, std::string& data);
void             read_file_data(const char *filename, std::vector<char>& data);

std::string      read_file_data_ex(const char* filename, size_t align = 16);
void             read_file_data_ex(const char *filename, std::string& data, size_t align = 16);
void             read_file_data_ex(const char *filename, std::vector<char>& data, size_t align = 16);
/// Read file data with null-terminated '\0'
void             read_file_data_as_string(const char *filename, std::vector<char>& output);
void             read_file_data_as_string(const char *filename, std::string& output);

void             write_file_data(const char* filename, const char* data, size_t size);

bool             exists(const char* filename);

bool             is_type_of(const std::string& filename, const char* type);

bool             is_type_of_v2(const std::string& filename, const char* type);

void             mkdir(const char* directory);

std::pair<std::string, std::string> split_fullpath(const std::string& fullpath);

std::string      get_short_name(const std::string& complete_filename);
std::string      get_path_of(const std::string& complete_filename);

const char*      get_extend(const char* filename);
const char*      get_extend(const std::string& filename);

/* get extend with wildcard, such as *.png */
std::string      get_type(const std::string& filename);

#ifdef _WIN32

#define OFN_FILTER(tips,filters) TEXT(tips) TEXT("\0") TEXT(filters) TEXT("\0")
#define OFN_FILTER_END()         TEXT("\0")

bool             get_open_filename(TCHAR* output, const TCHAR* filters, TCHAR* history = nullptr, HWND hWnd = nullptr);

TCHAR*           browse_folder(TCHAR* out, TCHAR* history = TEXT(""), HWND hWnd = nullptr);

INT CALLBACK     _BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData);

struct serach_file_info {
    std::string subtree;
    std::string filename;
};

void search_files(const std::string& root, const TCHAR* dir, std::vector<serach_file_info>& file_list, bool recursive = false, const char* filter = "*.*");

#endif /* end of win32 utils */

};

#endif

