//
// Created by dingrui on 1/6/26.
//

#include "x_crypt_task.h"

#include <iostream>

#include "x_crypt.h"

void XCryptTask::Init(std::string &password) {
    this->enc_ = std::make_shared<XCrypt>();
    this->enc_->Init(password);
}

void XCryptTask::StartImpl() {
    std::cout << "XCryptTask::StartImpl() start" << std::endl;
    while (!this->is_exit) {
        std::shared_ptr<XData> data = PopFront();
        if (!data) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        std::cout << "crypt task, get data size " << data->size() << std::endl;
        auto out = XData::Make(mem_pool_);
        int out_size = out->size() + this->enc_->GetPadding(data->size());
        out->New(out_size);
        int data_size = this->enc_->Encrypt((char *) data->data(), data->size(), (char *) out->data());
        out->set_size(data_size);
        std::cout << "crypt finish, size " << data_size << std::endl;
        out->set_end(data->end());
        if (this->next_) {
            this->next_->PushBack(out);
        }
        if (data->end()) {
            break;
        }
    }
    std::cout << "XCryptTask::StartImpl() end" << std::endl;
}
