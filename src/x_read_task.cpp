//
// Created by dingrui on 1/3/26.
//

#include "x_read_task.h"

#include <iostream>

#include "x_data.h"

bool XReadTask::Init(const std::string& file_name)
{
    if (file_name.empty())
    {
        return false;
    }
    ifs_.open(file_name, std::ios::binary);
    if (!ifs_)
    {
        std::cerr << "Failed to open file " << file_name << std::endl;
        return false;
    }
    std::cout << file_name << " open succ" << std::endl;
    this->ifs_.seekg(0, std::ios::end);
    this->data_byte_ = this->ifs_.tellg();
    this->ifs_.seekg(0, std::ios::beg);
    std::cout << "file size is " << data_byte_ << " bytes" << std::endl;
    return true;
}

void XReadTask::StartImpl()
{
    std::cout << "XReadTask::StartImpl() start" << std::endl;
    const int dataSize = 1024;
    while (!this->is_exit)
    {
        std::shared_ptr<XData> data = XData::Make(this->mem_pool_);
        void*                  buf  = data->New(dataSize);
        ifs_.read(static_cast<char*>(buf), dataSize);
        std::streamsize n = this->ifs_.gcount();
        if (n <= 0)
        {
            break;
        }
        data->set_size(static_cast<int>(n));
        std::cout << "read " << ifs_.gcount() << " bytes, data is " << (char*)buf << std::endl;
        if (n < dataSize)
        {
            data->set_is_last_block(true);
            std::cout << "read last block" << std::endl;
        }
        if (this->next_)
        {
            this->next_->PushBack(data);
        }
        if (data->get_is_last_block())
        {
            break;
        }
    }
    this->ifs_.close();
    std::cout << "XReadTask::StartImpl() end" << std::endl;
}
