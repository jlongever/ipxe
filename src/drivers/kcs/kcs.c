#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <io.h>
#include <kcs.h>

#undef KCS_DEBUG_PRINT

UINT_32 KCS_DRIVER_write( KCS_DRIVER_message_t * request )
{
    KCS_DRIVER_CONTROL_wait_ibf_zero();

    KCS_DRIVER_CONTROL_clear_obf();

    KCS_DRIVER_CONTROL_start_write();

    KCS_DRIVER_CONTROL_wait_ibf_zero();

    if( !KCS_DRIVER_STATUS_is_write_state() )
    {
#if (KCS_DEBUG_PRINT)
        printf("KCS_DRIVER_ERROR_NOT_WRITE_STATE__WRITE_START\n");
#endif
        return KCS_DRIVER_ERROR_NOT_WRITE_STATE__WRITE_START;
    }

    KCS_DRIVER_CONTROL_clear_obf();

    UINT_8 count = 0;
    while( count < request->data_length - 1 )
    {
        KCS_DRIVER_CONTROL_write_data( request->data[count] );

        KCS_DRIVER_CONTROL_wait_ibf_zero();

        if( !KCS_DRIVER_STATUS_is_write_state() )
        {
#if (KCS_DEBUG_PRINT)
            printf("KCS_DRIVER_ERROR_NOT_WRITE_STATE__WRITE_DURING\n");
#endif
            return KCS_DRIVER_ERROR_NOT_WRITE_STATE__WRITE_DURING;
        }

        KCS_DRIVER_CONTROL_clear_obf();

        count++;
    }

    // Last Byte
    KCS_DRIVER_CONTROL_end_write();

    KCS_DRIVER_CONTROL_wait_ibf_zero();

    if( !KCS_DRIVER_STATUS_is_write_state() )
    {
#if (KCS_DEBUG_PRINT)
        printf("KCS_DRIVER_ERROR_NOT_WRITE_STATE__WRITE_END\n");
#endif
        return KCS_DRIVER_ERROR_NOT_WRITE_STATE__WRITE_END;
    }

    KCS_DRIVER_CONTROL_clear_obf();

    KCS_DRIVER_CONTROL_write_data( request->data[count] );

    return KCS_DRIVER_ERROR_NO_ERROR;
}


UINT_32 KCS_DRIVER_read( KCS_DRIVER_message_t * response )
{
    UINT_8 count = 0;

    while(1)
    {
        KCS_DRIVER_CONTROL_wait_ibf_zero();

        if( !KCS_DRIVER_STATUS_is_read_state() )
        {
            if( !KCS_DRIVER_STATUS_is_idle_state() )
            {
#if (KCS_DEBUG_PRINT)
                printf("KCS_DRIVER_ERROR_NOT_IDLE_STATE__READ_DURING\n");
#endif
                return KCS_DRIVER_ERROR_NOT_IDLE_STATE__READ_DURING;
            }

            KCS_DRIVER_CONTROL_wait_obf_one();

            KCS_DRIVER_CONTROL_read_data();

            return KCS_DRIVER_ERROR_NO_ERROR; //SUCCESS
        }

        KCS_DRIVER_CONTROL_wait_obf_one();

        response->data[count] = KCS_DRIVER_CONTROL_read_data(); // Any bounds checking?
        count++;

        response->data_length = count;

        KCS_DRIVER_CONTROL_read_next();
    }
}


UINT_32 KCS_DRIVER_error()
{
    UINT_32 retries = 0;
    UINT_32 status = KCS_DRIVER_ERROR_NO_ERROR;

    while( retries < MAX_KCS_RETRIES )
    {
        KCS_DRIVER_CONTROL_wait_ibf_zero();

        KCS_DRIVER_CONTROL_get_status_abort();

        KCS_DRIVER_CONTROL_wait_ibf_zero();

        KCS_DRIVER_CONTROL_clear_obf();

        KCS_DRIVER_CONTROL_write_data(0x00);

        KCS_DRIVER_CONTROL_wait_ibf_zero();

        if( !KCS_DRIVER_STATUS_is_read_state() )
        {
            retries++;
            continue;
        }

        KCS_DRIVER_CONTROL_wait_obf_one();

        status = KCS_DRIVER_CONTROL_read_data();

        KCS_DRIVER_CONTROL_read_next();

        KCS_DRIVER_CONTROL_wait_ibf_zero();

        if( !KCS_DRIVER_STATUS_is_idle_state() )
        {
            retries++;
            continue;
        }

        KCS_DRIVER_CONTROL_wait_ibf_zero();

        KCS_DRIVER_CONTROL_clear_obf();

        return status;
    }

#if (KCS_DEBUG_PRINT)
    printf("KCS_DRIVER_ERROR_COMMS_FAILURE\n");
#endif

    return KCS_DRIVER_ERROR_COMMS_FAILURE;
}

UINT_32 KCS_DRIVER_execute( KCS_DRIVER_message_t * request, KCS_DRIVER_message_t * response )
{
    UINT_32 status;

    if( KCS_DRIVER_ERROR_NO_ERROR != (status = KCS_DRIVER_write(request)))
    {
        return ( status | KCS_DRIVER_error() );
    }

    if( KCS_DRIVER_ERROR_NO_ERROR != (status = KCS_DRIVER_read(response)))
    {
        return ( status | KCS_DRIVER_error() );
    }

    return KCS_DRIVER_ERROR_NO_ERROR;
}


UINT_32 KCS_DRIVER_STATUS_is_write_state()
{
    UINT_8 data = 0;
    KCS_DRIVER_STATUS_status_register_t * status = 0;

    // Read Status
    data = KCS_DRIVER_CONTROL__read(KCS_DRIVER_REGISTERS_STATUS_ADDR);
    status = (KCS_DRIVER_STATUS_status_register_t *)&data;

    if( status->state_bits == KCS_DRIVER_STATUS_WRITE_STATE )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

UINT_32 KCS_DRIVER_STATUS_is_read_state()
{
    UINT_8 data = 0;
    KCS_DRIVER_STATUS_status_register_t * status = 0;

    // Read Status
    data = KCS_DRIVER_CONTROL__read(KCS_DRIVER_REGISTERS_STATUS_ADDR);
    status = (KCS_DRIVER_STATUS_status_register_t *)&data;

    if( status->state_bits == KCS_DRIVER_STATUS_READ_STATE )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

UINT_32 KCS_DRIVER_STATUS_is_error_state()
{
    UINT_8 data = 0;
    KCS_DRIVER_STATUS_status_register_t * status = 0;

    // Read Status
    data = KCS_DRIVER_CONTROL__read(KCS_DRIVER_REGISTERS_STATUS_ADDR);
    status = (KCS_DRIVER_STATUS_status_register_t *)&data;

    if( status->state_bits == KCS_DRIVER_STATUS_ERROR_STATE )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

UINT_32 KCS_DRIVER_STATUS_is_idle_state()
{
    UINT_8 data = 0;
    KCS_DRIVER_STATUS_status_register_t * status = 0;

    // Read Status
    data = KCS_DRIVER_CONTROL__read(KCS_DRIVER_REGISTERS_STATUS_ADDR);
    status = (KCS_DRIVER_STATUS_status_register_t *)&data;

    if( status->state_bits == KCS_DRIVER_STATUS_IDLE_STATE )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

UINT_8 KCS_DRIVER_CONTROL__read( UINT_16 reg )
{
    return READ_PORT_8( reg );
}

void KCS_DRIVER_CONTROL__write( UINT_16 reg, UINT_8 data )
{
    WRITE_PORT_8(reg, data);
}

void KCS_DRIVER_CONTROL_write_data( UINT_8 data )
{
    KCS_DRIVER_CONTROL__write(KCS_DRIVER_REGISTERS_DATA_IN_ADDR, data);
}

UINT_8 KCS_DRIVER_CONTROL_read_data()
{
    return KCS_DRIVER_CONTROL__read(KCS_DRIVER_REGISTERS_DATA_OUT_ADDR);
}

UINT_32 KCS_DRIVER_CONTROL_wait_ready()
{
    UINT_8 data = 0;
    KCS_DRIVER_STATUS_status_register_t * status = 0;
    UINT_32 retries = 0;

    // While we
    while( retries < MAX_KCS_RETRIES )
    {

        // Read Status
        data = KCS_DRIVER_CONTROL__read(KCS_DRIVER_REGISTERS_STATUS_ADDR);
        status = (KCS_DRIVER_STATUS_status_register_t *)&data;

        // If IDLE. Break
        // Else increment retry counter
        if( status->state_bits == KCS_DRIVER_STATUS_IDLE_STATE )
        {
            break;
        }
        else
        {
            retries++;
        }
    }

    // Debug. Return number of retries
    return retries;
}

UINT_32 KCS_DRIVER_CONTROL_wait_ibf_zero()
{
    UINT_8 data = 0;
    KCS_DRIVER_STATUS_status_register_t * status = 0;
    UINT_32 retries = 0;

    // While we
    while( retries < MAX_KCS_RETRIES )
    {

        // Read Status
        data = KCS_DRIVER_CONTROL__read(KCS_DRIVER_REGISTERS_STATUS_ADDR);
        status = (KCS_DRIVER_STATUS_status_register_t *)&data;

        // If IBF cleared. Break
        // Else increment retry counter
        if( status->ibf == 0 )
        {
            break;
        }
        else
        {
            retries++;
        }
    }

    // Debug. Return number of retries
    return retries;
}

UINT_32 KCS_DRIVER_CONTROL_wait_obf_one()
{
    UINT_8 data = 0;
    KCS_DRIVER_STATUS_status_register_t * status = 0;
    UINT_32 retries = 0;

    // While we
    while( retries < MAX_KCS_RETRIES )
    {

        // Read Status
        data = KCS_DRIVER_CONTROL__read(KCS_DRIVER_REGISTERS_STATUS_ADDR);
        status = (KCS_DRIVER_STATUS_status_register_t *)&data;

        // If OBF set. Break
        // Else increment retry counter
        if( status->obf == 1 )
        {
            break;
        }
        else
        {
            retries++;
        }
    }

    // Debug. Return number of retries
    return retries;
}

void KCS_DRIVER_CONTROL_clear_obf()
{

    UINT_8 data = 0;
    KCS_DRIVER_STATUS_status_register_t * status = 0;

    // Read Status
    data = KCS_DRIVER_CONTROL__read(KCS_DRIVER_REGISTERS_STATUS_ADDR);
    status = (KCS_DRIVER_STATUS_status_register_t *)&data;

    if( status->obf == 1 )
    {
        // Just read data register
        KCS_DRIVER_CONTROL_read_data();
    }
}

void KCS_DRIVER_CONTROL_start_write()
{
    KCS_DRIVER_CONTROL__write(KCS_DRIVER_REGISTERS_COMMAND_ADDR, KCS_DRIVER_CONTROL_WRITE_START);
}

void KCS_DRIVER_CONTROL_end_write()
{
    KCS_DRIVER_CONTROL__write(KCS_DRIVER_REGISTERS_COMMAND_ADDR, KCS_DRIVER_CONTROL_WRITE_END);
}

void KCS_DRIVER_CONTROL_read_next()
{
    KCS_DRIVER_CONTROL__write(KCS_DRIVER_REGISTERS_DATA_IN_ADDR, KCS_DRIVER_CONTROL_READ);
}

void KCS_DRIVER_CONTROL_get_status_abort()
{
    KCS_DRIVER_CONTROL__write(KCS_DRIVER_REGISTERS_COMMAND_ADDR, KCS_DRIVER_CONTROL_GET_STATUS_ABORT);
}



UINT_32 KCS_DRIVER_decode_status(UINT_32 status, KCS_DRIVER_message_t * request, KCS_DRIVER_message_t * response )
{
    UINT_16 i;

    // Print Status
    printf("KCS  Status: %X\n", status);

    // Print Request
    printf("Req: ");
    for(i=0; i < request->data_length; i++ )
    {
        printf("%X ",request->data[i] );
    }


    // Print Response
    printf("Rsp: ");
    for(i=0; i < response->data_length; i++ )
    {
        printf("%X ",response->data[i] );
    }

    return (status |= (response->data[3] << 24));

}
