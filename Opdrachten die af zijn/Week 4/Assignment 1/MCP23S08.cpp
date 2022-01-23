#include "MCP23S08.hpp"
#include <cstdint>
#include <cstdio>
#include <utility>
#include "miscutil.hpp"

#define MCP_DEVICE_ADDRESS_READ  0b01000111
#define MCP_DEVICE_ADDRESS_WRITE 0b01000110

#define CONVO_LOG_LENGTH 50

char recvlog[CONVO_LOG_LENGTH];
char sendlog[CONVO_LOG_LENGTH];
bool reads  [CONVO_LOG_LENGTH];
unsigned char used;

namespace pins{
    DigitalOut   SCK   (D13, 0); // serial clock
    DigitalOut   SI    (D11, 0); // serial input
    DigitalIn    SO    (D12   ); // serial output
    //           A1    (POW, 1);// hardware address input
    //           A0    (POW, 1);// hardware address input
    //           RESET (POW, 1);// reset (biased)
    DigitalOut   CS    (D10, 1); // chip select
    InterruptIn  INTR  (D6    );
    // rhs is not directly controlled
}

inline void tick(){
    wait_ns(100);
}

inline void start(){
    sys::status.write(HIGH);
    pins::CS.write(LOW);
    //used = 0;
}

inline bool exchangebit(bool b){
    tick();
    pins::SI.write(b);
    pins::SCK.write(HIGH);
    ThisThread::sleep_for(5ms);
    tick();
    bool out = pins::SO.read();
    pins::SCK.write(LOW);
    return out;
}

inline void stop(){
    tick();
    pins::CS.write(HIGH);
    sys::status.write(LOW);
}

char exchange(char value){
    char out = exchangebit(value & 0x80);
    for (char select = 0x40 ;select; select>>=1){
        out = out << 1 | exchangebit(value&select);
    }

    sendlog[used] = value;
    recvlog[used] = out;
    used++;
    if (used > CONVO_LOG_LENGTH){
        used = 0; // prevents overwriting memory at the cost of invalid data
    }
    putchar('.');
    fflush(stdout);

    return out;
}

static inline char receive(){
    reads[used] = true;
    return exchange(0);
}

static inline void send(char value){
    reads[used]= false;
    exchange(value);
}

bool sequential = false;

void ensureSequential(){
    if (!sequential){
        mcp::update(MCPAddress::IOCON, MCP_IOCON_SEQUENTIAL_OPERATION,~MCP_IOCON_SEQUENTIAL_OPERATION);
        sequential = true;
    }
}

inline void apply(char* destination, char carry, char resource){
    *destination = 
        (~carry & *destination) | 
        ( carry & resource);
}

void mcp::update(MCPAddress address, char value, char carry){
    if (carry){
        apply(&value,carry,get(address));
    }
    set(address,value);
}

void mcp::set(MCPAddress addr, const char value){
    start();
    send(MCP_DEVICE_ADDRESS_WRITE);
    send(addr);
    send(value);
    stop();
}

char mcp::get(MCPAddress address){
    start();
    send(MCP_DEVICE_ADDRESS_READ);
    send(address);
    char out = receive();
    stop();
    return out;
}

void mcp::setSeq(MCPAddress address, const char *value, size_t len){
    ensureSequential();
    start();
    send(MCP_DEVICE_ADDRESS_WRITE);
    send(address);
    while(len--){
        send(*value);
        ++value;
    }
    stop();
}

void mcp::getSeq(MCPAddress startAddress, char* value, size_t len){
    ensureSequential();
    start();
    send(MCP_DEVICE_ADDRESS_READ);
    send(startAddress);
    while(len--){
        *value = receive();
        ++value;
    }
    stop();
}

void mcp::updateSeq(MCPAddress startAddress, char* value, const char* carry, size_t len){
    ensureSequential();
    start();
    send(MCP_DEVICE_ADDRESS_READ);
    send(startAddress);
    for(size_t i=0;i<len;++i){
        apply(&value[i],carry[i],receive());
    }
    stop();
    setSeq(startAddress, value, len);
}