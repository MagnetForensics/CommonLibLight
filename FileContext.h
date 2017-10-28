/*++

Copyright (c) 2007 - 2017, Matthieu Suiche
Copyright (c) 2012 - 2014, MoonSols Limited
Copyright (c) 2015 - 2017, Comae Technologies FZE

Module Name:

    FileContext.h

Abstract:

    This module contains the internal structure definitions and APIs used by
    the Hibr2Bin.

Author:

    Matthieu Suiche (m) 1-April-2016

Revision History:

--*/

#ifndef __FILE_CXT_H__
#define __FILE_CXT_H__

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x1000
#endif

#define MIN_READ_BUFFER_SIZE ((0x10 + 1) * PAGE_SIZE)

enum PlatformType {
    PlatformNone = 0,
    PlatformX86 = 1, // MACHINE_X86,
    PlatformX64 = 2  // MACHINE_AMD64
};

class FileContext {

public:
    FileContext()
    {

    }

    FileContext(PlatformType Platform, uint32_t Major, uint32_t Minor)
    {
        SetPlatform(Platform);
        SetMajorVersion(Major);
        SetMinorVersion(Minor);
    }

    uint32_t GetPlatform() { return m_Platform; }
    void SetPlatform(PlatformType Platform) { m_Platform = Platform; }

    uint32_t GetMajorVersion() { return m_MajorVersion; }
    void SetMajorVersion(uint32_t MajorVersion) { m_MajorVersion = MajorVersion; }

    uint32_t GetMinorVersion() { return m_MinorVersion; }
    void SetMinorVersion(uint32_t MinorVersion) { m_MinorVersion = MinorVersion; }

    FILE* GetFileHandle() { return m_FileHandle; }
    void SetFileHandle(FILE* Handle) { m_FileHandle = Handle; }

    bool
        OpenFile(
            char * FileName,
            uint32_t Type
        );

    bool
        OpenFile(
            char * FileName
        ) {
        return OpenFile(FileName, 0);
    }

    bool
        CreateOutputFile(
            char * FileName
        );

    void *
        ReadFile(
            uint64_t Offset,
            uint32_t Size,
            uint8_t **m_Data
        );

    bool
        WriteFile(
            void *Buffer,
            uint32_t NbOfBytesToWrite
        );

    void *
        GetTempBuffer(
        );

    bool
        IsWin8AndAbove(
        );

    bool
        IsWin7AndAbove(
        );

    bool
        IsVistaAndAbove(
        );

    bool
        Is64Bits(
        );

    bool
        IsWinXP(
        );

    bool
        IsWinXP64(
        );

    bool
        IsWinVista(
        );

    bool
        IsWin7(
        );

    bool
        IsWin8(
        );

    bool
        IsWin81(
        );

    bool
        IsWin10(
        );

    void
        Close(
        );

    uint64_t
        GetFileSize(
            void
        );

    ~FileContext();

private:
    PlatformType m_Platform;
    uint32_t m_MajorVersion;
    uint32_t m_MinorVersion;

    FILE* m_FileHandle = NULL;
    FILE* m_OutFileHandle = NULL;

    uint8_t *m_ReadedData = NULL;
    uint32_t m_ReadedDataSize = 0;

    uint8_t *m_PreAllocatedBuffer = NULL;
    uint32_t m_PreAllocatedBufferSize = MIN_READ_BUFFER_SIZE;
};

#endif