#pragma once
#include <string>
#include <sstream>
class local_binarybuf
{
public:
    local_binarybuf(size_t buffersize = 256);
    local_binarybuf(const local_binarybuf& right);
    local_binarybuf(local_binarybuf&& right);
    ~local_binarybuf();

    local_binarybuf& operator=(const local_binarybuf& right);
    local_binarybuf& operator=(local_binarybuf&& right);
    
    template<typename _Nty>
    size_t write_i(const _Nty value);

    template<typename _Nty>
    _Nty read_i();

    template<typename _Nty>
    _Nty& read_i(_Nty& ov);

    void write_v(const std::string&);
    std::string read_v();

    void write_binarybuf(const local_binarybuf&);
    local_binarybuf read_binarybuf(void);

    template<typename _Nty>
    void modify_i(std::streamoff offset, const _Nty value);

public:
    void load(const char* filename);
    void save(const char* filename);
    size_t reset_rd(); // reset read pointer
protected:
    std::string    buffer_impl_;
    size_t         rdptr_;
};

template <typename _Nty>
inline size_t local_binarybuf::write_i(const _Nty value)
{
    size_t offset = buffer_impl_.size();
    buffer_impl_.append((const char*)&value, sizeof(value));
    return offset;
}

template <typename _Nty>
inline _Nty local_binarybuf::read_i()
{
    auto pvalue = (_Nty*)(buffer_impl_.c_str() + rdptr_);
    rdptr_ += sizeof(_Nty);
    return *pvalue;
}

template <typename _Nty>
inline _Nty & local_binarybuf::read_i(_Nty & ov)
{
    // TODO: insert return statement here
    ov = *( (_Nty*)(buffer_impl_.c_str() + rdptr_) );
    rdptr_ += sizeof(_Nty);
    return ov;
}

template <typename _Nty>
inline void local_binarybuf::modify_i(std::streamoff offset, const _Nty value)
{
    auto pvalue = (_Nty*)(buffer_impl_.c_str() + offset);
    *pvalue = value;
}
