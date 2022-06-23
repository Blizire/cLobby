#include "main.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow)
{
    MSG msg;
    HWND hwnd;
    WNDCLASSW wc;

    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.lpszClassName = L"Window";
    wc.hInstance     = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpszMenuName  = NULL;
    wc.lpfnWndProc   = WndProc;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassW(&wc);

    hwnd = CreateWindowEx( WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
                "Window",
                "Idle - 0",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                0, 0, 350, 30, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    RGBQUAD *idleBuffer   = NULL;
    RGBQUAD *activeBuffer = NULL;
    BITMAPINFO *bmi       = (BITMAPINFO *) calloc(1, sizeof(BITMAPINFO));

    HDC *dcScreenCapture  = (HDC *) calloc(1, sizeof(HDC));
    HDC *windowDCHandle   = (HDC *) calloc(1, sizeof(HDC));
    HBITMAP *bitmap       = (HBITMAP *) calloc(1, sizeof(HBITMAP));
    int *height           = (int *) calloc(1, sizeof(int));
    int *width            = (int *) calloc(1, sizeof(int));

    BMPSetup(&idleBuffer, 
                &activeBuffer, 
                bmi, 
                dcScreenCapture, 
                windowDCHandle, 
                bitmap, 
                height, 
                width,
                "Untitled - Paint");

    double diff = 0;
    char *titleBuffer = (char *) calloc(50, sizeof(char));
    
    while (GetMessage(&msg, NULL, 0, 0)) {

        diff = scanner(dcScreenCapture, 
                        windowDCHandle, 
                        &idleBuffer, 
                        &activeBuffer, 
                        bmi,
                        bitmap, 
                        width, 
                        height);
        snprintf(titleBuffer, 50, "Scanning - %f", diff);
        SetWindowTextA(hwnd, titleBuffer);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    switch(msg) {

        case WM_DESTROY:

            PostQuitMessage(0);
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int BMPSetup(RGBQUAD **idleBuffer, 
                RGBQUAD **activeBuffer, 
                BITMAPINFO *bmi,
                HDC *dcScreenCapture,
                HDC *windowDCHandle,
                HBITMAP *bitmap,
                int *height,
                int *width,
                char *windowName)
{
    // find window to screen cap
    HWND windowHandle =  FindWindowA(NULL, windowName);
    if(!windowHandle)
    {
        puts("Error getting window handle!");
        return -1;
    }

    // get device content for window
    *windowDCHandle = GetWindowDC(windowHandle);

    if(!windowDCHandle)
    {
        puts("Error getting device context!");
        return -1;
    }

    // creates another dc compatible with targeted window dc
    *dcScreenCapture = CreateCompatibleDC(*windowDCHandle);
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
    *width = windowCords->right - windowCords->left;
    *height = windowCords->bottom - windowCords->top;

    // makes empty bitmap based of original dc
    *bitmap = CreateCompatibleBitmap(*windowDCHandle, *width, *height);
    if(!bitmap)
    {
        puts("Error making compatible bitmap!");
        return -1;
    }
    
    // select object basically allows the device context to write/read bitmap
    SelectObject(*dcScreenCapture, *bitmap);

    // load the image data from windowDCHandle to dcScreenCapture
    BitBlt(*dcScreenCapture, 0, 0, *width, *height, *windowDCHandle, 0, 0, SRCCOPY);
    //  the first screenshot (idlebuffer)  gets loaded here
    unsigned long long rgbArraySize = (*width) * (*height);
    free(*idleBuffer);
    free(*activeBuffer);
    *idleBuffer = (RGBQUAD *) calloc(rgbArraySize, sizeof(RGBQUAD));
    *activeBuffer = (RGBQUAD *) calloc(rgbArraySize, sizeof(RGBQUAD));

    // header that getdibbits will need later
    //BITMAPINFO bmi = {0};
    (*bmi).bmiHeader.biSize = sizeof((*bmi).bmiHeader);
    (*bmi).bmiHeader.biWidth = *width;
    (*bmi).bmiHeader.biHeight = *height;
    (*bmi).bmiHeader.biPlanes = 1;
    (*bmi).bmiHeader.biBitCount = 32;
    (*bmi).bmiHeader.biCompression = BI_RGB;

    // load up the idle buffer
    GetDIBits(*dcScreenCapture, *bitmap, 0, *height, *idleBuffer, bmi, DIB_RGB_COLORS);
    return 0;
}

double scanner(HDC *dcScreenCapture, 
                HDC *windowDCHandle, 
                RGBQUAD **idleBuffer, 
                RGBQUAD **activeBuffer, 
                BITMAPINFO *bmi,
                HBITMAP *bitmap, 
                int *width, 
                int *height)
{
    // detect differences between first screenshot and the consecutive scans
    int diff = 1;
    unsigned long long rgbArraySize = (*width) * (*height);
    double diffAverage = 0;
    // continously get screen data and compare the buffer.
    BitBlt(*dcScreenCapture, 0, 0, *width, *height, *windowDCHandle, 0, 0, SRCCOPY);
    GetDIBits(*dcScreenCapture, *bitmap, 0, *height, *activeBuffer, bmi, DIB_RGB_COLORS);
    for(int i = 0; i < rgbArraySize; i++)
    {
        // compare buffers and increase diff score
        if( (*idleBuffer)[i].rgbRed != (*activeBuffer)[i].rgbRed
            || (*idleBuffer)[i].rgbGreen != (*activeBuffer)[i].rgbGreen
            || (*idleBuffer)[i].rgbBlue != (*activeBuffer)[i].rgbBlue)
        {            
            diff++;
            diffAverage = (double)diff / (double)rgbArraySize;
        }
    }

    return diffAverage;
}