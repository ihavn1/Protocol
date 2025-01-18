#include <string.h>
#include <stdio.h>

#include "messageBuffer.h"
#include "framing.h"
#include "checksum.h"

void printHex(const messageBuffer_t *message)
{
	for (uint8_t i = 1; i < message->messageLen; i++)
	{
		printf("[%02X] ", message->p_packet->payload[i]);
	}

	printf("\n");
}

void call_back(messageBuffer_t message)
{
	printf("After unstuff and unframing\n");
	printHex(&message);

	if (isChecksumOkAndRemoveIt(&message))
	{
		printf("CHECKSUM OK\n");
	}
	else
	{
		printf("CHECKSUM FAILED!!!!!!!!!!!!!\n");
	}

	printf("After Checksum Removed\n");
	printHex(&message);
}

int main()
{
	messageBuffer_t message;

	message.seqNo = 1;
	message.ackNackSeqNo = 2;
	message.acknowlegde = true;

	printf("messageBuffer size: %d\n", (int)sizeof(message));

	uint8_t tmp[] = {0, framing_ESC_BYTE, framing_FLAG_BYTE, 0, 0};

	messageBuffer_setPayloadLength(&message, 5);
	memcpy(message.payload, tmp, 5);

	printf("Before stuff and framing\n");
	printHex(&message);

	// Add checksum
	addChecksum(&message);

	printf("After checksum added\n");
	printHex(&message);

	framing_frameAndStuffMessage(&message);
	printf("After stuff and framing\n");
	printHex(&message);

	framing_create(call_back);

	for (uint8_t i = 0; i < message.messageLen; i++)
	{
		framing_byteReceived(message.payload[i]);
	}

	return 0;
}
