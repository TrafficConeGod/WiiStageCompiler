#include "DataStream.h"
#include <iostream>

template<typename T>
T* DataStream::PtrAtPos() {
    return (T*)((intptr_t)buf + pos);
}

template<typename T>
DataStream& DataStream::operator<<(const T& val) {
    size += sizeof(T);
    buf = realloc(buf, size);
    T* ptr = PtrAtPos<T>();
    // writing all the bytes manully since x86 is little endian while powerpc is big endian (yes this was a huge headache to figure out)
    const u_char* readBuf = (const u_char*)&val;
    u_char* writeBuf = (u_char*)ptr;
    for (size_t i = 0; i < sizeof(T); i++) {
        writeBuf[(sizeof(T) - 1) - i] = readBuf[i];
    }
    //
    pos += sizeof(T);
    return *this;
}

bool DataStream::IsReadable() {
    return pos < size;
}