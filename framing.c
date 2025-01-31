#include <stdbool.h>
#include "framing.h"
#include "checksum.h"

enum FRAMING_STATES { IDLE, DATA, ESC };
static void(*_messageReceivedCallBack)(bool ok, uint8_t *pFrame, uint8_t frameLen);
static enum FRAMING_STATES _state;
static uint8_t _frameBuffer[MAX_MESSAGE_BUFFER_SIZE];
static uint8_t _frameBufferLen = 0;

// --------------------------------------------
static void _storeInBuffer(uint8_t byte)
{
	if (_frameBufferLen < MAX_MESSAGE_BUFFER_SIZE)
	{
		_frameBuffer[_frameBufferLen++] = byte;
	}
}

// --------------------------------------------
static bool _isBufferEmpty()
{
	return _frameBufferLen == 0;
}

// --------------------------------------------
static void _gotoState(enum FRAMING_STATES newState)
{
	// Entry New state
	switch (newState)
	{
	case IDLE:
		_frameBufferLen = 0;
		break;

	default:
		break;
	}

	_state = newState;
}

// --------------------------------------------
void framing_create(void(*messageReceived)(uint8_t* pFrame, uint8_t frameLen))
{
	_gotoState(IDLE);
	_messageReceivedCallBack = messageReceived;
	_frameBufferLen =0;
}

// --------------------------------------------
void framing_destroy()
{
	// Nothing to clean up here
}

// --------------------------------------------
framing_status_t framing_frameAndStuffMessage(messageBuffer_t message, uint8_t *pFrame, uint8_t *len)
{
	uint8_t _rawMessage[MAX_MESSAGE_BUFFER_SIZE] = { 0 };
	uint8_t _rawMessageLen = 0;
	
	*len = 0;

	messageBuffer_getRawMessage(message, _rawMessage, &_rawMessageLen);
	addChecksum(_rawMessage, &_rawMessageLen);

	pFrame[(*len)++] = framing_FLAG_BYTE;

	for (uint8_t src = 0; src < _rawMessageLen; src++)
	{
		if (_rawMessage[src] == framing_FLAG_BYTE || _rawMessage[src] == framing_ESC_BYTE)
		{
			if (*len >= MAX_MESSAGE_BUFFER_SIZE - 1)
				return BUFFER_TO_SMALL;

			pFrame[(*len)++] = framing_ESC_BYTE;
		}

		if (*len >= MAX_MESSAGE_BUFFER_SIZE - 1)
			return BUFFER_TO_SMALL;

		pFrame[(*len)++] = _rawMessage[src];
	}

	pFrame[(*len)++] = framing_FLAG_BYTE;

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
			_frameBufferLen = 0;;
		}
		else if (byte == framing_FLAG_BYTE && !_isBufferEmpty())
		{
			if (isChecksumOkAndRemoveIt(_frameBuffer, &_frameBufferLen)) {
				_messageReceivedCallBack(true, _frameBuffer, _frameBufferLen);
			}
			else {
				_messageReceivedCallBack(false, _frameBuffer, _frameBufferLen);
			}
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
