#include "screenshot.h"

//int screenshot(char * imageName, char * windowName){
unsigned long long screenshot(COLORREF **rgbArray, char * windowName){
    int width;
    int height;
    HWND windowHandle;
    HDC hdcHandle;
    HDC hdcCapture;
    HBITMAP bmpOld;
    HBITMAP bitmap;
    //PBITMAPINFO bmpHeaders;
    LPRECT windowCords = calloc(1, sizeof(LPRECT));

    // find window to screen cap
    windowHandle =  FindWindowA(NULL, windowName);

    if(!windowHandle)
    {
        puts("Error getting window handle!");
        return (unsigned long long) -1;
    }

    // get device contect for window
    hdcHandle = GetWindowDC(windowHandle);

    if(!hdcHandle)
    {
        puts("Error getting device context!");
        return (unsigned long long) -1;
    }

    // creates another dc compatible with targeted window dc
    hdcCapture = CreateCompatibleDC(hdcHandle);
    if(!hdcCapture){
        puts("Error creating compatible DC!");
        return (unsigned long long) -1;
    }

    // obtain w/h info from dc
    if(!GetWindowRect(windowHandle, windowCords)){
        puts("Error getting window dimensions!");
        return (unsigned long long) -1;
    }
    width = windowCords->right - windowCords->left;
    height = windowCords->bottom - windowCords->top;




    // makes empty bitmap based of original dc
    bitmap = CreateCompatibleBitmap(hdcHandle, width, height);
    if(!bitmap)
    {
        puts("Error making compatible bitmap!");
        return (unsigned long long) -1;
    }

    // select object in given DC, new object replaces previous object
    bmpOld = (HBITMAP)SelectObject(hdcCapture, bitmap);
    if(bmpOld ==  HGDI_ERROR || bmpOld == NULL)
    {
        puts("Error selecting bitmap to hdcCapture!");
        return (unsigned long long) -1;       
    }
    //printf("width: %i\nheight: %i\nsize : %i", width, height, width * height);
    // transfer pixels from hdcHandle -> hdcCapture
    BitBlt(hdcCapture, 0, 0, width, height, hdcHandle, 0, 0, SRCCOPY);
    unsigned long long rgbArraySize = width * height;
    //*rgbArray = (COLORREF *) calloc(rgbArraySize, sizeof(COLORREF));
    COLORREF *tempRgbArray;
    tempRgbArray = (COLORREF *) calloc(rgbArraySize, sizeof(COLORREF));
    //COLORREF pixel2 = GetPixel(hdcCapture, 100, 700);
    int rgbArrayIndex = 0;
    for (int xBMPCord = 0; xBMPCord < width; xBMPCord++){
        for(int yBMPCord = 0; yBMPCord < height; yBMPCord++){
            tempRgbArray[rgbArrayIndex] = GetPixel(hdcCapture, xBMPCord, yBMPCord);
            rgbArrayIndex++;
            //printf("accessed [%i]", rgbArrayIndex);
        }
    }
    //printf("pixels placed : %i\n", rgbArrayIndex);
    free(*rgbArray);
    *rgbArray = tempRgbArray;
    //puts("Loaded pixel buffer");

    // done drawing (BitBlt'ing) so must return original
    SelectObject(hdcCapture, bmpOld);
    if(bmpOld ==  HGDI_ERROR || bmpOld == NULL)
    {
        puts("Error selecting bitmap to hdcCapture!");
        return (unsigned long long) -1;       
    }

    // create info struct to  write to file
    //PBITMAPINFO bmpHeaders;
    //bmpHeaders = CreateBitmapInfoStruct((HWND)NULL, bitmap);
    
    // save file to disk
    //char * imageName = "test.bmp";
    //CreateBMPFile((HWND)NULL, imageName, bmpHeaders, bitmap, hdcCapture);
    

    // clean up memory
    DeleteObject(bitmap);
    DeleteObject(hdcCapture);
    ReleaseDC(windowHandle, hdcHandle);

    // rgbArraySize in bits / datatype
    return rgbArraySize;
}