/*++

Copyright (c) 2007 - 2017, Matthieu Suiche
Copyright (c) 2012 - 2014, MoonSols Limited
Copyright (c) 2015 - 2017, Comae Technologies FZE

Module Name:

    Misc.cpp

Abstract:

    This module contains the common library functions.

Author:

    Matthieu Suiche (m) 1-May-2016

Revision History:

--*/

#include "precomp.h"

bool g_DbgMode = false;

#if _WIN32
HCRYPTPROV g_hCryptProv = NULL;
HCRYPTHASH g_hCrypHash = NULL;
ULONG g_HashLen = 0;
#endif

#if _WIN32
uint16_t
GetConsoleTextAttribute(
    HANDLE hConsole
)
{
    HANDLE Handle = (HANDLE)hConsole;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(Handle, &csbi);
    return(csbi.wAttributes);
}
#endif

void
White(
    const char * Format,
    ...
)
{
    va_list va;

#if _WIN32
    HANDLE Handle;
    uint16_t Color;
    Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    Color = GetConsoleTextAttribute((HANDLE)Handle);
    SetConsoleTextAttribute(Handle, 0xF);
#endif

    va_start(va, Format);
#ifndef _WIN32
    vprintf(C_WHITE, va);
#endif
    vprintf(Format, va);
#ifndef _WIN32
    vprintf(C_RESET, va);
#endif
    va_end(va);

#if _WIN32
    SetConsoleTextAttribute(Handle, Color);
#endif
}

void
Red(
    const char * Format,
    ...
)
{
    va_list va;

#if _WIN32
    HANDLE Handle;
    uint16_t Color;
    Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    Color = GetConsoleTextAttribute((HANDLE)Handle);
    SetConsoleTextAttribute(Handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
#endif

    va_start(va, Format);
#ifndef _WIN32
    vprintf(C_RED, va);
#endif
    vprintf(Format, va);
#ifndef _WIN32
    vprintf(C_RESET, va);
#endif
    va_end(va);

#if _WIN32
    SetConsoleTextAttribute(Handle, Color);
#endif
}

void
Green(
    const char * Format,
    ...
)
{
    va_list va;

#if _WIN32
    HANDLE Handle;
    uint16_t Color;
    Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    Color = GetConsoleTextAttribute((HANDLE)Handle);
    SetConsoleTextAttribute(Handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif

    va_start(va, Format);
#ifndef _WIN32
    vprintf(C_GREEN, va);
#endif
    vprintf(Format, va);
#ifndef _WIN32
    vprintf(C_RESET, va);
#endif
    va_end(va);

#if _WIN32
    SetConsoleTextAttribute(Handle, Color);
#endif
}

void
Yellow(
    const char * Format,
    ...
)
{
    va_list va;

#if _WIN32
    HANDLE Handle;
    uint16_t Color;
    Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    Color = GetConsoleTextAttribute((HANDLE)Handle);
    SetConsoleTextAttribute(Handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif

    va_start(va, Format);
#ifndef _WIN32
    vprintf(C_YELLOW, va);
#endif
    vprintf(Format, va);
#ifndef _WIN32
    vprintf(C_RESET, va);
#endif
    va_end(va);

#if _WIN32
    SetConsoleTextAttribute(Handle, Color);
#endif
}

void
dbgPrint(
    const char * Format,
    ...
)
{
    if (!g_DbgMode) return;
    va_list va;

#if _WIN32
    HANDLE Handle;
    uint16_t Color;
    Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    Color = GetConsoleTextAttribute((HANDLE)Handle);
    SetConsoleTextAttribute(Handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif

    va_start(va, Format);
#ifndef _WIN32
    vprintf(C_YELLOW, va);
#endif
    vprintf(Format, va);
#ifndef _WIN32
    vprintf(C_RESET, va);
#endif
    va_end(va);

#if _WIN32
    SetConsoleTextAttribute(Handle, Color);
#endif
}

#if _WIN32
void
GetCursorPosition(HANDLE hConsole, PCOORD Coord)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);

    *Coord = csbi.dwCursorPosition;
}

bool
CryptInitSha256(
    void
)
{
    // https://support.microsoft.com/en-us/kb/238187
    if (!CryptAcquireContext(&g_hCryptProv,
        NULL,
        MS_ENH_RSA_AES_PROV,
        PROV_RSA_AES,
        0)) // CRYPT_MACHINE_KEYSET))
    {
        if (GetLastError() == NTE_BAD_KEYSET)
        {
            if (!CryptAcquireContext(&g_hCryptProv,
                NULL,
                MS_ENH_RSA_AES_PROV,
                PROV_RSA_AES,
                CRYPT_NEWKEYSET)) // CRYPT_NEWKEYSET | CRYPT_MACHINE_KEYSET))
            {
                DbgPrint(" Error: CryptAcquireContext() failed with %d\n", GetLastError());
                return false;
            }
        }
    }

    if (!CryptCreateHash(g_hCryptProv, CALG_SHA_256, 0, 0, &g_hCrypHash)) {
        DbgPrint(" Error: CryptCreateHash() failed with %d\n", GetLastError());
        return false;
    }

    return true;
}

bool
CryptHashData(
    void *Buffer,
    uint32_t BufferSize
)
{
    if (g_hCrypHash == NULL) return false;
    if (!CryptHashData(g_hCrypHash, (const uint8_t*)Buffer, BufferSize, 0)) return false;

    return true;
}

uint8_t *
CryptGetHash(
)
{
    uint32_t InputSize = sizeof(uint32_t);

    if (g_hCrypHash == NULL) return false;

    if (!CryptGetHashParam(g_hCrypHash, HP_HASHSIZE, (uint8_t *)&g_HashLen, (DWORD *)&InputSize, 0)) return false;

    uint8_t *FinalHash = new uint8_t[g_HashLen];
    if (!CryptGetHashParam(g_hCrypHash, HP_HASHVAL, FinalHash, &g_HashLen, 0)) return false;

    return FinalHash;
}

uint32_t
CryptGetHashLen(
)
{
    return g_HashLen;
}

void
CryptClose()
{
    if (g_hCryptProv) CryptDestroyHash(g_hCryptProv);
    if (g_hCrypHash) CryptReleaseContext(g_hCrypHash, 0);
}

#endif