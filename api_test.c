#include <stdio.h>
#include <stdlib.h>

#include "ftf_cmd.h"
#include "ftf_vty.h"

// Include your interface header file


// Define your API function here
/*
 * Three part: function declaration; define structure; function definition
 * For part one - function declaration:
 *			= Advise make function name as test function name plus t
 *			  e.g. To test function: parse_macaddr(which is a function in your interface), then
 *				   define its name as 'parse_macaddr_t'
 * For part two - define structure:
 *			= Advise make structure name as test function name plus n
 *			  e.g. So parse_macaddr has struct name 'parse_macaddr_n'
 *			  Then define three member: cmdstr, funcname, helpstr
 *				- cmdstr: Advise test function name, so let it be "parse_macaddr"
 *				- funcname: function name in part one, so be parse_macaddr_t, intention, this is not string
 *				- helpstr: words to explain your function, including usage, purpose and parameters
 * For part three - function definition
 *			= Define your function, notice argc and argv starts from parameters, so not including
 *			"parse_macaddr" itself
 *
 * Below is an example, you can copy, paste, modify, and compile
 */
DEFUN(test_cmd_t,
		test_cmd_n,
		"test",
		"Usage: test MAC\n"
		"Parse mac address from format FF:FF:FF:FF:FF:FF to 255 255 255 255 255 255\n"
		"	Parameters: MAC - mac addresss of regular format\n")
{
	if(argc != 1) {
		printf("Usage: test MAC\n");
		return -1;
	}

	printf("Try to transfer: %s\n", argv[0]);
	return 0;
}


DEFUN(port_mac_write_t,
		port_mac_write_n,
		"port mac write",
		"\nUsage: port_mac_write port\n"
		"Save port info such as port, mac, vlanID into file /var/s_mac, be careful,\n"
		"the port should be static\n"
		"	Parameters: PORT - port number to write into file\n")
{
	int port;

	if(argc != 1) {
		printf("Usage: port_mac_write PORT\n");
		return -1;
	}

	port = atoi(argv[0]);
	printf("Save info of port %d into /var/s_mac\n", port);
	return 0;
}

int main()
{
	install_element(&test_cmd_n);
	install_element(&port_mac_write_n);

	ftf_run();

	return 0;
}
