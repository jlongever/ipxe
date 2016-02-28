#ifndef __IPMI_H__
#define __IPMI_H__

#include <ipmi_msgdefs.h>
#include <compiler.h>

#define __user

/**   @struct   raw_ipmi_request_t
 *    @brief    Get configuration for Net
 */
typedef struct __attribute__ ((__packed__))
{
    uint8_t netfn_lun;
    uint8_t command;
    uint8_t *data;
} raw_ipmi_request_t ;

/**   @struct   raw_ipmi_response_t
 *    @brief    Get configuration for Net
 */
typedef struct __attribute__ ((__packed__))
{
    uint8_t netfn_lun;
    uint8_t command;
    uint8_t data[IPMI_MAX_MSG_LENGTH];
} raw_ipmi_response_t ;

/**   @struct   get_config_t
 *    @brief    Get configuration for Net
 */
typedef struct __attribute__ ((__packed__))
{
    uint8_t netfn_lun;
    uint8_t command;
    uint8_t channel_num   : 4;                /* The channel number (usually 1 for LAN) */
    uint8_t               : 3;                /* Reserved bits */
    uint8_t get_revision  : 1;                /* 0 = Get paramter, 1 = get paramter revision */
    uint8_t parameter;                        /* Paramter selector */
    uint8_t set_selector;                     /* Set selector. 00 if paramter does not use selector */
    uint8_t block_selector;                   /* Block selector. 00 if paramter does not use selector */
} get_config_request_t ;

/**  @struct   set_config_response_t
 *   @brief    Set configuration response
 */
typedef struct __attribute__ ((__packed__))
{
    uint8_t netfn_lun;
    uint8_t command;
    uint8_t ccode;
    uint8_t revision;
    uint8_t buffer[33];
} get_config_response_t;

/**   @struct   ip_address_parameter_t
 *    @brief    The IP address parameter
 */
typedef struct __attribute__ ((__packed__))
{
    uint32_t address;
} ip_address_parameter_t;

/**   @struct   set_config_t
 *    @brief    Set configuration for Net
 */
typedef struct __attribute__ ((__packed__))
{
    uint8_t netfn_lun;
    uint8_t command;
    uint8_t channel_num  : 4;
    uint8_t              : 4;
    uint8_t parameter;
} set_config_request_t;

/**  @struct   set_config_response_t
 *   @brief    Set configuration response
 */
typedef struct __attribute__ ((__packed__))
{
    uint8_t netfn_lun;
    uint8_t command;
    uint8_t ccode;
    uint8_t revision;
} set_config_response_t;

/**   @struct   mac_address_parameter_t
 *    @brief    The MAC address parameter
 */
typedef struct __attribute__ ((__packed__))
{
    uint8_t mac_address[6];
} mac_address_parameter_t;


void ipmi_finish( int );
void ipmi_init( void );
int ipmi_ready( void );
uint32_t get_lan_settings( get_config_request_t *req, get_config_response_t *rsp );
uint32_t get_ip_address( void );
uint32_t get_mac_address( uint8_t *mac );
uint32_t raw_ipmi_command( raw_ipmi_request_t *req, raw_ipmi_response_t *rsp, uint8_t *data, uint16_t *length );

#define IPMI_READY  \
    ipmi_ready()

#define IPMI_MAX_ADDR_SIZE 32
struct ipmi_addr {
	int   addr_type;
	short channel;
	char  data[IPMI_MAX_ADDR_SIZE];
};

#define IPMI_SYSTEM_INTERFACE_ADDR_TYPE	0x0c
struct ipmi_system_interface_addr {
	int           addr_type;
	short         channel;
	unsigned char lun;
};

#define IPMI_IPMB_ADDR_TYPE		0x01
#define IPMI_IPMB_BROADCAST_ADDR_TYPE	0x41
struct ipmi_ipmb_addr {
	int           addr_type;
	short         channel;
	unsigned char slave_addr;
	unsigned char lun;
};

#define IPMI_LAN_ADDR_TYPE		0x04
struct ipmi_lan_addr {
	int           addr_type;
	short         channel;
	unsigned char privilege;
	unsigned char session_handle;
	unsigned char remote_SWID;
	unsigned char local_SWID;
	unsigned char lun;
};

#define IPMI_BMC_CHANNEL  0xf
#define IPMI_NUM_CHANNELS 0x10

struct ipmi_msg {
	unsigned char  netfn;
	unsigned char  cmd;
	unsigned short data_len;
	unsigned char  __user *data;
};

struct kernel_ipmi_msg {
	unsigned char  netfn;
	unsigned char  cmd;
	unsigned short data_len;
	unsigned char  *data;
};


#define IPMI_INVALID_CMD_COMPLETION_CODE	0xC1
#define IPMI_TIMEOUT_COMPLETION_CODE		0xC3
#define IPMI_UNKNOWN_ERR_COMPLETION_CODE	0xff

#endif /* __IPMI_H__ */
