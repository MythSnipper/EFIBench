#include <timer.h>


//sleep for a certain number of miliseconds(smallest is 1 μs aka 0.001 ms)
void sleep(double ms){
    uefi_call_wrapper(BS->Stall, 1, (uint64_t)(ms * 1000));
}




