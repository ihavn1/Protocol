#ifndef _MESSAGE_BUFFER_H
#define _MESSAGE_BUFFER_H
#include <stdint.h>

#define MAX_MESSAGE_BUFFER_SIZE 20

typedef struct messageBuffer_st* messageBuffer_t;

messageBuffer_t messageBuffer_create();
void messageBuffer_copyToPayload(messageBuffer_t self, uint8_t buffer[], uint8_t bufferLen);
uint8_t* messageBuffer_getPayloadPointer(messageBuffer_t self);
uint8_t messageBuffer_getPayloadLen(messageBuffer_t self);

#endif // _MESSAGE_BUFFER_H
