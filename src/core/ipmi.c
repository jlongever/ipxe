#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ipmi.h>
#include <kcs.h>
#include <ipxe/init.h>

int g_bmc_present = 0;

#define MAC_ADDR_LEN 6

/*
 *   IPMI Raw Command
 */
uint32_t raw_ipmi_command( raw_ipmi_request_t *req, raw_ipmi_response_t *rsp, uint8_t *data, uint16_t *length )
{
    uint32_t status = 0;
    uint32_t retry = MAX_REQ_RETRIES;
    do
    {
        KCS_DRIVER_message_t kcs_request;
        KCS_DRIVER_message_t kcs_response;

        req->data = &data[0];

        kcs_request.data_length = 2 + *length; //sizeof(raw_ipmi_request_t) + *length;
        kcs_request.data = (uint8_t*)req;
        kcs_response.data = (uint8_t*)rsp;

        status = KCS_DRIVER_execute( &kcs_request, &kcs_response );
        if( ! status )
        {
            *length = kcs_response.data_length - 1;
            break;
        }

        sleep(1);
    } while( retry-- );

    return status;
}

/*
 *   IPMI Get LAN settings
 */
uint32_t get_lan_settings( get_config_request_t *req, get_config_response_t *rsp )
{
    int status = 0;
    uint32_t retry = MAX_REQ_RETRIES;
    do
    {
        if( IPMI_READY )
        {
            KCS_DRIVER_message_t kcs_request;
            KCS_DRIVER_message_t kcs_response;

            req->netfn_lun = IPMI_NETFN_TRANSPORT << 2;
            req->command = IPMI_GET_CONFIG_CMD;

            kcs_request.data_length = sizeof(get_config_request_t);
            kcs_request.data = (uint8_t*)req;
            kcs_response.data = (uint8_t*)rsp;

            status = KCS_DRIVER_execute( &kcs_request, &kcs_response );
            if( ! status )
                break;

            sleep(1);
        }

    } while( retry-- );

    return status;
}

/*
 *  Get the IP Address of the IPMI controller */
uint32_t get_ip_address( void )
{
    uint32_t status = 0;
    uint32_t retry = MAX_REQ_RETRIES;
    do
    {
        if( IPMI_READY )
        {
            get_config_request_t req;
            get_config_response_t resp;

            memset(&req, 0, sizeof(get_config_request_t));
            memset(&resp, 0, sizeof(get_config_response_t));

            /* Assign the request data */
            req.channel_num    = 1;
            req.get_revision   = 0;
            req.parameter      = 3; /* IP Adrress */
            req.set_selector   = 0;
            req.block_selector = 0;

            status = get_lan_settings( &req, &resp );
            if( 0 == status )
            {
                return ((resp.buffer[0] << 24) |
                        (resp.buffer[1] << 16) |
                        (resp.buffer[2] << 8)  |
                        (resp.buffer[3] << 0)  );
            }

            sleep(1);
        }

    } while( retry-- );

    return status;
}

/*
 *  Get the MAC Address of the IPMI controller */
uint32_t get_mac_address( uint8_t *mac )
{
    uint32_t status = 0;
    uint32_t retry = MAX_REQ_RETRIES;
    do
    {
        if( IPMI_READY )
        {
            get_config_request_t req;
            get_config_response_t resp;

            memset(&req, 0, sizeof(get_config_request_t));
            memset(&resp, 0, sizeof(get_config_response_t));

            /* Assign the request data */
            req.channel_num    = 1;
            req.get_revision   = 0;
            req.parameter      = 5; /* MAC Address */
            req.set_selector   = 0;
            req.block_selector = 0;

            status = get_lan_settings( &req, &resp );
            if( 0 == status )
            {
                memcpy( mac, resp.buffer, sizeof(uint8_t)*MAC_ADDR_LEN );
                break;
            }

            sleep(1);
        }

    } while( retry-- );

    return status;
}


int ipmi_ready( void )
{
    /* Check if ready if not initialized */
    if( MAX_KCS_RETRIES > KCS_DRIVER_CONTROL_wait_ready() )
        return 1;

    /* Not ready and not initialized */
    return 0;
}

void ipmi_finish ( int flags __unused )
{
    g_bmc_present = 0;
    return;
}

void ipmi_init( void )
{
    if( ipmi_ready() )
    {
        g_bmc_present = 1;
    }
}
