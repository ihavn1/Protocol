#ifndef _FRAMING_H
#define _FRAMING_H
#include <stdint.h>

#include "messageBuffer.h"

#define framing_FLAG_BYTE			0x55
#define framing_ESC_BYTE			0xAA



typedef enum { OK, BUFFER_TO_SMALL } framing_status_t;

/**
 * \brief Create and initialise the the framing and unframing module.
 * 
 * \note Only call once for initialition.
 * 
 * \param[in] messageReceived pointer to call back function that will be called
 *			  when a message is unframed and unstuffed succesfully.
 */
void framing_create(void(*messageReceived)(messageBuffer_t message));

/**
 * \brief Destroys the framing and unframing module.
 */
void framing_destroy();

/**
 * \brief Stuff and frame a message.
 * \param[in,out] message to be stuffed and framed result returned.
 * \return status.
 * \retval	OK message is now stuffed and framed.
 * \retval  BUFFER_TO_SMALL message to small to hold stuffing and framing.
 */
framing_status_t framing_frameAndStuffMessage(messageBuffer_t *message);

/**
 * \brief Handles individual payload received from the the underlaying layer.
 * \param [in] byte last payload received from underlaying layer.
 */
void framing_byteReceived(uint8_t byte);

#endif // _FRAMING_H