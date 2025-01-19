#include <stdbool.h>
#include "framing.h"

enum FRAMING_STATES { IDLE, DATA, ESC };
static void(*_messageReceivedCallBack)(messageBuffer_t message);
static enum FRAMING_STATES _state;
static uint8_t _buffer[MAX_MESSAGE_BUFFER_SIZE];
static uint8_t _bufferLen = 0;

// --------------------------------------------
static void _storeInBuffer(uint8_t byte)
{
	if (_bufferLen < MAX_MESSAGE_BUFFER_SIZE)
	{
		_buffer[_bufferLen++] = byte;
	}
}

// --------------------------------------------
static bool _isBufferEmpty()
{
	return _bufferLen == 0;
}

// --------------------------------------------
static void _gotoState(enum FRAMING_STATES newState)
{
	// Entry New state
	switch (newState)
	{
	case IDLE:
		_bufferLen = 0;
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
	_bufferLen=0;
}

// --------------------------------------------
void framing_destroy()
{
	// Nothing to clean up here
}

// --------------------------------------------
framing_status_t framing_frameAndStuffMessage(messageBuffer_t message)
{
	uint8_t dst = 0;
	uint8_t dst_buffer[MAX_MESSAGE_BUFFER_SIZE];

	dst_buffer[dst++] = framing_FLAG_BYTE;

	for (uint8_t src = 0; src < messageBuffer_getFrameLen(message); src++)
	{
		if (messageBuffer_getFramePointer(message)[src] == framing_FLAG_BYTE || messageBuffer_getFramePointer(message)[src] == framing_ESC_BYTE)
		{
			if (dst >= MAX_MESSAGE_BUFFER_SIZE - 1)
				return BUFFER_TO_SMALL;

			dst_buffer[dst++] = framing_ESC_BYTE;
		}

		if (dst >= MAX_MESSAGE_BUFFER_SIZE - 1)
			return BUFFER_TO_SMALL;

		dst_buffer[dst++] = messageBuffer_getFramePointer(message)[src];
	}

	dst_buffer[dst++] = framing_FLAG_BYTE;

	messageBuffer_copyToFrame(message, dst_buffer, dst);

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
			_bufferLen = 0;;
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
