#include <string.h>
#include <stdio.h>

#include "messageBuffer.h"
#include "framing.h"
#include "checksum.h"

void printHex(const uint8_t *buffer, uint8_t bufferLen)
{
	for (uint8_t i = 1; i < bufferLen; i++)
	{
		printf("[%02X] ", buffer[i]);
	}

	printf("\n");
}

static void call_back(messageBuffer_t message)
{
	printf("After unstuff and unframing\n");
	printHex(messageBuffer_getFramePointer(message),messageBuffer_getFrameLen(message));

	if (isChecksumOkAndRemoveIt(message))
	{
		printf("CHECKSUM OK\n");
	}
	else
	{
		printf("CHECKSUM FAILED!!!!!!!!!!!!!\n");
	}

	printf("After Checksum Removed\n");
	printHex(messageBuffer_getFramePointer(message),messageBuffer_getFrameLen(message));
}

int main()
{
	messageBuffer_t message = messageBuffer_create();

	// 
	messageBuffer_setFrameSeqNo(message, 1);
	messageBuffer_setFrameSeqNo(message, 2);
	messageBuffer_setFrameAckowledge(message, true);

	uint8_t tmp[] = {0, framing_ESC_BYTE, framing_FLAG_BYTE, 0, 0};

	messageBuffer_copyToPayload(message, tmp, sizeof(tmp));

	printf("Before stuff and framing\n");
	printHex(messageBuffer_getFramePointer(message),messageBuffer_getFrameLen(message));

	// Add checksum
	addChecksum(message);
	printf("After checksum added\n");
	printHex(messageBuffer_getFramePointer(message),messageBuffer_getFrameLen(message));

	framing_frameAndStuffMessage(message);
	printf("After stuff and framing\n");
	printHex(messageBuffer_getFramePointer(message), messageBuffer_getFrameLen(message));

	framing_create(call_back);

	for (uint8_t i = 0; i < messageBuffer_getFrameLen(message); i++)
	{
		framing_byteReceived(messageBuffer_getFramePointer(message)[i]);
	}

	return 0;
}
