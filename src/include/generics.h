#ifndef GENERICS_H
#define GENERICS_H

#define BYTE 	unsigned char
#define WORD 	unsigned short
#define LWORD	unsigned long

#define INT8	signed char
#define INT16	short
#define INT32 	int
#define INT64	__int64

#define UINT8	unsigned char
#define UINT16	unsigned short
#define UINT32	unsigned int
#define UINT64  unsigned __int64

typedef char CHAR;
typedef int  INT_32;
typedef int  INT_16;
typedef unsigned int  UINT;
typedef unsigned int  UINT_32;
typedef unsigned short  UINT_16;
typedef unsigned char UINT_8;

typedef signed char INT_8E;
typedef unsigned short	BITS_16E;
typedef unsigned int	BITS_32E;


typedef short		INT_16E;
typedef int		INT_32E;
typedef long long	INT_64E;

typedef unsigned char	UINT_8E;
typedef unsigned short	UINT_16E;
typedef unsigned int	UINT_32E;

typedef unsigned long long UINT_64E;
typedef unsigned long long UINT_64;

#define KB_1	0x00000400
#define KB_2	0x00000800
#define KB_4	0x00001000
#define KB_8	0x00002000
#define KB_16	0x00004000
#define KB_28	0x00007000
#define KB_32	0x00008000
#define KB_64	0x00010000
#define KB_96   0x00018000
#define KB_128	0x00020000
#define KB_256	0x00040000
#define KB_512	0x00080000
#define MB_1	0x00100000
#define MB_2	0x00200000
#define MB_4	0x00400000		// Memory Manager
#define MB_5	0x00500000
#define MB_6	0x00600000		// Memory Manager
#define MB_8	0x00800000		// Memory Manager
#define MB_16	0x01000000
#define MB_32	0x02000000
#define MB_64	0x04000000
#define MB_128	0x08000000
#define MB_256	0x10000000
#define MB_512	0x20000000
#define GB_1	0x40000000
#define GB_2	0x80000000
#define GB_3	0xC0000000
#define GB_4	0x100000000
#define GB_5	0x140000000
#define GB_6	0x180000000
#define GB_7	0x1C0000000
#define GB_8	0x200000000
#define GB_16	0x400000000
#define GB_32	0x800000000

typedef struct time_date
{
	UINT_8E	seconds;
	UINT_8E minutes;
	UINT_8E hour;
	UINT_8E day;
	UINT_8E date;
	UINT_8E month;
	UINT_8E year;
	UINT_8E century;
} TIME_DATE;

#endif
