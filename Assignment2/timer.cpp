// timer.cpp
#include "systemc.h"
#include "timer.h"

void timer::runtimer() {
    while(1) {
        if (start.read()) {
            cout << "Timer: timer start detected " << endl;
            count = init_count;
            timeout.write(0);
    } else {
        if (count == 0) {
            timeout.write(1);
        } else {
            count--;
            timeout.write(0);
        }
    }
    wait();
    }
}
