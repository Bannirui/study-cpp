//
// Created by dingrui on 1/3/26.
//

#include "x_read_task.h"

#include <iostream>

#include "x_data.h"

bool XReadTask::Init(std::string file_name) {
    if (file_name.empty()) {
        return false;
    }
    ifs_.open(file_name, std::ios::binary);
    if (!ifs_) {
        std::cerr << "Failed to open file " << file_name << std::endl;
        return false;
    }
    std::cout << file_name << " open succ" << std::endl;
    ifs_.seekg(0, std::ios::end);
    data_byte_ = ifs_.tellg();
    ifs_.seekg(0, std::ios::beg);
    std::cout << "file size is " << data_byte_ << std::endl;
    return true;
}

void XReadTask::StartImpl() {
    std::cout << "XReadTask::StartImpl() start" << std::endl;
    while (!is_exit) {
        if (ifs_.eof()) {
            break;
        }
        std::shared_ptr<XData> data = XData::Make(this->mem_pool_);
        int dataSize = 1024;
        void *buf = data->New(dataSize);
        ifs_.read((char *) buf, sizeof(buf));
        if (ifs_.gcount() <= 0) {
            break;
        }
        data->set_size(ifs_.gcount());
        std::cout << "read " << ifs_.gcount() << " bytes, addr is " << buf << std::endl;
        if (this->ifs_.eof()) {
            data->set_end(true);
        }
        if (this->next_) {
            this->next_->PushBack(data);
        }
    }
    std::cout << "XReadTask::StartImpl() end" << std::endl;
}
