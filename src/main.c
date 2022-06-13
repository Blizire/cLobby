#include "main.h"



int main(int argc, char * argv[]){
    int width;
    int height;
    HWND windowHandle;
    HDC hdcHandle;
    HDC hdcCapture;
    HBITMAP bmpOld;
    HBITMAP bitmap;

    // find window to screen cap
    windowHandle =  FindWindowA(NULL, "*Untitled - Notepad");

    if(!windowHandle)
    {
        puts("Error getting window handle!");
        return -1;
    }

    // set window into foreground
    SetForegroundWindow(windowHandle);

    // get device contect for window
    hdcHandle = GetWindowDC(windowHandle);

    if(!hdcHandle)
    {
        puts("Error getting device context!");
        return -1;
    }

    // creates another dc compatible with targeted window dc
    hdcCapture = CreateCompatibleDC(hdcHandle);

    // obtain w/h info from dc
    width = GetDeviceCaps(hdcHandle, HORZRES);
    height = GetDeviceCaps(hdcHandle, VERTRES);

    // makes empty bitmap based of original dc
    bitmap = CreateCompatibleBitmap(hdcHandle, width, height);

    if(!bitmap)
    {
        puts("Error making compatible bitmap!");
        return -1;
    }

    // select object in given DC, new object replaces previous object
    bmpOld = (HBITMAP)SelectObject(hdcCapture, bitmap);

    // transfer pixels from hdcHandle -> hdcCapture
    BitBlt(hdcCapture, 0, 0, width, height, hdcHandle, 0, 0, SRCCOPY);

    // done drawing (BitBlt'ing) so must return original
    SelectObject(hdcCapture, bmpOld);

    // clean up memory
    DeleteObject(bitmap);
    DeleteObject(hdcCapture);
    ReleaseDC(windowHandle, hdcHandle);

    return 0;
}