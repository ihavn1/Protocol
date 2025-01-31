#include <string.h>
#include <stdio.h>

#include "messageBuffer.h"
#include "framing.h"

void printHex(const uint8_t *buffer, uint8_t bufferLen)
{
	for (uint8_t i = 0; i < bufferLen; i++)
	{
		printf("[%02X] ", buffer[i]);
	}

	printf("\n");
}

static void call_back(bool ok, uint8_t frame[], uint8_t frameLen)
{
	printf("After unstuff and unframing\n");
	printHex(frame, frameLen);

	if (ok)
	{
		printf("CHECKSUM OK\n");
	}
	else
	{
		printf("CHECKSUM FAILED!!!!!!!!!!!!!\n");
	}
}

int main()
{
	messageBuffer_t message = messageBuffer_create();
	uint8_t workBuffer[MAX_MESSAGE_BUFFER_SIZE] = { 0 };
	uint8_t workBufferLen = 0;

	messageBuffer_setFrameSeqNo(message, 1);
	messageBuffer_setFrameSeqNo(message, 2);
	messageBuffer_setFrameAckowledge(message, true);

	uint8_t tmp[] = {0, framing_ESC_BYTE, framing_FLAG_BYTE, 0, 0};

	messageBuffer_copyToPayload(message, tmp, sizeof(tmp));

	printf("Before stuff and framing\n");
	messageBuffer_getRawMessage(message, workBuffer, &workBufferLen);
	printHex(workBuffer,workBufferLen);

	framing_frameAndStuffMessage(message,workBuffer, &workBufferLen);
	printf("After stuff and framing\n");
	printHex(workBuffer, workBufferLen);

	framing_create(call_back);

	for (uint8_t i = 0; i < workBufferLen; i++)
	{
		framing_byteReceived(workBuffer[i]);
	}

	return 0;
}
