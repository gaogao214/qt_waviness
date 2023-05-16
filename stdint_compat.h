/** @file stdint_compat.h
* @brief compatibility with ISO C 99 integer datatypes for VC++ compiler 32 bits
*
* @author S.Nahas
* @date Feb 3, 2009
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

#ifndef STDINT_COMPAT_H_
#define STDINT_COMPAT_H_

#ifndef WIN32
#error this header has been validated for VC++
#endif
typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;

#endif /* STDINT_COMPAT_H_ */
