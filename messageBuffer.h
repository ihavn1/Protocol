#ifndef _MESSAGE_BUFFER_H
#define _MESSAGE_BUFFER_H
#include <stdint.h>
#include <stdbool.h>

#define MAX_MESSAGE_BUFFER_SIZE 20

typedef struct messageBuffer_st* messageBuffer_t;

messageBuffer_t messageBuffer_create();
void messageBuffer_reset(messageBuffer_t self);
void messageBuffer_copyToPayload(messageBuffer_t self, uint8_t buffer[], uint8_t bufferLen);
void messageBuffer_copyToFrame(messageBuffer_t self, uint8_t buffer[], uint8_t bufferLen);
void messageBuffer_AppendByte(messageBuffer_t self, uint8_t byte);
void messageBuffer_removeByte(messageBuffer_t self);
uint8_t* messageBuffer_getPayloadPointer(messageBuffer_t self);
uint8_t messageBuffer_getPayloadLen(messageBuffer_t self);

void messageBuffer_setFrameSeqNo(messageBuffer_t self, uint8_t seqNo);
void messageBuffer_setFrameAckNakSeqNo(messageBuffer_t self, uint8_t seqNo);
void messageBuffer_setFrameAckowledge(messageBuffer_t self, bool ack);
void messageBuffer_getRawMessage(messageBuffer_t self, uint8_t* pDest, uint8_t* len);

#endif // _MESSAGE_BUFFER_H
