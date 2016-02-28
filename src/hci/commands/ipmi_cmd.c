/*
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <ipxe/command.h>
#include <ipxe/parseopt.h>
#include <ipxe/settings.h>
#include <ipxe/settings_ui.h>
#include <ipmi.h>

FILE_LICENCE ( GPL2_OR_LATER );

/** @file
 *
 * Raw IPMI commands
 *
 */

/** "system_info" options */
struct ipmi_options {};

/** "ipmi_cmd" option list */
static struct option_descriptor ipmi_opts[] = {};

/** "ipmi_cmd" command descriptor */
static struct command_descriptor ipmi_cmd =
	COMMAND_DESC ( struct ipmi_options, ipmi_opts, 0, 255, "" );


/**
 * "raw_ipmi" command
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @ret rc		Return status code
 */
static int run_raw ( int argc, char **argv )
{
    int rc, i;
    raw_ipmi_request_t req;
    raw_ipmi_response_t rsp;

    uint16_t length = 0;
    uint8_t data[IPMI_MAX_MSG_LENGTH];
    uint8_t netfn, cmd;

    memset(data, 0, IPMI_MAX_MSG_LENGTH);
    netfn = strtoul( argv[2], NULL, 0 );
    cmd = strtoul( argv[3], NULL, 0 );

    req.netfn_lun = netfn << 2;
    req.command = cmd;

    /* Any data parameters */
    printf(" Request: %02x %02x ", netfn, cmd );
    for( i = 0; i < argc - 4; i++ )
    {
        data[i] = strtoul( argv[i + 4], NULL, 0 );
        printf("%02x ", data[i] );
        length++;
    }
    printf("\n");

    /* Send Command */
    rc = raw_ipmi_command(  &req, &rsp, data, &length );
    if( rc )
    {
        printf(" Error sending raw ipmi command!\n");
        return rc;
    }

    printf(" Response Data: " );
    for( i = 0; i < length; i++ )
        printf("%02x ", rsp.data[i]);
    printf("\n");

    return rc;
}

/**
 * "ipmi_exec" command
 *
 * @v argc		Argument count
 * @v argv		Argument list
 * @ret rc		Return status code
 */
static int ipmi_exec ( int argc, char **argv )
{
	struct ipmi_options opts;
	int rc;

	/* Parse options */
	if ( ( rc = parse_options ( argc, argv, &ipmi_cmd, &opts ) ) != 0 )
    {
		return rc;
    }

    if( argc < 2 )
    {
        printf( " Usage:  ipmi <command> [<options>]\n");
        return -1;
    }

    if( 0 == strcmp( argv[1], "raw" ) )
    {
        if( argc < 4 )
        {
            printf(" Invalid argument! \n\tipmi raw <netfn> <command> [<data>]\n");
            return -1;
        }

        return run_raw( argc, argv );
    }

	return 0;
}

/** System information UI commands */
struct command ipmi_command __command = {
	.name = "ipmi",
	.exec = ipmi_exec,
};
