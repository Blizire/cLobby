#ifndef BITMAP_H
#define BITMAP_H

#include <Windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <stdio.h>

#include <strsafe.h>

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, 
                  HBITMAP hBMP, HDC hDC);

#endif