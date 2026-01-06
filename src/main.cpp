#include <iostream>

#include"x_read_task.h"
#include "x_crypt.h"
#include "x_crypt_task.h"

int main() {
    std::shared_ptr<std::pmr::memory_resource> memPool(new std::pmr::synchronized_pool_resource);
    std::string pwd = "12345678";

    std::shared_ptr<XReadTask> r_task = std::make_shared<XReadTask>();
    r_task->Init("asset/tmp.txt");
    r_task->set_mem_pool(memPool);

    std::shared_ptr<XCryptTask> c_task = std::make_shared<XCryptTask>();
    c_task->Init(pwd);
    c_task->set_mem_pool(memPool);

    r_task->set_next(c_task);

    r_task->Start();
    c_task->Start();

    c_task->Wait();
    r_task->Wait();

    return 0;
}
