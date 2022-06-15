#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <stdio.h>
#include <Windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <windef.h>
#include "bitmap.h"

unsigned long long screenshot(COLORREF **rgbArray, char *windowName);

#endif /* SCREENSHOT_H */