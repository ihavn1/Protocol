#ifndef _CHECKSUM_H
#define _CHECKSUM_H

#include <stdint.h>
#include <stdbool.h>

#include "messageBuffer.h"

/**
 * \brief Calculates 8-bit 2s complement checksum of the message and append the checksum byte to the message.
 * 
 * The checksum are calculated as an 8-bit checksum by adding all bytes modula 256.
 * 
 * \param[in] message to calculate checksum on.
 */
void addChecksum(messageBuffer_t *message);

/**
 * \brief Check if checksum (last byte of byte message) is ok and remove it from buffer.
 * 
 * \param[in] message buffer to check the checksum on.
 */
bool isChecksumOkAndRemoveIt(messageBuffer_t *message);

#endif // _CHECKSUM_H
