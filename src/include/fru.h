#ifndef _FRU_H
#define _FRU_H

#include <generics.h>
#include <ipmi.h>
#include <ipmi_msgdefs.h>

///////////////////////////////////////////////////////////////////////////////////////
///
/// @name   FRU IPMI
/// @brief  This section contains FRU IPMI related constructs
///
///////////////////////////////////////////////////////////////////////////////////////
//@{

#define MAX_FRU_READ_SIZE  (22)
#define MAX_FRU_WRITE_SIZE (MAX_FRU_READ_SIZE)

#define FRU_AREA_INFO  (0x10)
#define FRU_READ       (0x11)
#define FRU_WRITE      (0x12)

#define MAX_FRU_ID     (254)

typedef struct __attribute__((__packed__))
{
    UINT_8  netfn_lun;
    UINT_8  command;
    UINT_8  ccode;
    UINT_16 size;             // Inventory area size in bytes
    UINT_8  access  : 1;      // 0 = byte access, 1 = word access
    UINT_8          : 7;      // Reserved
} fru_area_info_t;

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Message structure for an FRU Read Request
///
////////////////////////////////////////////////////////////////////////////////
typedef struct __attribute__((__packed__))
{
    UINT_8  netfn_lun;
    UINT_8  command;
    UINT_8  fru_id;
    UINT_16 offset;
    UINT_8  count;
} read_fru_request_t;

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Message structure for an FRU Read Response
///
////////////////////////////////////////////////////////////////////////////////
typedef struct __attribute__((__packed__))
{
    UINT_8 netfn_lun;
    UINT_8 command;
    UINT_8 ccode;
    UINT_8 count;
    UINT_8 data[MAX_FRU_READ_SIZE];
} read_fru_response_t;

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Message structure for an FRU Write Request
///
////////////////////////////////////////////////////////////////////////////////
typedef struct  __attribute__((__packed__))
{
    UINT_8  netfn_lun;
    UINT_8  command;
    UINT_8  fru_id;
    UINT_16 offset;
    UINT_8  data[MAX_FRU_WRITE_SIZE];
} write_fru_request_t;

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Message structure for an FRU Write Response
///
////////////////////////////////////////////////////////////////////////////////
typedef struct __attribute__((__packed__))
{
    UINT_8 netfn_lun;
    UINT_8 command;
    UINT_8 ccode;
    UINT_8 count;      // Number of bytes written
} write_fru_response_t;

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Message structure for an FRU Write Response
///
////////////////////////////////////////////////////////////////////////////////
typedef struct __attribute__ ((__packed__))
{
    UINT_8  netfn_lun;
    UINT_8  command;
    uint8_t format_version;
    uint8_t internal_area;
    uint8_t chassis_area;
    uint8_t board_area;
    uint8_t product_area;
    uint8_t multi_area;
    uint8_t pad;
    uint8_t checksum;
} fru_common_header_t;

/* Prototypes */
UINT_32 fru_write( write_fru_request_t *req, write_fru_response_t *rsp, UINT_16 num_bytes );
UINT_32 fru_read( read_fru_request_t *req, read_fru_response_t *rsp );
int fru_print_all( void );

//@}

#endif
