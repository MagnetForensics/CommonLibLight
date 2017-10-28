/*++

Copyright (c) 2007 - 2017, Matthieu Suiche
Copyright (c) 2012 - 2014, MoonSols Limited
Copyright (c) 2015 - 2017, Comae Technologies FZE

Module Name:

    FileContext.cpp

Abstract:

    This module contains the internal structure definitions and APIs used by
    the Hibr2Bin.

Author:

    Matthieu Suiche (m) 1-April-2016

Revision History:

--*/

#include "precomp.h"

bool
read_file_at(
    FILE *fp,
    uint64_t offset,
    void *buffer,
    uint32_t buffer_size
)
{
    size_t size = PAGE_SIZE;
    size_t result = 0;
    bool status = false;

    if (buffer_size) size = buffer_size;

    if (fseek64(fp, offset, SEEK_SET)) {
        printf("error: can't change the offset to 0x%" I64_FORMAT "x.\n", offset);
        goto cleanup;
    }

    result = fread(buffer, 1, size, fp);
    if (result != size) {
        // printf("error: can't read data at 0x%llx. result = 0x%zx\n", offset, result);
        goto cleanup;
    }

    status = true;

cleanup:
    return status;
}


bool
FileContext::Is64Bits(
)
{
    if (GetPlatform() == PlatformX64) return true;
    else return false;
}

bool
FileContext::IsWin10(
)
{
    if ((GetMajorVersion() == 10) && (GetMinorVersion() == 0)) return true;
    return false;
}

bool
FileContext::IsWin81(
)
{
    if ((GetMajorVersion() == 6) && (GetMinorVersion() == 3)) return true;

    return false;
}

bool
FileContext::IsWin8(
)
{
    if ((GetMajorVersion() == 6) && (GetMinorVersion() == 2)) return true;
    return false;
}

bool
FileContext::IsWin7()
{
    if ((GetMajorVersion() == 6) && (GetMinorVersion() == 1)) return true;

    return false;
}

bool
FileContext::IsWinVista(
)
{
    if ((GetMajorVersion() == 6) && (GetMinorVersion() == 0)) return true;
    return false;
}

bool
FileContext::IsWinXP64(
)
{
    if ((GetMajorVersion() == 5) && (GetMinorVersion() == 2) && Is64Bits()) return true;

    return false;
}

bool
FileContext::IsWinXP(
)
{
    if ((GetMajorVersion() == 5) && (GetMinorVersion() == 1)) return true;

    return false;
}

bool
FileContext::IsVistaAndAbove(
)
{
    if (GetMajorVersion() >= 6) return true;

    return false;
}

bool
FileContext::IsWin7AndAbove(
)
{
    if ((GetMajorVersion() > 6) || ((GetMajorVersion() == 6) && (GetMinorVersion() >= 1)))
    {
        return true;
    }

    return false;
}

bool
FileContext::IsWin8AndAbove(
)
{
    if ((GetMajorVersion() > 6) || ((GetMajorVersion() == 6) && (GetMinorVersion() >= 2)))
    {
        return true;
    }

    return false;
}

void *
FileContext::GetTempBuffer()
{
    if (m_PreAllocatedBuffer == NULL) m_PreAllocatedBuffer = new uint8_t[m_PreAllocatedBufferSize];
    RtlZeroMemory(m_PreAllocatedBuffer, m_PreAllocatedBufferSize);

    return m_PreAllocatedBuffer;
}

void *
FileContext::ReadFile(
    uint64_t Offset,
    uint32_t DataBufferSize,
    uint8_t **DataBuffer
)
{
    void *Buffer = NULL;

    // assert(DataBufferSize <= MAX_COMPRESSED_BLOCK_SIZE);

    if (DataBuffer == NULL)
    {
        if (m_ReadedDataSize < DataBufferSize)
        {
            delete[] m_ReadedData;
            m_ReadedData = NULL;
            m_ReadedDataSize = 0;
        }

        if (m_ReadedData == NULL)
        {
            m_ReadedData = new uint8_t[DataBufferSize];
            m_ReadedDataSize = DataBufferSize;
        }
        RtlZeroMemory(m_ReadedData, DataBufferSize);
        Buffer = m_ReadedData;
    }
    else
    {
        if (*DataBuffer == NULL) *DataBuffer = new uint8_t[DataBufferSize];
        RtlZeroMemory(*DataBuffer, DataBufferSize);
        Buffer = *DataBuffer;
    }

    if (!read_file_at(GetFileHandle(), Offset, Buffer, DataBufferSize)) return NULL;

    return Buffer;
}

bool
FileContext::OpenFile(
    char * FileName,
    uint32_t Type
)
{
    m_FileHandle = fopen(FileName, "rb");
    if (m_FileHandle == NULL) {
        printf("FileOpen: Could not open file.\n");
        Close();
        return false;
    }

    return true;
}

bool
FileContext::CreateOutputFile(
    char * FileName
)
{
    m_OutFileHandle = fopen(FileName, "wb");
    if (m_OutFileHandle == NULL) {
        printf("FileOpen: Could not open file.\n");
        Close();
        return false;
    }

    return true;
}

bool
FileContext::WriteFile(
    void *Buffer,
    uint32_t NbOfBytesToWrite
)
{
    uint32_t WrittenBytes;
    bool Ret = false;

    WrittenBytes = 0;

    WrittenBytes = (uint32_t)fwrite(Buffer, 1, NbOfBytesToWrite, m_OutFileHandle);
    if (WrittenBytes != NbOfBytesToWrite) {
        printf("error: can't write data. result = 0x%x\n", WrittenBytes);
        goto cleanup;
    }

    Ret = true;

cleanup:
    return Ret;
}

void
FileContext::Close(
)
{
    if (m_FileHandle) {
        fclose(m_FileHandle);
        m_FileHandle = NULL;
    }

    if (m_OutFileHandle)
    {
        fclose(m_OutFileHandle);
        m_OutFileHandle = NULL;
    }
}

FileContext::~FileContext()
{
    Close();

    if (m_ReadedData) delete[] m_ReadedData;
    if (m_PreAllocatedBuffer) delete[] m_PreAllocatedBuffer;
}

uint64_t
FileContext::GetFileSize(
    void
)
{
    uint64_t FileSize = 0;
#if _WIN32
    ::GetFileSizeEx(m_FileHandle, (PLARGE_INTEGER)&FileSize);
#else
    fseek64(m_FileHandle, 0, SEEK_END);
    FileSize = ftell64(m_FileHandle);
#endif

    return FileSize;
}