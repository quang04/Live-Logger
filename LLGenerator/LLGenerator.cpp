#include <iostream>
#include <afxwin.h>
#include <string>
#include <conio.h>
#include <format>

#define DV_BUFFER_ENTRY "LL_BUFFER_ENTRY"
#define DV_BUFFER_READY "LL_BUFFER_READY"
#define DV_BUFFER_DATA "LL_BUFFER_DATA"

#define DV_BUFFER_DATA_SIZE 4096

struct DVBuffer
{
    UINT8 tab;
    char data[DV_BUFFER_DATA_SIZE];
};

bool LogToLL(const std::string& msg, UINT8 tab)
{
    auto hBufferEntry = OpenEvent(EVENT_ALL_ACCESS,
        FALSE,
        DV_BUFFER_ENTRY);

    if (hBufferEntry == NULL) return false;

    auto hBufferReady = OpenEvent(EVENT_MODIFY_STATE, FALSE, DV_BUFFER_READY);

    if (hBufferReady == NULL)
    {
        CloseHandle(hBufferEntry);
        return false;
    }

    auto hShareFile = CreateFileMapping(INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        sizeof(DVBuffer),
        DV_BUFFER_DATA);

    if (hShareFile == NULL)
    {
        CloseHandle(hBufferEntry);
        CloseHandle(hBufferReady);
        return false;
    }

    PSTR pShareMem = (LPSTR)MapViewOfFile(hShareFile,
        FILE_MAP_WRITE,
        0,
        0,
        0);

    if (!pShareMem)
    {
        CloseHandle(hShareFile);
        CloseHandle(hBufferEntry);
        CloseHandle(hBufferReady);
        return false;
    }


    // wait at here to make sure data sync
    WaitForSingleObject(hBufferEntry, INFINITE);

    char data[DV_BUFFER_DATA_SIZE];
    memcpy(data, msg.c_str(), DV_BUFFER_DATA_SIZE);


    // create buffer data for mapping
    sprintf_s(pShareMem, sizeof(DVBuffer), "%c", tab);
    sprintf_s(pShareMem + sizeof(UINT8), sizeof(DVBuffer) - sizeof(UINT8), "%s", data);


    // signal data ready event
    SetEvent(hBufferReady);

    // clean up handles
    UnmapViewOfFile(pShareMem);
    CloseHandle(hShareFile);
    CloseHandle(hBufferEntry);
    CloseHandle(hBufferReady);

    return true;
}

int main()
{
    std::cout << "Press any key to stop spam data" << std::endl;

    int i = 0;
    unsigned int nTab = 0;
    const int totalTab = 5;
    while (!_kbhit())
    {

        auto msg = std::format("Message from process {} %, msg id: {}", GetCurrentProcessId(), ++i);

        nTab = i % (totalTab);
        LogToLL(msg, nTab);
        Sleep(100);
    }
}

