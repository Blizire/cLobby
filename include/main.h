#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <Windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <windef.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int BMPSetup(RGBQUAD **idleBuffer, 
                RGBQUAD **activeBuffer, 
                BITMAPINFO *bmi,
                HDC *dcScreenCapture,
                HDC *windowDCHandle,
                HBITMAP *bitmap,
                int *height,
                int *width,
                char *windowName);

double scanner(HDC *dcScreenCapture, 
                HDC *windowDCHandle, 
                RGBQUAD **idleBuffer, 
                RGBQUAD **activeBuffer, 
                BITMAPINFO *bmi,
                HBITMAP *bitmap, 
                int *width, 
                int *height);

#endif /* MAIN_H */