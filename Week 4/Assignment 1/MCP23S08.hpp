#ifndef H_MCP23S08
#define H_MCP23S08

#include "mbed.h"
#include "MCP_regctl.h"


// define addresses
enum MCPAddress{
    IODIR   = 0x00,
    IPOL    = 0x01,
    GPINTEN = 0x02,
    DEFVAL  = 0x03,
    INTCON  = 0x04,
    IOCON   = 0x05,
    GPPU    = 0x06,
    INTF    = 0x07,
    INTCAP  = 0x08,
    GPIO    = 0x09,
    OLAT    = 0x0A,
};

extern char recvlog[];
extern char sendlog[];
extern unsigned char used;
extern bool reads[];

namespace mcp{
    namespace pins{
        extern InterruptIn  INTR; // interrupt handle
    }
    void set(MCPAddress address, const char value);
    char get(MCPAddress address);
    void update(MCPAddress address, char value, char carry);

    void setSeq(MCPAddress startAddress, const char* value, size_t len);
    void getSeq(MCPAddress startAddress, char* value, size_t len);
    void updateSeq(MCPAddress startAddress, char* value, const char* carry, size_t len);

    void toggleBit(MCPAddress address, char bit);
    void setBit(MCPAddress address, char bit, bool value);
    bool getBit(MCPAddress address, char bit);
}

#endif // H_MCP23S08