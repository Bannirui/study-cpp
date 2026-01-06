//
// Created by dingrui on 1/3/26.
//

#pragma once

#include <memory>
#include <memory_resource>
#include <thread>

class XIOStream {
public:
    void Start();

    void Wait();

    void Stop() { is_exit = true; };

    void set_mem_pool(std::shared_ptr<std::pmr::memory_resource> mem_pool) { this->mem_pool_ = mem_pool; };

protected:
    virtual void StartImpl() =0;

protected:
    bool is_exit{false};
    long long data_byte_{0};

protected:
    std::shared_ptr<std::pmr::memory_resource> mem_pool_;

private:
    std::thread th_;
};
