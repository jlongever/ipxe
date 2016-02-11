#ifndef __IO__
#define __IO__
uint8_t READ_PORT_8( uint16_t port );
uint16_t READ_PORT_16( uint16_t port );
uint32_t READ_PORT_32( uint16_t port );
void WRITE_PORT_8( uint16_t port, uint8_t value );
void WRITE_PORT_16( uint16_t port, uint16_t value );
void WRITE_PORT_32( uint16_t port, uint32_t value );

uint32_t get_eflags( void );
void put_eflags( uint32_t eflags );
#endif
