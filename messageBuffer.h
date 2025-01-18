#ifndef _MESSAGE_BUFFER_H
#define _MESSAGE_BUFFER_H
#include <stdint.h>

#define MAX_MESSAGE_BUFFER_SIZE 20

typedef struct
{
	uint8_t messageLen;

	union peter
	{
		struct frameStruct
		{
			uint8_t seqNo : 2;
			uint8_t ackNackSeqNo : 2;
			uint8_t acknowlegde : 1;

			uint8_t payload[MAX_MESSAGE_BUFFER_SIZE];
		};

		struct frameStruct *packet;
	};

} messageBuffer_t;

void messageBuffer_setPayloadLength(messageBuffer_t *message, uint8_t len);

#endif // _MESSAGE_BUFFER_H
