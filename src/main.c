#include "main.h"

int main(){
    char *windowName = "Untitled - Paint";

    // find window to screen cap
    HWND windowHandle =  FindWindowA(NULL, windowName);
    if(!windowHandle)
    {
        puts("Error getting window handle!");
        return -1;
    }

    // get device content for window
    HDC windowDCHandle = GetWindowDC(windowHandle);

    if(!windowDCHandle)
    {
        puts("Error getting device context!");
        return -1;
    }

    // creates another dc compatible with targeted window dc
    HDC dcScreenCapture = CreateCompatibleDC(windowDCHandle);
    if(!dcScreenCapture){
        puts("Error creating compatible DC!");
        return -1;
    }    

    // obtain w/h info from dc
    LPRECT windowCords = calloc(1, sizeof(LPRECT));
    if(!GetWindowRect(windowHandle, windowCords)){
        puts("Error getting window dimensions!");
        return -1;
    }
    int width = windowCords->right - windowCords->left;
    int height = windowCords->bottom - windowCords->top;

    // makes empty bitmap based of original dc
    HBITMAP bitmap = CreateCompatibleBitmap(windowDCHandle, width, height);
    if(!bitmap)
    {
        puts("Error making compatible bitmap!");
        return -1;
    }

    // select object basically allows the device context to write/read bitmap
    SelectObject(dcScreenCapture, bitmap);

    // load the image data from windowDCHandle to dcScreenCapture
    BitBlt(dcScreenCapture, 0, 0, width, height, windowDCHandle, 0, 0, SRCCOPY);

    // header that getdibbits will need later
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    //  the first screenshot (idlebuffer)  gets loaded here
    unsigned long long rgbArraySize = width * height;
    RGBQUAD *idleBuffer = (RGBQUAD *) calloc(rgbArraySize, sizeof(RGBQUAD));
    RGBQUAD *activeBuffer = (RGBQUAD *) calloc(rgbArraySize, sizeof(RGBQUAD));
    GetDIBits(dcScreenCapture, bitmap, 0, height, idleBuffer, &bmi, DIB_RGB_COLORS);


    // detect differences between first screenshot and the consecutive scans
    for (;;) 
    {
        int diff = 1;
        double diffAverage = 0;
        
        for(int i = 0; i < rgbArraySize; i++)
        {
            // continously get screen data and compare the buffer.
            BitBlt(dcScreenCapture, 0, 0, width, height, windowDCHandle, 0, 0, SRCCOPY);            
            GetDIBits(dcScreenCapture, bitmap, 0, height, activeBuffer, &bmi, DIB_RGB_COLORS);

            // compare buffers and increase diff score
            if( idleBuffer[i].rgbRed != activeBuffer[i].rgbRed
                || idleBuffer[i].rgbGreen != activeBuffer[i].rgbGreen
                || idleBuffer[i].rgbBlue != activeBuffer[i].rgbBlue)
            {            
                diff++;
                diffAverage = (double)diff / (double)rgbArraySize;
            }
        }

        // if the active buffer is different we have detected a great enough change to act on
        printf("average difference : %f\n diff : %i\n", diffAverage, diff);

        if(diffAverage > 0.70){
            free(idleBuffer);
            free(activeBuffer);    
            break;
        }
    }

    DeleteObject(bitmap);
    DeleteObject(dcScreenCapture);
    ReleaseDC(windowHandle, windowDCHandle);
    return 0;
}