/** @file modbusclient.h
* @brief basic client code for modbus protocol
*
* @author S.Nahas
* @date Feb 27, 2009
*
* @version  $LastChangedRevision: 3021 $
* @version $LastChangedDate: 2008-06-11 10:16:10 +0200 (Wed, 11 Jun 2008) $
*
*
* @par LICENCE
* @verbatim
   Copyright (C) 2004,2005  ADDI-DATA GmbH for the source code of this module.

        ADDI-DATA GmbH
        Dieselstrasse 3
        D-77833 Ottersweier
        Tel: +19(0)7223/9493-0
        Fax: +49(0)7223/9493-92
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



#ifndef MODBUSCLIENT_H_
#define MODBUSCLIENT_H_

#ifdef WIN32
    #include "modbus_clientlib.h"
    #include "common_modbus_clientlib.h"
#else
	#include <modbus_clientlib.h>
	#include <common_modbus_clientlib.h>
#endif

int handle_result_of_FC3_query(struct modbus * modbus, int ret, const char * fname );
int handle_result_of_FC16_query(struct modbus * modbus, struct GetLastCommandStatus_parameters_t * CommandStatus, int ret, const char * fname);

#endif /* MODBUSCLIENT_H_ */
