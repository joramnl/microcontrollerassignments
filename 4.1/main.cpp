#include <cstdint>
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#include "mbed.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include "magic.h"

#define elements(a) sizeof(a)/sizeof(a[0])

#define MAGIC 0xDEAD00F


class custom_SPI : private NonCopyable<custom_SPI>{
    DigitalOut clk;
    DigitalOut so ;
    DigitalIn  si ;
    DigitalOut ncs;
    Ticker t;
    // transmission
    // -- provided
    volatile bool accepting;
    uint_least8_t data;
    // -- send state
    bool tickstate;
    uint_least8_t bit : 3;

public:

    inline custom_SPI(PinName clk, PinName so, PinName si, PinName ncs, std::chrono::microseconds clock)
    : clk(clk), so(so), si(si), ncs(ncs), bit(0){
        t.attach([this](){
            if (tickstate){
                this->_writeBit();
            }
            this->clk.write((tickstate = !tickstate));
            // 
        }, clock);
    }

    void send(uint_least8_t address, uint_least8_t * data, size_t len){
        provide(address);
        for(int i=0;i<len;++i){
            provide(data[i]);
        }
    }

private:
    void provide(uint_least8_t octet){
        while(!accepting) ThisThread::yield();
        data = octet;
        bit  = 0;
        accepting = false;
    }

    void _writeBit(){
        if (!accepting){
            so.write((data >> this->bit) & 1);
            this->bit = (this->bit + 1) % 8;
        }
    }
};


custom_SPI spi(A0,A1,A2,A3,20ms);

int main(){
    puts("BOOT!\n\n");
    static uint8_t data[] = {0xa5,0x5a};
    spi.send(0b0100000,data,elements(data));

}