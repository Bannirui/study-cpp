//
// Created by dingrui on 1/6/26.
//

#include "x_crypt_task.h"

#include <iostream>

#include "x_crypt.h"

void XCryptTask::Init(std::string& password)
{
    this->enc_ = std::make_shared<XCrypt>();
    this->enc_->Init(password);
}

void XCryptTask::StartImpl()
{
    std::cout << "XCryptTask::StartImpl() start" << std::endl;
    while (!this->is_exit)
    {
        std::shared_ptr<XData> in = PopFront();
        if (!in)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        const bool isLastBlock = in->get_is_last_block();
        const int  inSize      = in->size();
        std::cout << "crypt task, get data size " << inSize << " bytes, last=" << isLastBlock << std::endl;
        std::shared_ptr<XData> out        = XData::Make(mem_pool_);
        int                    maxOutSize = inSize + (isLastBlock ? this->enc_->GetPadding(inSize) : 0);
        out->New(maxOutSize);

        int outSize = 0;
        if (this->is_encrypt_)
        {
            outSize = this->enc_->Encrypt(static_cast<char*>(in->data()), inSize, static_cast<char*>(out->data()),
                                          isLastBlock);
        }
        else
        {
            outSize = this->enc_->Decrypt(static_cast<char*>(in->data()), inSize, static_cast<char*>(out->data()),
                                          isLastBlock);
        }
        std::cout << "crypt task finish, output size=" << outSize << ", last=" << isLastBlock << std::endl;
        out->set_size(outSize);
        out->set_is_last_block(isLastBlock);
        if (this->next_)
        {
            this->next_->PushBack(out);
        }
        if (isLastBlock)
        {
            break;
        }
    }
    std::cout << "XCryptTask::StartImpl() end" << std::endl;
}
