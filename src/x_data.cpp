//
// Created by dingrui on 1/6/26.
//

#include "x_data.h"

#include <iostream>

std::shared_ptr<XData> XData::Make(std::shared_ptr<std::pmr::memory_resource> pool) {
    std::shared_ptr<XData> ptr(new XData());
    ptr->mem_pool_ = pool;
    return ptr;
}

void *XData::New(long long mem_size) {
    if (mem_size <= 0) {
        std::cerr << "XData::New failed, mem_size<=0" << std::endl;
        return nullptr;
    }
    if (!this->mem_pool_) { return nullptr; }
    this->data_ = this->mem_pool_->allocate(mem_size);
    this->mem_size_ = mem_size;
    this->size_ = mem_size;
    return this->data_;
}

XData::~XData() {
    if (!this->data_ || !this->mem_pool_) { return; }
    this->mem_pool_->deallocate(this->data_, this->mem_size_);
    this->data_ = nullptr;
    this->mem_size_ = 0;
    this->size_ = 0;
}
