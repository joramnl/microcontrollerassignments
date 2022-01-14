#include "mbed.h"
#include <cstdio>

InterruptIn button(PA_1);
DigitalOut led(PA_0);

bool expect; // expect a change (fault detection)
bool actual; // actual state
bool faulty; // whether the actual state and expected state differ upon an iteraction. caused by rise -> rise or fall -> fall. the second change is ignored
bool state; // the state of the led. used to turn it on and off
bool change; // whether to change the state or not. toggled on button rise
bool rose; // whether a rise interrupt was handled this iteration
bool fell; // ^ but fall interrupt

// DigitalOut in(PA_1);
/**
 * checks whether the flag is set, prints and resets if set
 */
void check(bool& val, const char* message){
    if (val){
        val = false;
        puts(message);
    }
}

int a;

int main(){
    printf("BOOT! %d\n",a);
    printf("expect %d\n"
           "actual %d\n"
           "faulty %d\n"
           "state  %d\n"
           "change %d\n"
           "rose   %d\n"
           "fell   %d\n",expect,actual,faulty,state,change,rose,fell);
    ++a;
    button.rise([](){
        rose=true;
        if (expect){
            change = !change;
            expect=false;
        }else{
            faulty=true;
        }
    });
    button.fall([](){
        fell=true;
        if (expect){
            faulty=true;
        }else{
            actual=false;
            expect=true;
        }
    });
    while(true){
        printf("%d %d %d %d %d %d %d\n",rose,fell,expect,actual,faulty,state,change);
        check(faulty,"input fault");
        rose = fell = faulty = false;
        if (change){
            led.write(state =!state);
        }
        ThisThread::sleep_for(500ms);
    }
}