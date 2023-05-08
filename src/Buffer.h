#pragma once
#include<string>

class Buffer
{
private:
    std::string buf;
public:
    Buffer(/* args */);
    ~Buffer();

    void append(const char* _str, int _size);
    ssize_t size();
    const char * c_str();
    void clear();
    void getLine();

    void setBuf(const char * ch);
};


