//
// Created by dingrui on 1/3/26.
//

#include "x_crypt.h"

#include <iostream>
#include <string.h>
#include <unistd.h>


bool XCrypt::Init(std::string password) {
    const_DES_cblock key = {0};
    size_t key_size = password.size();
    if (key_size > sizeof(key)) {
        key_size = sizeof(key);
    }
    memcpy(key, password.c_str(), key_size);
    DES_set_key(&key, &key_sch_);
    return true;
}

int XCrypt::Encrypt(const char *in_data, int in_size, char *out_data, bool is_end) {
    if (!in_data || in_size <= 0 || !out_data) {
        return 0;
    }
    int write_size = 0;
    const_DES_cblock in = {0};
    DES_cblock out = {0};
    const int block_size = sizeof(const_DES_cblock);
    int data_size = 0;
    int padding = block_size - (in_size % block_size);
    for (int i = 0; i < in_size; i += block_size) {
        if (in_size - i < block_size) {
            data_size = in_size - i;
        } else {
            data_size = block_size;
        }
        memcpy(in, in_data + write_size, data_size);
        if (is_end && i + block_size >= in_size) {
            if (padding == block_size) {
                // xxxxxxxx88888888
                DES_ecb_encrypt(&in, &out, &key_sch_, DES_ENCRYPT);
                memcpy(out_data + write_size, &out, block_size);
                write_size += block_size;
                memset(in, padding, sizeof(int));
            } else {
                // xxxxxxx1
                // xxxxxx22
                // xxxxx333
                // xxxx4444
                // xxx55555
                // xx666666
                // x7777777
                memset(in + (in_size % block_size), padding, padding);
            }
        }
        DES_ecb_encrypt(&in, &out, &key_sch_, DES_ENCRYPT);
        memcpy(out_data + write_size, &out, block_size);
        write_size += block_size;
    }
    return write_size;
}

int XCrypt::Decrypt(const char *in_data, int in_size, char *out_data, bool is_end) {
    if (!in_data || in_size <= 0 || !out_data) {
        return 0;
    }
    int write_size = 0;
    const_DES_cblock in = {0};
    DES_cblock out = {0};
    const int block_size = sizeof(const_DES_cblock);
    int data_size = 0;
    for (int i = 0; i < in_size; i += block_size) {
        memcpy(in, in_data + write_size, block_size);
        DES_ecb_encrypt(&in, &out, &key_sch_, DES_DECRYPT);
        data_size = block_size;
        if (is_end && in_size - i <= block_size) {
            data_size = block_size - out[7];
            if (data_size == 0) {
                break;
            }
            if (data_size < 0)
            {
                std::cerr << "Decrypt failed, padding size is error" << std::endl;
                break;
            }
        }
        memcpy(out_data + write_size, &out, data_size);
        write_size += data_size;
    }
    return write_size;
}

int XCrypt::GetPadding(int dataSize) {
    const int block_size = sizeof(const_DES_cblock);
    int padding = block_size - (dataSize % block_size);
    if (padding == 0) {
        padding = block_size;
    }
    return padding;
}
