/** @file addidata_network.h
* @brief network related utility functions
*
* @author S.Nahas
* @date Feb 2, 2009
*
* @version  $LastChangedRevision: 3021 $
* @version $LastChangedDate: 2008-06-11 10:16:10 +0200 (Wed, 11 Jun 2008) $
*
*
* @par LICENCE
* @verbatim
   Copyright (C) 2009  ADDI-DATA GmbH for the source code of this module.

        ADDI-DATA GmbH
        Airport boulevard B210
        D-77836 Rheinmuenster
        Tel: +49(0)7229/1847-0
        Fax: +49(0)7229/1847-200
        http://www.addi-data-com
        info@addi-data.com

   This program is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation; either version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

   You shoud also find the complete GPL in the COPYING file
   accompanying this source code.
   @endverbatim
*
*
*/


#ifndef ADDIDATA_NETWORK_H_
#define ADDIDATA_NETWORK_H_

#ifdef WIN32
	#include <winsock2.h>
#else
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
#endif

#ifdef WIN32
	/* MS Windows: load Ws2_32.dll */
	int addidata_network_init(void);
	/* MS Windows: free Ws2_32.dll */
	int addidata_network_cleanup(void);
#else
	static inline int addidata_network_init(void) { return 0; }
	static inline int addidata_network_cleanup(void) { return 0; }
#endif

/** parse string "name" containing a network address
 * - either as IP or symbolic
 * and stores the result in "addr"
 *
 * @param[in] name : string to parse
 * @param[out] addr : contains parsed IP address
 *
 * @retval 0 success
 * @retval -1 parse error
 */
int addidata_network_addrparse(const char* name, struct sockaddr_in* addr);


/** require the user to input a network address on stdin ,
 * using prompt "prompt"
 * and try to parse it
 *
 * @param[in] prompt console invite
 * @param[out] name contains parsed file (zeroed on all cases)
 *
 * @retval 0 success
 * @retval -1 parse error
 */
int addidata_network_ask(const char* prompt, char name[100]);

/** to call on network error
 * handle MS Windows separate namespace */
void addidata_network_perror(const char* msg);

int addidata_network_set_socket_timeout(int fd, const struct timeval timeout);

#endif /* ADDIDATA_NETWORK_H_ */
