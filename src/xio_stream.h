//
// Created by dingrui on 1/3/26.
//

#pragma once

#include <list>
#include <memory>
#include <memory_resource>
#include <thread>

#include "x_data.h"

class XIOStream {
public:
    void Start();

    void Wait();

    void Stop() { is_exit = true; };

    void set_mem_pool(std::shared_ptr<std::pmr::memory_resource> mem_pool) { this->mem_pool_ = mem_pool; };
    void set_next(std::shared_ptr<XIOStream> next) { this->next_ = next; }

    void PushBack(std::shared_ptr<XData> data);

    std::shared_ptr<XData> PopFront();

protected:
    virtual void StartImpl() =0;

protected:
    bool is_exit{false};
    long long data_byte_{0};
    std::shared_ptr<std::pmr::memory_resource> mem_pool_;
    std::shared_ptr<XIOStream> next_;

private:
    std::thread th_;
    std::mutex mutex_;
    std::list<std::shared_ptr<XData> > datas_;
};
