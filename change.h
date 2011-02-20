#ifndef __CHANGE_H__
#define __CHANGE_H__
#include <string>
#include <iomanip>
#include <windows.h>
#define IP_DISTANCE 112
#define PORT_DISTANCE 100
#define VERSION_ADDRESS 0x45a250
#define RSA_KEY "109120132967399429278860960508995541528237502902798129123468757937\
266291492576446330739696001110603907230888610072655818825358503429057592827629436\
413108566029093628212635953836686562675849720620786279431090218017681061521755056\
710823876476444260558147179707119674283982419152118103759076030616683978566631413"

class Change
{
public:
    HANDLE getTibiaProcess();
    bool changeIP(std::string, int, std::string);
    bool writeMemory(unsigned, const char*);
    bool writeMemoryByte(unsigned, int);
    bool setRSA(unsigned);
    //int readMemory(unsigned);
    std::string getClientVersion();
};
#endif //__CHANGE_H__
