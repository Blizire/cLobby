#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <stdio.h>
#include <Windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <windef.h>
#include "bitmap.h"

int screenshot(char * imageName, char * windowName);

#endif /* SCREENSHOT_H */