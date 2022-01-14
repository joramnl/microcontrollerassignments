#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

#include "mbed.h"
#include <algorithm>
#include <chrono>
#include <cstdio>

#define elements(a) sizeof(a)/sizeof(a[0])


DigitalOut led_red(PA_0);
DigitalOut led_grn(PA_1);

BufferedSerial serial(USBTX,USBRX);

// the command itself
enum command{
    NONE    = 0,
    TURN_ON ,
    TURN_OFF,
    SWITCH  ,
};
/// a part of the resolution chain. two children should not start with the same extension
struct command_resolution{
    const char* ext; // extension. this is the segment used until this command ends or the parse tree splits
    const command command;
    const unsigned char child_count; // child count
    const command_resolution* children;
};

const command_resolution ON_OFF[]{
        {
                .ext         = "N"    ,
                .command     = TURN_ON,
                .child_count = 0      ,
                .children    = nullptr,
        },{
                .ext         = "FF"    ,
                .command     = TURN_OFF,
                .child_count = 0       ,
                .children    = nullptr ,
        }
};

const command_resolution ROOT[] {
        {
                .ext         = "SWITCH",
                .command     = SWITCH  ,
                .child_count = 0       ,
                .children    = nullptr ,
        },{
                .ext         = "O"   ,
                .command     = NONE  ,
                .child_count = elements(ON_OFF),
                .children    = ON_OFF,
        }
};

char read1(){
    char read;
    while(true){
        while(true) {
            ssize_t res = serial.read(&read,1);
            if (res != 1){
                printf("Reading Failure!\n");
            }else{
                break;
            }
        }
        if (read == '\0' || read == '\r'){
//            printf("invalid character!\n");
        }else{
            putchar(read);
            return read;
        }
    }
}

inline void wait_until_newline(){
    while(read1() != '\n');
}
/**
 * Resolves a command from a command_resolution array.
 * This implementation does not use the heap at all
 * @param head the start of the array
 * @param count the number of resolutions in the array
 */
command resolve_command(const command_resolution* head, int count){
    // resolve child
    char read = read1();
    for (int i=0;i<count;++i){
        if (head[i].ext[0] == read){
            // continue on this resolution!
            head = &head[i];
            // ensure extension matches
            for (i=1;;++i){
                if (head->ext[i] == '\0'){
                    // end of this part reached
                    if (head->child_count){
                        return resolve_command(head->children,head->child_count);
                    }else{
                        // no children
                        read = read1();
                        if (read == '\n'){
                            return head->command;
                        }else{
                            wait_until_newline();
                            return NONE;
                        }
                    }
                }
                read = read1();
                if (read == '\n' || read != head->ext[i]){
                    wait_until_newline();
                    return NONE;
                }
            }
        }
    }
    return NONE;
}

EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread input_manager;
Thread eventoid;

void sp_main(){
    bool state = false;
    while(true){
        command cmd = resolve_command(ROOT, elements(ROOT));
        switch(cmd){
            default:
            case NONE:
                printf("Invalid command\n");
                break;
            case TURN_ON :led_red.write((state =   true)); break;
            case TURN_OFF:led_red.write((state =  false)); break;
            case SWITCH  :led_red.write((state = !state)); break;
        }
    }
}

InterruptIn btn_user(USER_BUTTON);

bool skip_blink = false;

void blinker_main(){
    static bool round, state;
    
    if (skip_blink){
        round = !round;
        if (round){
            return;
        }
    }
    led_grn.write((state = ! state));
}

int main(){
    puts("BOOT!\n\n");
    btn_user.fall([](){
        skip_blink =! skip_blink;
    });
    input_manager.start(&sp_main);
    eventoid.start(callback(&queue,&EventQueue::dispatch_forever));
    queue.call_every(250ms,&blinker_main);
    
}