#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <generics.h>
#include <kcs.h>
#include <ipxe/init.h>
#include <ipxe/io.h>
#include <fru.h>

/*
 *   IPMI FRU Write
 */
uint32_t fru_write( write_fru_request_t *req, write_fru_response_t *rsp, uint16_t num_bytes )
{
    int status = 0;

    KCS_DRIVER_message_t kcs_request;
    KCS_DRIVER_message_t kcs_response;

    req->netfn_lun = IPMI_NETFN_STORAGE_REQUEST << 2;
    req->command = FRU_WRITE;

    kcs_request.data_length = sizeof(write_fru_request_t) + num_bytes;
    kcs_request.data = (uint8_t*)&req;
    kcs_response.data = (uint8_t*)&rsp;

    status = KCS_DRIVER_execute( &kcs_request, &kcs_response );
    return status;
}

/*
 *   IPMI FRU Read
 */
uint32_t fru_read( read_fru_request_t *req, read_fru_response_t *rsp )
{
    int status = 0;

    KCS_DRIVER_message_t kcs_request;
    KCS_DRIVER_message_t kcs_response;

    req->netfn_lun = IPMI_NETFN_STORAGE_REQUEST << 2;
    req->command = FRU_READ;

    kcs_request.data_length = sizeof(read_fru_request_t);
    kcs_request.data = (uint8_t*)req;
    kcs_response.data = (uint8_t*)rsp;

    status = KCS_DRIVER_execute( &kcs_request, &kcs_response );
    return status;
}
