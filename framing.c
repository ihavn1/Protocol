#include <stdbool.h>
#include "framing.h"

enum FRAMING_STATES { IDLE, DATA, ESC };
static void(*_messageReceivedCallBack)(messageBuffer_t message);
static enum FRAMING_STATES _state;
static messageBuffer_t _buffer;

// --------------------------------------------
static void _clear_buffer()
{
	_buffer.messageLen = 0;
}

// --------------------------------------------
static void _storeInBuffer(uint8_t byte)
{
	if (_buffer.messageLen < MAX_MESSAGE_BUFFER_SIZE)
	{
		_buffer.payload[_buffer.messageLen++] = byte;
	}
}

// --------------------------------------------
static bool _isBufferEmpty()
{
	return _buffer.messageLen == 0;
}

// --------------------------------------------
static void _gotoState(enum FRAMING_STATES newState)
{
	// Entry New state
	switch (newState)
	{
	case IDLE:
		_clear_buffer();
		break;

	default:
		break;
	}

	_state = newState;
}

// --------------------------------------------
void framing_create(void(*messageReceived)(messageBuffer_t message))
{
	_gotoState(IDLE);
	_messageReceivedCallBack = messageReceived;
}

// --------------------------------------------
void framing_destroy()
{
	// Nothing to clean up here
}

// --------------------------------------------
framing_status_t framing_frameAndStuffMessage(messageBuffer_t* message)
{
	uint8_t dst = 0;
	uint8_t dst_buffer[MAX_MESSAGE_BUFFER_SIZE];

	dst_buffer[dst++] = framing_FLAG_BYTE;

	for (uint8_t src = 0; src < message->messageLen; src++)
	{
		if (message->p_packet->payload[src] == framing_FLAG_BYTE || message->p_packet->payload[src] == framing_ESC_BYTE)
		{
			if (dst >= MAX_MESSAGE_BUFFER_SIZE - 1)
				return BUFFER_TO_SMALL;

			dst_buffer[dst++] = framing_ESC_BYTE;
		}

		if (dst >= MAX_MESSAGE_BUFFER_SIZE - 1)
			return BUFFER_TO_SMALL;

		dst_buffer[dst++] = message->p_packet->payload[src];
	}

	dst_buffer[dst++] = framing_FLAG_BYTE;

	for (uint8_t i = 0; i < dst; i++)
	{
		message->p_packet->payload[dst] = dst_buffer[i];
	}

	message->messageLen = dst + 1;

	return OK;
}

// --------------------------------------------
void framing_byteReceived(uint8_t byte)
{
	switch (_state)
	{
	case IDLE:
		if (byte == framing_FLAG_BYTE)
		{
			_gotoState(DATA);
		}
		break;

	case DATA:
		if (byte == framing_ESC_BYTE)
		{
			_gotoState(ESC);
		}
		else if (byte == framing_FLAG_BYTE && _isBufferEmpty())
		{
			_clear_buffer();
		}
		else if (byte == framing_FLAG_BYTE && !_isBufferEmpty())
		{
			_messageReceivedCallBack(_buffer);
			_gotoState(IDLE);
		}
		else
		{
			_storeInBuffer(byte);
		}
		break;

	case ESC:
		if (byte == framing_FLAG_BYTE || byte == framing_ESC_BYTE)
		{
			_storeInBuffer(byte);
			_gotoState(DATA);
		}
		else
		{
			_gotoState(IDLE);
		}
		break;

	default:
		break;
	}
}
