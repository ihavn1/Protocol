#include <stdlib.h>
#include <string.h>

#include "messageBuffer.h"

struct messageBuffer_st
{
	uint8_t payloadLen;

	struct {
		uint8_t seqNo : 2;
		uint8_t ackNackSeqNo : 2;
		uint8_t acknowledge : 1;
		uint8_t reserved : 3;  // To make the total bits 8

		uint8_t payload[MAX_MESSAGE_BUFFER_SIZE];
	} frameStruct;
};

// ---------------------------------------------
messageBuffer_t messageBuffer_create() {
	messageBuffer_t self = malloc(sizeof(struct messageBuffer_st));

	if (self) {
		self->payloadLen = 0;
	}

	return self;
}

// ---------------------------------------------
void messageBuffer_reset(messageBuffer_t self) {
	self->payloadLen = 0;
}

// ---------------------------------------------
void messageBuffer_copyToPayload(messageBuffer_t self, uint8_t buffer[], uint8_t bufferLen) {
	memcpy(self->frameStruct.payload, buffer, bufferLen);
	self->payloadLen = bufferLen;
}

// ---------------------------------------------
void messageBuffer_copyToFrame(messageBuffer_t self, uint8_t buffer[], uint8_t bufferLen) {
	// memcpy(messageBuffer_getFramePointer(self), buffer, bufferLen);
	self->payloadLen = bufferLen-1;
}

// ---------------------------------------------
void messageBuffer_AppendByte(messageBuffer_t self, uint8_t byte) {
	self->frameStruct.payload[self->payloadLen++] = byte;

}

// ---------------------------------------------
void messageBuffer_removeByte(messageBuffer_t self) {
	if (self->payloadLen > 0) {
		self->payloadLen--;
	}
}

// ---------------------------------------------
uint8_t* messageBuffer_getPayloadPointer(messageBuffer_t self) {
	return self->frameStruct.payload;
}

// ---------------------------------------------
uint8_t messageBuffer_getPayloadLen(messageBuffer_t self) {
	return self->payloadLen;
}

// ---------------------------------------------
void messageBuffer_setFrameSeqNo(messageBuffer_t self, uint8_t seqNo)
{
	self->frameStruct.seqNo = seqNo;
}

// ---------------------------------------------
void messageBuffer_setFrameAckNakSeqNo(messageBuffer_t self, uint8_t seqNo)
{
	self->frameStruct.ackNackSeqNo = seqNo;
}

// ---------------------------------------------
void messageBuffer_setFrameAckowledge(messageBuffer_t self, bool ack)
{
	self->frameStruct.acknowledge = ack;
}

// ---------------------------------------------
uint8_t messageBuffer_getRawMessageLen(messageBuffer_t self) {
	return self->payloadLen + 1;
}

// ---------------------------------------------
void messageBuffer_getRawMessage(messageBuffer_t self, uint8_t * pDest, uint8_t *len) {
	if (self->payloadLen != 0) {
		memcpy(pDest, (uint8_t*)&self->frameStruct, messageBuffer_getRawMessageLen(self));
	}
	*len = messageBuffer_getRawMessageLen(self);
}
