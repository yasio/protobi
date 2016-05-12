#include "local_binarybuf.h"
#include <iostream>
#include <fstream>
#include "xxfsutility.h"

local_binarybuf::local_binarybuf(size_t buffersize)
{
    buffer_impl_.reserve(buffersize);
    rdptr_ = 0;
}

local_binarybuf::local_binarybuf(const local_binarybuf& right) : buffer_impl_(right.buffer_impl_), rdptr_(right.rdptr_)
{

}

local_binarybuf::local_binarybuf(local_binarybuf && right) : buffer_impl_(std::move(right.buffer_impl_)), rdptr_(right.reset_rd())
{
}

local_binarybuf::~local_binarybuf()
{
}

local_binarybuf& local_binarybuf::operator=(const local_binarybuf& right)
{
    buffer_impl_ = right.buffer_impl_;
    rdptr_ = right.rdptr_;
    return *this;
}

local_binarybuf& local_binarybuf::operator=(local_binarybuf&& right)
{
    buffer_impl_ = std::move(right.buffer_impl_);
    rdptr_ = right.reset_rd();
    return *this;
}

void local_binarybuf::write_v(const std::string & value)
{
    auto size = value.size();
    buffer_impl_.append((const char*)&size, sizeof(size));
    if(size != 0)
        buffer_impl_.append(value.c_str(), size);
}

std::string local_binarybuf::read_v()
{
    auto psize = (size_t*)(buffer_impl_.c_str() + rdptr_);
    rdptr_ += sizeof(size_t);
    auto temp = std::string(buffer_impl_.c_str() + rdptr_, *psize);
    rdptr_ += *psize;
    return std::move(temp);
}

void local_binarybuf::write_binarybuf(const local_binarybuf & buf)
{
    auto size = buf.buffer_impl_.size();
    buffer_impl_.append((const char*)&size, sizeof(size));
    if (size != 0)
        buffer_impl_.append(buf.buffer_impl_.c_str(), size);
}

local_binarybuf local_binarybuf::read_binarybuf(void)
{
    local_binarybuf temp;

    auto psize = (size_t*)(buffer_impl_.c_str() + rdptr_);
    rdptr_ += sizeof(size_t);
    temp.buffer_impl_.assign(buffer_impl_.c_str() + rdptr_, *psize);
    rdptr_ += *psize;

    return std::move(temp);
}

void local_binarybuf::load(const char * filename)
{
    fsutil::read_file_data(filename, buffer_impl_);
    reset_rd();
}

void local_binarybuf::save(const char * filename)
{
    fsutil::write_file_data(filename, buffer_impl_.c_str(), buffer_impl_.size());
}

size_t local_binarybuf::reset_rd()
{
    auto offset = rdptr_;
    rdptr_ = 0;
    return offset;
}
