#ifndef COBS_H_
#define COBS_H_

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/*
   cobs Encode in place does the encoding on the same array passed in, and does
   not require using a different array. This may be preferable for performance
   reasons. Encode in place assumes that the first byte is the header byte and
   the second byte is the code byte.
   */
void cobs_encodeInPlace(uint8_t* buffer, const uint8_t total_length);

/*
   For destination, pass in an array that is 2 bytes larger than the source
   length.
   */
void cobs_encode(const uint8_t* source,
    const uint8_t length,
    uint8_t* destination);


/*
   Decode in place does the decoding on the same array passed in. Decode in place
   assumes that the first byte is the header byte and the second byte is the code
   byte.
   */
void cobs_decodeInPlace(uint8_t* buffer,
    const uint8_t length);


/*
   For destination, pass in an array that is at least as large as the source
   length minus 2.
   Strips the  header and code byte from the data.
   */
void cobs_decode(const uint8_t* source,
    const uint8_t length,
    uint8_t* destination);

#endif
