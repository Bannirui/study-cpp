//
// Created by dingrui on 1/3/26.
//

#pragma once

#include <string>

#include<openssl/des.h>

class XCrypt {
public:
    // DES
    bool Init(std::string password);

    int Encrypt(const char *in_data, int in_size, char *out_data, bool is_end = false);

    int Decrypt(const char *in_data, int in_size, char *out_data, bool is_end = false);

    int GetPadding(int dataSize);

private:
    DES_key_schedule key_sch_;
};
