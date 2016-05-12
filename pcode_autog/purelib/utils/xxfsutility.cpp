#include "nsconv.h"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#else
#define O_BINARY 0
#endif
#include "xxfsutility.h"
#include "nsconv.h"
USING_NS_PURELIB;

long fsutil::get_file_size(const char *path)
{
    long filesize = -1;
    struct stat statbuff;
    if (stat(path, &statbuff) < 0){
        return filesize;
    }
    else{
        filesize = statbuff.st_size;
    }
    return filesize;
}

long fsutil::get_file_size(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    if(length != 0)
    {
        fseek(fp, 0, SEEK_SET);
    }
    return length;
}

std::string fsutil::read_file_data(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if(fp == nullptr) 
        return (char*)"";

    size_t size = get_file_size(fp);

    std::string storage(size, '\0');

    size_t bytes_readed = fread(&storage.front(), 1, size, fp);

    fclose(fp);
    if (bytes_readed < size)
        storage.resize(bytes_readed);
    return std::move(storage);
}

/// Read data, store to std::string
void fsutil::read_file_data(const char *filename, std::string& data)
{
    using namespace std;

    // Open stream
    std::ifstream stream(filename, ios::binary);
    if (!stream)
        throw runtime_error(std::string("cannot open file ") + filename);
    stream.unsetf(ios::skipws);

    // Determine stream size
    stream.seekg(0, ios::end);
    size_t size = stream.tellg();
    stream.seekg(0);   

    // Load data and add terminating automatically by std::string
    data.resize(size);
    stream.read(&data.front(), static_cast<streamsize>(size));
}

std::string fsutil::read_file_data_ex(const char* filename, size_t align)
{
    FILE* fp = fopen(filename, "rb");
    if(fp == nullptr) 
        return (char*)"";

    size_t size = get_file_size(fp);

    std::string storage(size + align, '\0');

    size_t bytes_readed = fread(&storage, 1, size, fp);
    fclose(fp);

    size_t padding_size = 0;
    if(align > 0) {
        padding_size = align - size % align;
        for(size_t offst = 0; offst < padding_size; ++offst) 
        {
            *(&storage.front() + bytes_readed + offst) = padding_size;
        }
    }

    storage.resize(bytes_readed + padding_size);
    
    return std::move(storage);
}

/// Read data and add terminating 0
void fsutil::read_file_data_ex(const char *filename, std::string& data, size_t align)
{
    // Open stream
    std::ifstream stream(filename, std::ios::binary);
    if (!stream)
        throw std::runtime_error(std::string("cannot open file ") + filename);
    stream.unsetf(std::ios::skipws);

    // Determine stream size
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    stream.seekg(0);   

    // Calc Padding size
    size_t padding_size = 0;
    if(align > 0)
        padding_size = align - size % align;

    // Load data
    data.resize(size + padding_size + 1);
    stream.read(&data.front(), static_cast<std::streamsize>(size));

    // Padding data
    for(size_t offst = 0; offst < padding_size; ++offst) 
    {
        *(&data.front() + size + offst) = padding_size;
    }

    //  And add terminating
    data[size] = 0;
}

/// Read data and add terminating 0
void fsutil::read_file_data_ex(const char *filename, std::vector<char>& data, size_t align)
{
    // Open stream
    std::ifstream stream(filename, std::ios::binary);
    if (!stream)
        throw std::runtime_error(std::string("cannot open file ") + filename);
    stream.unsetf(std::ios::skipws);

    // Determine stream size
    stream.seekg(0, std::ios::end);
    size_t size = stream.tellg();
    stream.seekg(0);   

    // Calc Padding size
    size_t padding_size = 0;
    if(align > 0) 
        padding_size = align - size % align;

    // Load data and add terminating 0
    data.resize(size + 1);
    stream.read(&data.front(), static_cast<std::streamsize>(size));

    // Padding data
    for(size_t offst = 0; offst < padding_size; ++offst) 
    {
        *(&data.front() + size + offst) = padding_size;
    }

    data[size] = 0;
}

void fsutil::read_file_data(const char *filename, std::vector<char>& output)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == nullptr)
        return ;

    size_t size = get_file_size(filename);

    output.resize(size);
    size_t bytes_readed = fread(&output.front(), 1, size, fp);

    fclose(fp);
}

void  fsutil::read_file_data_as_string(const char *filename, std::vector<char>& output)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == nullptr)
        return;

    size_t size = get_file_size(fp);

    output.resize(size + 1);
    size_t bytes_readed = fread(&output.front(), 1, size, fp);
	output[bytes_readed] = '\0';
    // output.resize(bytes_readed);

    fclose(fp);
}

void  fsutil::read_file_data_as_string(const char *filename, std::string& output)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == nullptr)
        return;

    size_t size = get_file_size(filename);

    output.resize(size);
    size_t bytes_readed = fread(&output.front(), 1, size, fp);

    fclose(fp);
}

void fsutil::write_file_data(const char* filename, const char* data, size_t size)
{
    FILE* fp = fopen(filename, "wb+");
    if(fp == nullptr)
        return;
    fwrite(data, size, 1,  fp);
    fclose(fp);
}

bool fsutil::exists(const char* filename)
{
    return 0 == access(filename, 0);
}

bool  fsutil::is_type_of(const std::string& filename, const char* type)
{
    size_t off = filename.find_last_of('.');
    if(off != filename.npos)
    {
        std::string ext = filename.substr(off);
        return ext == type;
    }
    return false;
}

bool  fsutil::is_type_of_v2(const std::string& filename, const char* type)
{
    static std::string alltype = "*.*";
    size_t off = filename.find_last_of('.');
    if(off != filename.npos)
    {
        std::string ext = "*" + filename.substr(off); // contains dot
        return ext == type || type == alltype;
    }
    else if(!filename.empty()){
        return type == alltype;
    }
    else {
        return false;
    }
}

void  fsutil::mkdir(const char* _Path)
{
    std::string dir = _Path;
    nsc::replace(dir, "\\", "/");
    std::vector<std::string> results = nsc::split(dir.c_str(), '/');

    dir = "";
    for(auto iter = results.begin(); iter != results.end(); ++iter)
    {
        dir.append(*iter + "/");
        if (!fsutil::exists(dir.c_str()))
        {
#ifdef _WIN32
            ::mkdir(dir.c_str());
#else
            ::mkdir(dir.c_str(), 0);
#endif
        }
    }
}

std::pair<std::string, std::string> fsutil::split_fullpath(const std::string& fullpath)
{
    std::string tmp = fullpath;
    std::pair<std::string, std::string> pr;
    nsc::replace(tmp, "\\", std::string("/"));
    size_t pos = tmp.find_last_of("/");
    if(pos != std::string::npos) {
        try {
            pr.first = tmp.substr(0, pos);
            pr.second = tmp.substr(pos + 1);
        }
        catch(...) {
        }
    }
    return pr;
}

std::string fsutil::get_short_name(const std::string& complete_filename)
{
    size_t pos = complete_filename.find_last_of("\\");

    if(pos == std::string::npos)
        pos = complete_filename.find_last_of("/");

    if(pos != std::string::npos) {
        try {
            return complete_filename.substr(pos + 1);
        }
        catch(...) {
        }
    }
    return "";
}

std::string fsutil::get_path_of(const std::string& complete_filename)
{
    size_t pos = complete_filename.find_last_of("\\");

    if(pos == std::string::npos)
        pos = complete_filename.find_last_of("/");

    if(pos != std::string::npos) {
        try {
            return complete_filename.substr(0, pos);
        }
        catch(...) {
        }
    }
    return "";
}

const char* fsutil::get_extend(const char* source)
{
    int len = strlen(source);

    for (len = len - 1; len >= 0; --len)
    {
        if (source[len] == '.')
            return source + len;
    }

    return "";
}

const char* fsutil::get_extend(const std::string& filename)
{
    auto dot = filename.rfind('.');
    if (dot != std::string::npos)
    {
        return &filename.front() + dot;
    }
    return "";
}

std::string fsutil::get_type(const std::string& filename)
{
	static std::string _s_wildcard = "*";
	return _s_wildcard + fsutil::get_extend(filename);
}

#if defined( _WIN32 ) && !defined(WP8)
#include <ShlObj.h>
bool fsutil::get_open_filename(TCHAR* output, const TCHAR* filters, TCHAR* history, HWND hwndParent)
{
    OPENFILENAME ofn;
    memset(&ofn,0,sizeof(OPENFILENAME));
    memset(output,0,sizeof(output));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = filters;
    ofn.lpstrFile = output;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = history;
    ofn.hwndOwner = hwndParent;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
    return GetOpenFileName(&ofn);
}


TCHAR* fsutil::browse_folder(TCHAR* pBuffer, TCHAR* history, HWND   hWnd)
{  
    BROWSEINFO   bf;  
    LPITEMIDLIST   lpitem;  

    memset(&bf, 0, sizeof BROWSEINFO);  

    bf.hwndOwner = hWnd;  

    bf.lpszTitle = TEXT("Ñ¡ÔñÄ¿Â¼");  

    bf.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

    bf.lParam = (LPARAM)history;

    bf.lpfn = _BrowseCallbackProc;

    lpitem = SHBrowseForFolder(&bf);  

    if(lpitem == NULL) 
        return   TEXT("");  

    SHGetPathFromIDList(lpitem, pBuffer);  

    CoTaskMemFree(lpitem);

    return    pBuffer;
}  

INT CALLBACK fsutil::_BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData) 
{
    TCHAR szDir[MAX_PATH];
    switch(uMsg) 
    {
    case BFFM_INITIALIZED: 
        // WParam is TRUE since you are passing a path.
        // It would be FALSE if you were passing a pidl.
        SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pData);
        break;
    case BFFM_SELCHANGED: 
        // Set the status window to the currently selected path.
        if (SHGetPathFromIDList((LPITEMIDLIST)lParam ,szDir))
        {
            SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
        }
        break;
    }
    return 0;
}


void fsutil::search_files(const std::string& root, const TCHAR* dir, std::vector<serach_file_info>& file_list, bool recursive, const char* filter)
{
    //srcFiles = (char *)malloc(FILEEMAX * sizeof(char));
    WIN32_FIND_DATA fd;
    ZeroMemory(&fd, sizeof(WIN32_FIND_DATA));

    HANDLE hFile;
    TCHAR tmpPath[256];
    TCHAR subPath[256];
    ZeroMemory(tmpPath, 256);
    ZeroMemory(subPath, 256);

    BOOL bRet = TRUE;
    static int nFileSize = 0;
    bool has_filter = (strlen(filter) != 0);

    //define the format of the basepath
    lstrcpy(tmpPath, dir);
    if (tmpPath[lstrlen(tmpPath) - 1] != '\\')
    {
        lstrcat(tmpPath, L"\\");
    }
    lstrcat(tmpPath, L"*");

    hFile = FindFirstFile(tmpPath, &fd);

    while (hFile != INVALID_HANDLE_VALUE && bRet)
    {
        if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY &&
            lstrcmp(fd.cFileName, L".") && lstrcmp(fd.cFileName, L"..") && recursive)
        {
            lstrcpy(subPath, dir);
            if (subPath[lstrlen(subPath) - 1] != '\\')
            {
                lstrcat(subPath, L"\\");
            }
            lstrcat(subPath, fd.cFileName);
            search_files(root, subPath, file_list, recursive, filter);
        }
        else if (!lstrcmp(fd.cFileName, L".") || !lstrcmp(fd.cFileName, L".."))
        {
        }
        else
        {
            if (/*fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY
                && fd.dwFileAttributes != FILE_ATTRIBUTE_HIDDEN
                && fd.dwFileAttributes != FILE_ATTRIBUTE_SYSTEM
                && fd.dwFileAttributes != 0x16 */
                !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                !(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
                !(fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
                )
            {

                std::string tp = ::nsc::transcode(tmpPath);
                size_t pos = tp.find_first_of(root);

                std::string tree = tp.substr(pos + 1 + root.size());
                tree.pop_back();

                serach_file_info sfi;
                sfi.filename = nsc::transcode(fd.cFileName);
                sfi.subtree = tree;

                //if (!(fsutil::is_type_of_v2(sfi.filename, "*.apk")
                //    || fsutil::is_type_of_v2(sfi.filename, "*.ttf")
                //    || fsutil::is_type_of_v2(sfi.filename, "*.mp3")
                //    || fsutil::is_type_of_v2(sfi.filename, "*.wav")
                //    || fsutil::is_type_of_v2(sfi.filename, "*.txt")
                //    || fsutil::is_type_of_v2(sfi.filename, "*.log")))
                //{ // except apk and ttf
                if (!has_filter)
                {
                    file_list.push_back(sfi);
                }
                else {
                    if (fsutil::is_type_of_v2(sfi.filename, filter))
                    {
                        file_list.push_back(sfi);
                    }
                }
                /*}*/
            }
        }
        bRet = FindNextFile(hFile, &fd);
    }

    FindClose(hFile);
}

#endif


