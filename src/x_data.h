//
// Created by dingrui on 1/6/26.
//

#pragma once

#include <memory>
#include <memory_resource>

// memory pool manager
class XData {
public:
    static std::shared_ptr<XData> Make(std::shared_ptr<std::pmr::memory_resource> pool);

    void *New(long long mem_size);

    void set_size(long long s) { size_ = s; };

    ~XData();

private:
    XData() = default;

private:
    void *data_ = nullptr;
    long long size_{0};
    long long mem_size_{0};
    std::shared_ptr<std::pmr::memory_resource> mem_pool_;
};
