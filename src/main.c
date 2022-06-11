#include "main.h"

int main(int argc, char * argv[]){
    SYSTEMTIME time;
    GetLocalTime(&time);
    printf("Local time is %02d:%02d\n", time.wHour, time.wMinute);
    return 0;
}