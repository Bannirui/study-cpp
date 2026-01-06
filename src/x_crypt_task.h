//
// Created by dingrui on 1/6/26.
//

#pragma once

#include "xio_stream.h"

class XCrypt;

class XCryptTask : public XIOStream {
public:
    void Init(std::string &password);;

private:
    void StartImpl() override;

private:
    std::shared_ptr<XCrypt> enc_;
};
