/*++

Copyright (c) 2007 - 2017, Matthieu Suiche
Copyright (c) 2012 - 2014, MoonSols Limited
Copyright (c) 2015 - 2017, Comae Technologies FZE

Module Name:

    Misc.h

Abstract:

    This module contains the common library functions.

Author:

    Matthieu Suiche (m) 1-May-2016

Revision History:

--*/

#ifndef __MISC_H__
#define __MISC_H__

#define C_RED    "\e[0;31m"
#define C_YELLOW "\e[0;33m"
#define C_GREEN  "\e[0;32m"
#define C_WHITE  "\e[0;37m"
#define C_RESET  "\e[0m"

void
Red(
    const char * Format, ...
);

void
White(
    const char * Format, ...
);

void
Green(
    const char * Format, ...
);

#if _WIN32
uint16_t
GetConsoleTextAttribute(
    HANDLE hConsole
);

void
GetCursorPosition(
    HANDLE hConsole,
    PCOORD Coord
);

bool
CryptInitSha256(
);

uint32_t
CryptGetHashLen(
);

bool
CryptHashData(
    void *Buffer,
    uint32_t BufferSize
);

uint8_t *
CryptGetHash(
);

void
CryptClose(
);
#endif

#endif