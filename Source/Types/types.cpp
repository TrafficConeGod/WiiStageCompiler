#include "types.h"
#include "UserType.h"
#include <sstream>

template<typename T>
void NumberSave(DataStream& stream, std::string str) {
    T num = 0;
    std::stringstream stringStream;
    stringStream << str;
    stringStream >> num;
    stream << num;
}

template<typename T>
void Vector2Save(DataStream& stream, std::string str) {
    std::string xStr;
    std::string yStr;
    size_t commaPos = 0;
    for (size_t i = 0; i < str.size(); i++) {
        char ch = str[i];
        if (ch == ',') {
            str[i] = '\0';
            xStr = &str[0];
            str[i] = ',';
            commaPos = i + 1;
        } else if (ch == ' ') {
            commaPos++;
        }
    }
    yStr = &str[commaPos];
    NumberSave<T>(stream, xStr);
    NumberSave<T>(stream, yStr);
}

void ActorSave(DataStream& stream, std::string str) {
    if (!labelNames.count(str)) {
        std::cout << "No actor with label: " << str << "\n";
        exit(1);
    }
    size_t index = labelNames[str];
    stream << index;
}

void StringSave(DataStream& stream, std::string str) {
    for (size_t i = 0; i < str.size(); i++) {
        stream << str[i];
    }
    stream << '\0';
}

Type* LongType = Type::Create("Long", NumberSave<long>);
Type* ULongType = Type::Create("ULong", NumberSave<ulong>);
Type* IntType = Type::Create("Int", NumberSave<int>);
Type* UIntType = Type::Create("UInt", NumberSave<uint>);
Type* ShortType = Type::Create("Short", NumberSave<short>);
Type* UShortType = Type::Create("UShort", NumberSave<ushort>);
Type* CharType = Type::Create("Char", NumberSave<char>);
Type* UCharType = Type::Create("UChar", NumberSave<u_char>);

Type* FloatType = Type::Create("Float", NumberSave<float>);
Type* DoubleType = Type::Create("Double", NumberSave<double>);

Type* Vector2FloatType = Type::Create("Vector2Float", Vector2Save<float>);
Type* Vector2IntType = Type::Create("Vector2Int", Vector2Save<int>);

Type* StringType = Type::Create("String", StringSave);

Type* ActorType = Type::Create("Actor", ActorSave);