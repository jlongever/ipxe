#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <generics.h>
#include <io.h>
#include <ipxe/io.h>

// I/O Port Functions
uint8_t READ_PORT_8( uint16_t port )
{
	uint8_t value;

	value = inb( port );

	return( value );
}

uint16_t READ_PORT_16( uint16_t port )
{
	uint16_t value;

	value = inw( port );

	return( value );
}

uint32_t READ_PORT_32( uint16_t port )
{
	uint32_t value;

	value = inl( port );

	return( value );
}

void WRITE_PORT_8( uint16_t port, uint8_t value )
{
	outb( value, port );
}

void WRITE_PORT_16( uint16_t port, uint16_t value )
{
	outw( value, port );
}

void WRITE_PORT_32( uint16_t port, uint32_t value )
{
	outl( value, port );
}

/* read the current value of the EFLAGS register */
uint32_t get_eflags( void )
{
    uint32_t eflags;
	asm ("pushfl; popl %0": "=r" (eflags));
	return(eflags);
}

/* set the EFLAGS register to the given value */
void put_eflags( uint32_t eflags )
{
	asm ("push %0; popfl":/* no output */: "r" (eflags));
}
