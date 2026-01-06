//
// Created by dingrui on 1/3/26.
//

#include "xio_stream.h"

#include <mutex>

void XIOStream::Start() {
    th_ = std::thread(&XIOStream::StartImpl, this);
}

void XIOStream::Wait() {
    if (th_.joinable()) {
        th_.join();
    }
}

void XIOStream::PushBack(std::shared_ptr<XData> data) {
    std::unique_lock<std::mutex> lock(this->mutex_);
    this->datas_.push_back(data);
}

std::shared_ptr<XData> XIOStream::PopFront() {
    std::unique_lock<std::mutex> lock(this->mutex_);
    if (this->datas_.empty()) {
        return nullptr;
    }
    auto data = this->datas_.front();
    // clean
    this->datas_.pop_front();
    return data;
}
