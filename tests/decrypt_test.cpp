//
// Created by dingrui on 1/8/26.
//

#include "../src/x_crypt_task.h"
#include "../src/x_read_task.h"
#include "../src/x_write_task.h"

#include <iostream>
#include <memory>
#include <memory_resource>

int main()
{
    std::string                                pwd = "12345678";
    std::shared_ptr<std::pmr::memory_resource> memPool(new std::pmr::synchronized_pool_resource);
    std::string                                inFilePath  = "asset/tmp_encrypt.txt";
    std::string                                outFilePath = "asset/tmp_decrypt.txt";

    std::shared_ptr<XReadTask> r_task = std::make_shared<XReadTask>();
    r_task->Init(inFilePath);
    r_task->set_mem_pool(memPool);

    std::shared_ptr<XCryptTask> c_task = std::make_shared<XCryptTask>();
    c_task->Init(pwd);
    c_task->set_mem_pool(memPool);
    c_task->set_is_encrypt(false);

    std::shared_ptr<XWriteTask> w_task = std::make_shared<XWriteTask>();
    w_task->set_mem_pool(memPool);
    w_task->Init(outFilePath);

    r_task->set_next(c_task);
    c_task->set_next(w_task);

    r_task->Start();
    c_task->Start();
    w_task->Start();

    r_task->Wait();
    c_task->Wait();
    w_task->Wait();
    return 0;
}
