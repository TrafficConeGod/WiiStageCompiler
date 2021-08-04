#pragma once
#include <iostream>

class DataStream {
    private:
        size_t pos = 0;

        template<typename T>
        inline T* PtrAtPos();
    public:
        void* buf = nullptr;
        size_t size = 0;

        template<typename T>
        DataStream& operator<<(const T& val);

        inline bool IsReadable();
};

#include "DataStream.inl"