#ifndef _ADDRESS_H
#define _ADDRESS_H

#include <string>

class Address
{
    public:
    std::string hostname;
    std::string port;
    public:
    Address(std::string h, std::string p): hostname(h), port(p) {}
    Address(){}
};
#endif // _ADDRESS_H