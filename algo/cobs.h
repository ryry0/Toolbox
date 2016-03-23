#ifndef COBS_H_
#define COBS_H_

#include <string.h>
#include <stdlib.h>

/*
   cobs Encode in place does the encoding on the same array passed in, and does
   not require using a different array. This may be preferable for performance
   reasons. Encode in place assumes that the first byte is the header byte and
   the second byte is the code byte.
   */
void cobsEncodeInPlace(char* buffer, const unsigned int length);

/*
   on non-embedded systems, should return size_t, and should be size_t length.
   Same thing applies to cobes decode.
   For destination, pass in an array that is 2 bytes larger than the source
   length.
   */
void cobsEncode(const char* source,
    const unsigned int length,
    char* destination);


/*
   Decode in place does the decoding on the same array passed in. Decode in place
   assumes that the first byte is the header byte and the second byte is the code
   byte.
   */
void cobsDecodeInPlace(char* buffer,
    const unsigned int length);


/*
   on non-embedded systems, should return size_t, and should be size_t length.
   Same thing applies to cobes decode.
   For destination, pass in an array that is at least as large as the source
   length minus 2.
   Strips the  header and code byte from the data.
   */
void cobsDecode(const char* source,
    const unsigned int length,
    char* destination);

#endif
