#include "mbed.h"
#include "MCP23S08.hpp"
#include "miscutil.hpp"
#include <cstdio>
#include <cstdlib>


// main() runs in its own thread in the OS


/// used to print only what a certain side of the conversation has willingly said
void convoexclude(const char* speaker, bool isread){
    for (int i=0;i<used;++i){
        if (reads[i] == isread){ // was speaking
            print::bin(&speaker[i],1);
        }else{
            printf("         ");
        }
    }
    putchar('\n');
}
/// prints useful information about the conversation with the peripheral
void convodebug(){
    printf("\nExchanged %d bytes total\n",used);
    // simple display, showing also incidental reads/writes
    printf("tx: ");
    print::bin(sendlog,used);
    printf("\nrx: ");
    print::bin(recvlog,used);
    putchar('\n');
    // seperator
    for (int i=0;i<(used+1)*8+14;++i){
        putchar('-');
    }
    // advanced display, showing only who is talking
    printf("\ntx: ");
    convoexclude(sendlog, false);
    printf("   ");
    bool lastRead = false;
    for (int i=0;i<used;++i){
        if (reads[i] == lastRead){
            putchar(' ');
        }else{
            putchar(lastRead?'/':'\\');
            lastRead = !lastRead;
        }
        printf("        ");
    }
    printf("\nrx: ");
    convoexclude(recvlog, true );

    putchar('\n');
    fflush(stdout);
    used =0;
}

char value;
bool flip;
/// the part where the LEDs cycle
void cycle(){
    used = 0;

    if (!value){
        value = 0xff;
        flip = !flip;
    }
    char towrite = (flip ? value : ~value) &~0b11000000;
    print::bin(&towrite,1);
    putchar('\n');
    mcp::set(GPIO,towrite);

    used = 0;       
    value >>=1;
}
/// runs the bit where you can control the topmost button using the S1 button
void toggle(){
    used = 0;
    ThisThread::sleep_for(200ms);
    mcp::toggleBit(GPIO, MCP_GP3);
    mcp::setBit(GPIO, MCP_GP2, mcp::getBit(GPIO, MCP_GP7));
}
// dumps the contents of the registers to the terminal
void memdump(){
    char registers[11];
    printf("__dump__\n");
    mcp::getSeq(MCPAddress::DEFVAL, registers, 11);
    for (int i=0;i<11;++i){
        print::bin(&registers[i],1);
        putchar('\n');
    }
    fflush(stdout);
}

int main(){
    puts("\n\nBOOT!");
    ThisThread::sleep_for(100ms);
    sys::BusyMark mark;
    mcp::set(MCPAddress::IODIR,0xc0);
    mcp::set(MCPAddress::GPIO ,0x00);

    memdump();
    convodebug();
    // flash red then green a few times
    for (int i=0;i<10;++i){
        mcp::set(MCPAddress::GPIO,0b00101010);
        ThisThread::sleep_for(100ms);
        mcp::set(MCPAddress::GPIO,0b00010101);
        ThisThread::sleep_for(100ms);
    }
    // flash a 'random' set of leds
    mcp::set(GPIO,rand());// 00110000 ;)
    for (int i=0;i<10;++i){
        used = 0;
        mcp::set(GPIO,~mcp::get(GPIO));
        ThisThread::sleep_for(100ms);
        convodebug();
    }
    // cycle through the LEDs
    for (int i=0;i<50;++i){
        cycle();
        ThisThread::sleep_for(100ms);
    }
    // start the toggle cycle
    mcp::set(MCPAddress::GPIO, 0xff);
    while (true){
        toggle();
    }
}

