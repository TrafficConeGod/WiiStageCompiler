#include "DataStream.h"
#include <iostream>

template<typename T>
T* DataStream::PtrAtPos() {
    return (T*)((intptr_t)buf + pos);
}

template<typename T>
DataStream& DataStream::operator<<(const T& val) {
    T* ptr = PtrAtPos<T>();
    *ptr = val;
    pos += sizeof(T);
    return *this;
}

template<typename T>
DataStream& DataStream::operator>>(T& val) {
    T* ptr = PtrAtPos<T>();
    val = *ptr;
    pos += sizeof(T);
    return *this;
}

bool DataStream::IsReadable() {
    return pos < size;
}