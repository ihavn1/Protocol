#include <stdlib.h>
#include <string.h>

#include "messageBuffer.h"

struct messageBuffer_st
{
	uint8_t payloadLen;

	struct 
	{
		uint8_t seqNo : 2;
		uint8_t ackNackSeqNo : 2;
		uint8_t acknowlegde : 1;

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
void messageBuffer_copyToPayload(messageBuffer_t self, uint8_t buffer[], uint8_t bufferLen) {
	memcpy(self->frameStruct.payload, buffer, bufferLen);
	self->payloadLen = bufferLen;
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
uint8_t* messageBuffer_getFramePointer(messageBuffer_t self) {
	return (uint8_t *) &self->frameStruct;
}

// ---------------------------------------------
uint8_t messageBuffer_getFrameLen(messageBuffer_t self) {
	return self->payloadLen+1;
}