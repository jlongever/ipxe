#ifndef _KCS_H
#define _KCS_H

#include <generics.h>

///////////////////////////////////////////////////////////////////////////////////////
///
/// @name   KCS Driver
/// @brief  This section contains KCS Driver related constructs
///
///////////////////////////////////////////////////////////////////////////////////////
//@{

#define MAX_REQ_RETRIES     20
#define MAX_KCS_RETRIES     100000

#define KCS_DRIVER_CONTROL_WRITE_START         0x61
#define KCS_DRIVER_CONTROL_WRITE_END           0x62
#define KCS_DRIVER_CONTROL_READ                0x68
#define KCS_DRIVER_CONTROL_GET_STATUS_ABORT    0x60

#define   KCS_DRIVER_ERROR_SHIFT               0x10

#define   KCS_DRIVER_ERROR_NOT_WRITE_STATE__WRITE_START     0x01 << KCS_DRIVER_ERROR_SHIFT
#define   KCS_DRIVER_ERROR_NOT_WRITE_STATE__WRITE_DURING    0x02 << KCS_DRIVER_ERROR_SHIFT
#define   KCS_DRIVER_ERROR_NOT_WRITE_STATE__WRITE_END       0x04 << KCS_DRIVER_ERROR_SHIFT
#define   KCS_DRIVER_ERROR_NOT_IDLE_STATE__READ_DURING      0x08 << KCS_DRIVER_ERROR_SHIFT
#define   KCS_DRIVER_ERROR_NOT_IDLE_STATE__DRIVER_START     0x10 << KCS_DRIVER_ERROR_SHIFT
#define   KCS_DRIVER_ERROR_COMMS_FAILURE                    0x20 << KCS_DRIVER_ERROR_SHIFT
#define   KCS_DRIVER_ERROR_NOT_READY                        0x40 << KCS_DRIVER_ERROR_SHIFT

// Status Code
#define   KCS_DRIVER_ERROR_NO_ERROR                   0x00
#define   KCS_DRIVER_ERROR_ABORTED_BY_COMMAND         0x01
#define   KCS_DRIVER_ERROR_ILLEGAL_CONTROL_CODE       0x02
#define   KCS_DRIVER_ERROR_LENGTH_ERROR               0x06

#define KCS_DRIVER_REGISTERS_DATA_IN_ADDR   0xCA8
#define KCS_DRIVER_REGISTERS_DATA_OUT_ADDR  0xCA8
#define KCS_DRIVER_REGISTERS_COMMAND_ADDR  0xCAC
#define KCS_DRIVER_REGISTERS_STATUS_ADDR  0xCAC

#define   KCS_DRIVER_STATUS_IDLE_STATE     0x00
#define   KCS_DRIVER_STATUS_READ_STATE     0x01
#define   KCS_DRIVER_STATUS_WRITE_STATE    0x02
#define   KCS_DRIVER_STATUS_ERROR_STATE    0x03


////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Message structure for sending IPMI commands over KCS
///
////////////////////////////////////////////////////////////////////////////////
typedef struct __attribute__((__packed__))
{
    UINT_8      data_length;    ///< Length of the command
    UINT_8  *   data;           ///< Content of the command

}   KCS_DRIVER_message_t;


////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Structure for the KCS Interface Status Register
///
////////////////////////////////////////////////////////////////////////////////
typedef struct __attribute__((__packed__))
{
    UINT_8      obf:1;
    UINT_8      ibf:1;
    UINT_8      sms_atn:1;
    UINT_8      c_d_bit:1;
    UINT_8      oem_bits:2;
    UINT_8      state_bits:2;

}   KCS_DRIVER_STATUS_status_register_t;


////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Write a request message to the BMC via KCS
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_write( KCS_DRIVER_message_t * request );

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Read a response message from the BMC via KCS
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_read( KCS_DRIVER_message_t * response );

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Calculate error status
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_error();


////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Decode error status and/or completion codes.
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_decode_status( UINT_32 status, KCS_DRIVER_message_t * request, KCS_DRIVER_message_t * response );


////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Execute an exchange with the BMC over KCS
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_execute( KCS_DRIVER_message_t * request, KCS_DRIVER_message_t * response );

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Check if KCS Interface is in the WRITE state
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_STATUS_is_write_state();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Check if KCS Interface is in the READ state
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_STATUS_is_read_state();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Check if KCS Interface is in the ERROR state
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_STATUS_is_error_state();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Check if KCS Interface is in the IDLE state
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_STATUS_is_idle_state();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Low-level Interface Read
///
////////////////////////////////////////////////////////////////////////////////
UINT_8  KCS_DRIVER_CONTROL__read( UINT_16 reg );

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Low-level Interface Write
///
////////////////////////////////////////////////////////////////////////////////
void    KCS_DRIVER_CONTROL__write( UINT_16 reg, UINT_8 data );

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Low-Level write to the data register
///
////////////////////////////////////////////////////////////////////////////////
void KCS_DRIVER_CONTROL_write_data( UINT_8 data );

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Low-Level read to the data register
///
////////////////////////////////////////////////////////////////////////////////
UINT_8 KCS_DRIVER_CONTROL_read_data();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Wait for the KCS Interface to be ready
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_CONTROL_wait_ready();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Wait for the IBF bit to be cleared
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_CONTROL_wait_ibf_zero();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Wait for the OBF bit to be set
///
////////////////////////////////////////////////////////////////////////////////
UINT_32 KCS_DRIVER_CONTROL_wait_obf_one();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Force clear the OBF bit
///
////////////////////////////////////////////////////////////////////////////////
void KCS_DRIVER_CONTROL_clear_obf();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Start the KCS Write Sequence
///
////////////////////////////////////////////////////////////////////////////////
void KCS_DRIVER_CONTROL_start_write();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  End the KCS Write Sequence
///
////////////////////////////////////////////////////////////////////////////////
void KCS_DRIVER_CONTROL_end_write();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Move KCS to the next Read byte
///
////////////////////////////////////////////////////////////////////////////////
void KCS_DRIVER_CONTROL_read_next();

////////////////////////////////////////////////////////////////////////////////
///
/// @brief  Read the status abort register.
///
////////////////////////////////////////////////////////////////////////////////
void KCS_DRIVER_CONTROL_get_status_abort();

//@}

UINT_32 KCS_DRIVER_write( KCS_DRIVER_message_t * request );
UINT_32 KCS_DRIVER_read( KCS_DRIVER_message_t * response );
UINT_32 KCS_DRIVER_error();
UINT_32 KCS_DRIVER_execute( KCS_DRIVER_message_t * request, KCS_DRIVER_message_t * response );
UINT_32 KCS_DRIVER_STATUS_is_write_state();
UINT_32 KCS_DRIVER_STATUS_is_read_state();
UINT_32 KCS_DRIVER_STATUS_is_error_state();
UINT_32 KCS_DRIVER_STATUS_is_idle_state();
UINT_8 KCS_DRIVER_CONTROL__read( UINT_16 reg );
void KCS_DRIVER_CONTROL__write( UINT_16 reg, UINT_8 data );
void KCS_DRIVER_CONTROL_write_data( UINT_8 data );
UINT_8 KCS_DRIVER_CONTROL_read_data();
UINT_32 KCS_DRIVER_CONTROL_wait_ready();
UINT_32 KCS_DRIVER_CONTROL_wait_ibf_zero();
UINT_32 KCS_DRIVER_CONTROL_wait_obf_one();
void KCS_DRIVER_CONTROL_clear_obf();
void KCS_DRIVER_CONTROL_start_write();
void KCS_DRIVER_CONTROL_end_write();
void KCS_DRIVER_CONTROL_read_next();
void KCS_DRIVER_CONTROL_get_status_abort();
UINT_32 KCS_DRIVER_decode_status(UINT_32 status, KCS_DRIVER_message_t * request, KCS_DRIVER_message_t * response );

#endif /* _KCS_H */