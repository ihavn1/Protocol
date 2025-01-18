#include "checksum.h"

// ----------------------------------------------
static uint8_t _checksum(messageBuffer_t *message) {
    uint8_t sum = 0;
    uint8_t * totalMessage = (uint8_t *)message;

    for (uint8_t i=1; i<(message->messageLen); i++) {
        sum += totalMessage[i];
    }

    return ~sum+1;
}

// ----------------------------------------------
void addChecksum(messageBuffer_t *message) {
    uint8_t * totalMessage = (uint8_t *)message;
    totalMessage[message->messageLen++] = _checksum(message);
}

// ----------------------------------------------
bool isChecksumOkAndRemoveIt(messageBuffer_t *message) {
    if (0 == _checksum(message)) {
        message->messageLen--;
        return true;
    }

    return false;
}