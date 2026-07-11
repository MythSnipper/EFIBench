#include <timer.h>


//sleep for a certain number of miliseconds(smallest is 1 μs aka 0.001 ms)
void sleep(double ms){
    uefi_call_wrapper(BS->Stall, 1, (uint64_t)(ms * 1000));
}

//get EFI_TIME
EFI_TIME getTime(){
    EFI_TIME time;
    uefi_call_wrapper(RT->GetTime, 2, &time, NULL);
    return time;
}

//get time in malloc'd string
char* getTimeStr(){
    EFI_TIME time = getTime();
    char* ret = genTimeStr(time);
    return ret;
}

char* genTimeStr(EFI_TIME time){
    char* ret = malloc(20); //include null terminator

    sprint4ch(ret, time.Year);
    ret[4] = '-';
    sprint2ch(ret+5, time.Month);
    ret[7] = '-';
    sprint2ch(ret+8, time.Day);
    ret[10] = ' ';
    sprint2ch(ret+11, time.Hour);
    ret[13] = ':';
    sprint2ch(ret+14, time.Minute);
    ret[16] = ':';
    sprint2ch(ret+17, time.Second);

    //null terminator
    ret[19] = '\0';

    return ret;
}

