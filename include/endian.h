#ifndef ENDIAN_H
#define ENDIAN_H

#include <stdint.h>

//! Byte swap unsigned short
uint16_t swap_uint16( uint16_t val );
int16_t swap_int16( int16_t val ) ;

//! Byte swap unsigned int
uint32_t swap_uint32( uint32_t val );
int32_t swap_int32( int32_t val );

int64_t swap_int64( int64_t val );
uint64_t swap_uint64( uint64_t val );


#endif /* ENDIAN_H */
