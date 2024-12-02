#pragma once

#include "Resource.h"

#define DV_BUFFER_DATA_SIZE 4096

struct DVBuffer
{
    UINT8 tab;
    char data[DV_BUFFER_DATA_SIZE];

    DVBuffer()
        : tab(0)
    {
        memset(data, '\0', DV_BUFFER_DATA_SIZE);
    }

    DVBuffer(const DVBuffer& other)
        : tab(other.tab)
    {
        memcpy(data, other.data, DV_BUFFER_DATA_SIZE);
    }

    DVBuffer(DVBuffer&& other)
        : tab(other.tab)
    {
        memcpy(data, other.data, DV_BUFFER_DATA_SIZE);
    }

};

#define MAX_TAB 3
#define MAX_LINE 10000


// pipe entry acts like mutex
#define DV_BUFFER_ENTRY "LL_BUFFER_ENTRY"
// pipe ready
#define DV_BUFFER_READY "LL_BUFFER_READY"
// pipe data
#define DV_BUFFER_DATA "LL_BUFFER_DATA"



// 100ms
#define DV_WAIT_DATA_TIMEOUT 100

