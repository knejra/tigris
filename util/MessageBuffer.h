#ifndef _MESSAGE_BUFFER_H
#define _MESSAGE_BUFFER_H

#include "Types.h"

#define offsetof(type, member) ((size_t) &((type *)0)->member)

struct MessageBuffer
{
    uint16_t fixed;
    uint64_t msglen;
    uint64_t msgId;
    uint8_t msgType;
    uint32_t from; // attention
    char *message;
};

typedef struct MessageBuffer MessageBuffer;

#endif // _MESSAGE_BUFFER_H