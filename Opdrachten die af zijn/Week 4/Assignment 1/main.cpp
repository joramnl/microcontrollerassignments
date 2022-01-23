#include "mbed.h"
#include "MCP23S08.hpp"
#include "miscutil.hpp"
#include <cstdio>
#include <cstdlib>


// main() runs in its own thread in the OS



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

void convodebug(){
    printf("transmitted %d bytes total\n",used);
    // simple display, showing also incidental reads/writes
    printf("tx: ");
    print::bin(sendlog,used);
    printf("\nrx: ");
    print::bin(recvlog,used);
    putchar('\n');
    // seperator
    for (int i=0;i<(used+1)*8+10;++i){
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
}

char value;
bool flip;
void toggle(){
    if (!value){
        value = 0xff;
        flip = !flip;
    }
    char towrite = (flip ? value : ~value) & 0b11111111;
    printf("\nWriting");
    mcp::set(MCPAddress::GPIO,towrite);
    char read = mcp::get(MCPAddress::GPIO);

    if (read != towrite){
        printf("\nPGIO oddity: %d -> %d\n\n",towrite,read);
    }

    convodebug();
    used = 0;       
    value >>=1;
}

int main(){
    puts("BOOT!");
    ThisThread::sleep_for(100ms);
    sys::BusyMark mark;
    mcp::set(MCPAddress::IODIR,0xff);
    mcp::set(MCPAddress::GPIO ,0x00);
    used = 0;
    // read entire address space

    char data[11];
    mcp::getSeq(MCPAddress::DEFVAL,data, 11);
    convodebug();
    print::bin(data,11);


    ThisThread::sleep_for(500ms);
    mcp::set(MCPAddress::GPIO, 0x7f);
    while (true){
        ThisThread::sleep_for(50ms);
        toggle();
    }
}

