#ifndef HASH_H
#define HASH_H

#include<bits/stdc++.h>
// FNV-1a 64位哈希算法的初始值
#define FNV1A_64_INIT 0xcbf29ce484222325ULL
// FNV-1a 64位哈希算法的质数
#define FNV1A_64_PRIME 0x100000001b3

uint64_t fnv1a_64(const void *data, size_t length) 
{
    uint64_t hash = FNV1A_64_INIT;
    const uint8_t *byte_data = (const uint8_t *)data; 
    for (size_t i = 0; i < length; i++) {
        hash ^= byte_data[i];
        hash *= FNV1A_64_PRIME;
    }
     return hash;
}

/*
const char *str = "Hello, World!";
size_t length = strlen(str);
uint64_t hash = fnv1a_64(str, length);
std::cout << "Hash of \"" << str << "\": " << hash << std::endl;
*/

#endif