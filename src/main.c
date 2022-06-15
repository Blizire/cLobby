#include "main.h"

    // set window into foreground
    //SetForegroundWindow(windowHandle);

int main(){
    //screenshot("test.bmp", "*Untitled - Notepad");
    COLORREF *idleBuffer = NULL;
    unsigned long long idleArrSize = screenshot(&idleBuffer, "Untitled - Paint");

    for (;;) 
    {
        int diff = 1;
        double diffAverage = 0;
        COLORREF *activeBuffer = NULL;
        screenshot(&activeBuffer, "Untitled - Paint");
        for(int i = 0; i < idleArrSize; i++)
        {
            if( idleBuffer[i] != activeBuffer[i])
                diff++;
        }
        diffAverage = (double)diff / (double)idleArrSize;
        printf("average difference : %f\n diff : %i\n", diffAverage, diff);
        free(activeBuffer);

        if(diffAverage > 0.70)
            break;
    }
    free(idleBuffer);
    return 0;
}