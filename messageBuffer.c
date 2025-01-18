#include "messageBuffer.h"

void messageBuffer_setPayloadLength(messageBuffer_t *message, uint8_t len) {
    message->messageLen = len+1;
}