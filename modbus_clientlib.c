/** @file modbus_clientlib.c MODBUS/UDP client generic code
 * @author: Sylvain Nahas
 *
*/

#ifdef WIN32
	#include "modbus.h"
	#include "modbus_clientlib.h"
	#include "addidata_network.h"
#else
	#include <modbus.h>
	#include <modbus_clientlib.h>
	#include <addidata_network.h>
#endif

#include "modbus.h"

// Don't remove or change this line
unsigned char bForceAllCompliant = 0;


//---------------------------------------------------------------------------------
/** generic check on MODBUS packet prefix */
static int modbus_check_generic_prefix(modbus_generic_prefix_t * pref, uint8_t idmin, uint8_t idmax)
{

	/* check protocol version */
	if (  MODBUS_NTOHS(pref->protocol_u16) != 0 )
		return -1;

	/* check length (could not be zero, since it a least includes request id */
	if ( 0 == MODBUS_NTOHS(pref->len_u16) )
		return -1;

	/* check unitid */
	if ((pref->unitid_u8 < idmin) || (pref->unitid_u8 > idmax))
		return -1;

	return 0;
}

//-------------------------------------------------------------------
/** read a modbus packet from network (it may be query, response or exception) 
 * @param modbus[in] The modbus context
 * @param buffer[out] buffer where to store packet
 * @param count[int] size in bytes of the buffer
 *
 * @return 0 when a packet was correctly received, -1 on system error, -2 when the modbus query prefix is incorrect, -3 when the len field is too big for buffer
 */
static int modbus_recv_packet(struct modbus * modbus, char* buffer, int count)
{
	socklen_t addrsz = sizeof(struct sockaddr_in);
	int ret = -1;
	modbus_query_prefix_t * query = NULL;
	char tmpbuff[10];
	int toread = 0;
	int read = 0;
	int remaining = 0;

	/* get query header - don't remove data from socket queue */
	MODBUS_DBG("waiting for %d bytes\n",sizeof(modbus_query_prefix_t));
	ret = recvfrom(modbus->fd, buffer,sizeof(modbus_query_prefix_t),MSG_WAITALL | MSG_PEEK, (struct sockaddr*) &(modbus->addr), &addrsz);
	
	if(-1==ret)
	{
#ifdef WIN32
        if (WSAGetLastError() == 10040)
        {
            /*  */
            ret = sizeof(modbus_query_prefix_t);
        }
        else
        {
        	modbus->lasterrno = errno;
	        return -1;
        }
#else
	    MODBUS_DBG("recvfrom (query header) failed\n");
	    modbus->lasterrno = errno;
	    return -1;
#endif
	}

	MODBUS_DBG("received %d bytes\n",ret);
	query = (modbus_query_prefix_t *) buffer;
	MODBUS_DBG("query->generic_prefix.len_u16=%d\n", MODBUS_NTOHS(query->generic_prefix.len_u16));

	//TODO: id as parameter
	if ( modbus_check_generic_prefix((modbus_generic_prefix_t*)buffer,0, 1) )
	{
		MODBUS_DBG("query prefix is incorrect\n");
		return -2;
	}

	/* check that packet fit in buffer */
	if ( 6 + MODBUS_NTOHS(query->generic_prefix.len_u16) > count )
	{
		MODBUS_DBG("packet len field too big for header\n");
		/* empty socket queue */
		toread = (6 + MODBUS_NTOHS(query->generic_prefix.len_u16));
		read = 0;
		while(read<toread)
		{
			int ret = recvfrom(modbus->fd, tmpbuff, (toread - read > sizeof(tmpbuff) ? sizeof(tmpbuff) : toread - read), MSG_WAITALL, NULL, NULL);
			if(-1==ret)
			{
                #ifdef WIN32
                    if (WSAGetLastError() == 10040)
                    {
                        /* this is not a true error */
                        ret = (toread - read > sizeof(tmpbuff) ? sizeof(tmpbuff) : toread - read);
                    }
                    else
                    {
		                MODBUS_DBG("error while emptying socket buffer: recvfrom\n");
		                modbus->lasterrno = errno;
		                return -1;
                    }
                #else
		            MODBUS_DBG("error while emptying socket buffer: recvfrom\n");
		            modbus->lasterrno = errno;
		            return -1;
                #endif

			}
			read += ret;
		}

		return -3;
	}

	/* compute remaining size
	 * we already received the fields "unitid" and "command" wich is comprised in len
	 */
	remaining = MODBUS_NTOHS(query->generic_prefix.len_u16) - 2;
	MODBUS_DBG("remaining bytes in packet: %d\n", remaining);

	/* get the whole packet */
	MODBUS_DBG("reading %d bytes\n", remaining+sizeof(modbus_query_prefix_t));
	ret = recvfrom(modbus->fd, buffer , sizeof(modbus_query_prefix_t)+remaining, MSG_WAITALL, (struct sockaddr*) NULL, NULL);

	if(-1==ret)
	{
#ifdef WIN32
        if (WSAGetLastError() == 10040)
        {
            /* this is not a true error */
            ret = sizeof(modbus_query_prefix_t)+remaining;
        }
		else
        {
	        MODBUS_DBG("recvfrom (query body) failed\n");
	        modbus->lasterrno = errno;
	        return -1;
        }
#else
	    MODBUS_DBG("recvfrom (query body) failed\n");
	    modbus->lasterrno = errno;
	    return -1;
#endif
	}
	MODBUS_DBG("received %d bytes\n",ret);
	/* OK, we got the whole packet */
	return 0;
}
//---------------------------------------------------------------------------------
/** convert exception code to string */
const char * modbus_exception_to_str(modbus_exception_code_t code)
{
	switch(code)
	{
		case MODBUS_OK:
			return "MODBUS_OK (should not happen)";
		case MODBUS_ILLEGAL_FUNCTION:
			return "MODBUS_ILLEGAL_FUNCTION (bad command field)";
		case MODBUS_ILLEGAL_DATA_ADDRESS:
			return "MODBUS_ILLEGAL_DATA_ADDRESS (bad reg field)";
		case MODBUS_ILLEGAL_DATA_VALUE:
			return "MODBUS_ILLEGAL_DATA_VALUE";
		case MODBUS_ILLEGAL_DATA_RESPONSE_LENGTH:
			return "MODBUS_ILLEGAL_DATA_RESPONSE_LENGTH";
		case MODBUS_ACKNOWLEDGE:
			return "MODBUS_ACKNOWLEDGE";
		case MODBUS_DSLAVE_DEVICE_BUSY:
			return "MODBUS_DSLAVE_DEVICE_BUSY";
		case MODBUS_NEGATIVE_ACKNOWLEDGE:
			return "MODBUS_NEGATIVE_ACKNOWLEDGE";
		case MODBUS_MEMORY_PARITY_ERROR:
			return "MODBUS_MEMORY_PARITY_ERROR";
		case MODBUS_REMOTE_EXECUTION_ERROR:
			return "MODBUS_REMOTE_EXECUTION_ERROR";
		case MODBUS_GATEWAY_PATH_UNAVAILABLE:
			return "MODBUS_GATEWAY_PATH_UNAVAILABLE";
		case MODBUS_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND:
			return "MODBUS_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND";
		default:
			return "UNKNOWN EXCEPTION CODE";
	}
}
//---------------------------------------------------------------------------------
/** pretty print exception content - returns exception code */
static modbus_exception_code_t modbus_on_exception(struct modbus * modbus, modbus_exception_t * exception)
{
	return (modbus->exception = exception->header.reason_u8);
}

//---------------------------------------------------------------------------------
/** generic function to parse the response of a MODBUS F3 (read multiple registers) query
 *解析MODBUS F3(读多寄存器)查询的响应的通用函数
 * @param[in] modbus The MODBUS context
 * @param[in] ref The ref field (transaction identifier from client, in theis example, always 0)
 * @param[in] reg Register to access ( a register identify the remote function )
 * @param[in] protocol Protocol version - always 0
 * @param[in] data buffer to hold the result returned by the command
 * @param[in] count size in bytes of expected data - must be exact
 *
 * @return 0 on success, -1 on system error, -2 on modbus exception, -3 if the received answer is incorrect
 *
 * @note handles exception
 */
static int modbus_get_FC3_response(struct modbus * modbus, uint16_t ref,uint16_t reg, char * data, uint16_t count )
{
	char buffer[0xFFFF];
	modbus_FC3_response_old_t * response_old = (modbus_FC3_response_old_t*) buffer;
	modbus_FC3_response_new_t * response_new = (modbus_FC3_response_new_t*) buffer;
	modbus_exception_t * exception =  NULL;

	MODBUS_DBG("%s\n",__FUNCTION__);

	switch ( modbus_recv_packet(modbus, buffer, sizeof(buffer)) )
	{
		case 0:
			/* a packet was correctly received */
			break;
		case -1:
			modbus->lasterrno = errno;
			/* system error */
			return -1;
		case -2: /* the modbus query prefix is incorrect */
		case -3: /* the len field is too big for buffer */
			return -3;
		default: /* internal error */
			MODBUS_ON_INTERNAL_ERROR();
	}

	/* OK, we got the whole packet - check if this an exception */
	exception =  (modbus_exception_t *) buffer;

	if (exception->header.command_u8 >= 0x80 )
	{
		modbus_on_exception(modbus, exception);
		return -2;
	}

	// Test if new modbus access
	if ((reg >= MIN_REG_VALUE_FOR_NEW_MODBUS) || (bForceAllCompliant != 0))
	{
		/* no exception - check is this is an answer to out previous command */
		response_new = (modbus_FC3_response_new_t*) buffer;

		if ( 		(response_new->header.command_u8 != MODBUS_FC3)
				|| 	( MODBUS_NTOHS(response_new->prefix.ref_u16) != ref)
				|| 	( MODBUS_NTOHS(response_new->prefix.protocol_u16) != 0)
				|| 	( response_new->header.bc_u8 != count )
			)
		{
			MODBUS_DBG("hum, doesn't seem to be a response to our previous command\n");
			MODBUS_DBG("response_new->prefix.len_u16=%d\n",MODBUS_NTOHS(response_new->prefix.len_u16));
			MODBUS_DBG("response_new->prefix.ref_u16=%d\n",MODBUS_NTOHS(response_new->prefix.ref_u16));
			MODBUS_DBG("response_new->prefix.protocol_u16=%d\n",MODBUS_NTOHS(response_new->prefix.protocol_u16));
			MODBUS_DBG("response_new->prefix.unitid_u8=%d\n",response_new->prefix.unitid_u8);
			MODBUS_DBG("response_new->header.command_u8=%d\n",response_new->header.command_u8);
			MODBUS_DBG("response_new->header.bc=%hhu, expected %hu\n",response_new->header.bc_u8, count);
			return -3;
		}

		/* copy received data */
		memcpy(data, response_new->data, count);
	}
	else
	{
		/* no exception - check is this is an answer to out previous command */
		response_old = (modbus_FC3_response_old_t*) buffer;

		if ( 		(response_old->header.command_u8 != MODBUS_FC3)
				|| 	( MODBUS_NTOHS(response_old->prefix.ref_u16) != ref)
				|| 	( MODBUS_NTOHS(response_old->prefix.protocol_u16) != 0)
				|| 	( MODBUS_NTOHS(response_old->header.bc_u16) != count )
			)
		{
			MODBUS_DBG("hum, doesn't seem to be a response to our previous command\n");
			MODBUS_DBG("response_old->prefix.len_u16=%d\n",MODBUS_NTOHS(response_old->prefix.len_u16));
			MODBUS_DBG("response_old->prefix.ref_u16=%d\n",MODBUS_NTOHS(response_old->prefix.ref_u16));
			MODBUS_DBG("response_old->prefix.protocol_u16=%d\n",MODBUS_NTOHS(response_old->prefix.protocol_u16));
			MODBUS_DBG("response_old->prefix.unitid_u8=%d\n",response_old->prefix.unitid_u8);
			MODBUS_DBG("response_old->header.command_u8=%d\n",response_old->header.command_u8);
			MODBUS_DBG("response_new->header.bc=%hu, expected %hu\n",MODBUS_NTOHS(response_old->header.bc_u16), count);
			return -3;
		}

		/* copy received data */
		memcpy(data, response_old->data, count);
	}

	return 0;
}
//---------------------------------------------------------------------------------
/** generic function to send a MODBUS F3 (read multiple registers) query
 *通用函数发送一个MODBUS F3(读多寄存器)查询
 * @param[in] modbus The MODBUS context
 * @param[in] ref The ref field (transaction identifier from client, in theis example, always 0)
 * @param[in] reg Register to access ( a register identify the remote function )
 * @param[in] data buffer to hold the result of the command 数据缓冲区保存命令的结果
 * @param[in] count size in bytes of expected data
 *
 * @return 0 on success, -1 on system error, -2 on MODBUS exception (the exception code is in modbus->exception), -3 if response from remote system is incorrect
 *
 */
int modbus_send_FC3_query(struct modbus * modbus, uint16_t ref, uint16_t reg, void * data, uint16_t count )
{
	int sz = 0;
	char *buffer = NULL;
	modbus_FC3_query_t * query = NULL;
	MODBUS_DBG("%s\n",__FUNCTION__);

	MODBUS_DBG("count=%d\n",count);

	MODBUS_DBG("sizeof(modbus_query_prefix_t)=%d\n",sizeof(modbus_query_prefix_t));
	// Test if new modbus access
	if ((reg >= MIN_REG_VALUE_FOR_NEW_MODBUS) || (bForceAllCompliant != 0))
	{
		MODBUS_DBG("sizeof(modbus_FC3_query_header)=%d\n",sizeof(modbus_FC16_query_new_header));
	}
	else
	{
		MODBUS_DBG("sizeof(modbus_FC3_query_header)=%d\n",sizeof(modbus_FC16_query_old_header));
	}

    sz = sizeof(modbus_FC3_query_t);

    buffer = (char *) malloc (sz);
    if (buffer == NULL)
    {
    	modbus->lasterrno = errno;
        return -1;
    }

	query = (modbus_FC3_query_t *) buffer;
	query->query_prefix.generic_prefix.len_u16 = MODBUS_HTONS(compute_len_field(sz));
	query->query_prefix.generic_prefix.protocol_u16 = MODBUS_HTONS(0);
	query->query_prefix.generic_prefix.ref_u16 = MODBUS_HTONS(ref);
	query->query_prefix.generic_prefix.unitid_u8 = 0;
	query->query_prefix.command_u8 = MODBUS_FC3;
	query->header.reg_u16 = MODBUS_HTONS(reg);
	query->header.wc_u16= MODBUS_HTONS( (uint16_t) (count / 2 ) );

	MODBUS_DBG("query->header.wc_u16=%hd\n",MODBUS_NTOHS(query->header.wc_u16));
	MODBUS_DBG("query->header.reg_u16=%hd\n",MODBUS_NTOHS(query->header.reg_u16));

	MODBUS_DBG("query->query_prefix.generic_prefix.len_u16=%d\n",MODBUS_NTOHS(query->query_prefix.generic_prefix.len_u16));
	
	if ( -1 == send(modbus->fd,(char*) buffer, sz, MSG_NOSIGNAL | MSG_EOR ) )
	{
		modbus->lasterrno = errno;
		free (buffer);
		return -1;
	}

	MODBUS_DBG("sent %d bytes\n",sz);

	free (buffer);
	return modbus_get_FC3_response(modbus, ref, reg, data, count);
}

//---------------------------------------------------------------------------------
/** generic function to parse the response of a MODBUS F16 (write multiple registers) query
 *
 * @param[in] modbus The MODBUS context
 * @param[in] reg Register that was accessed ( a register identify the remote function )
 * @param[in] wordcount The size in words of the body
 * @param[in] ref The ref field (transaction identifier from client, in theis example, always 0)
 * @param[in] protocol Protol version - always 0
 *
 * @return 0 on success, -1 on system error, -2 on MODBUS exception (the exception code is in modbus->exception), -3 if response is incorrect
 *
 * @note handles exception
 * */
static int modbus_get_FC16_response(struct modbus * modbus, uint16_t reg, uint16_t wordcount, uint16_t ref, uint16_t protocol)
{
	char buffer[0xFFFF];
	modbus_FC16_response_t * response = NULL;
	modbus_exception_t * exception =  NULL;

	MODBUS_DBG("%s\n",__FUNCTION__);

	switch ( modbus_recv_packet(modbus, buffer, sizeof(buffer)) )
	{
		case 0:
			/* a packet was correctly received */
			break;
		case -1:
			/* system error */
			modbus->lasterrno = errno;
			return -1;
		case -2: /* the modbus query prefix is incorrect */
		case -3: /* the len field is too big for buffer */
			return -3;
		default: /* internal error */
			MODBUS_ON_INTERNAL_ERROR();
	}

	/* OK, we got the whole packet - check if this an exception */
	exception =  (modbus_exception_t *) buffer;

	if (exception->header.command_u8 >= 0x80 )
	{
		modbus_on_exception(modbus,exception);
		/* get remote command status */
//		if (exception->header.reason_u8 == MODBUS_REMOTE_EXECUTION_ERROR)
//			send_GetLastCommandStatus_query(modbus);
//		return exception->header.reason_u8;
		return -2;
	}

	/* no exception - check is this is an answer to out previous command */
	response = (modbus_FC16_response_t*) buffer;

	if ( 		( MODBUS_NTOHS(response->header.reg_u16) != reg )
			|| 	(response->header.command_u8 != MODBUS_FC16)
			|| 	( MODBUS_NTOHS(response->prefix.ref_u16) != ref)
			|| 	(MODBUS_NTOHS(response->prefix.protocol_u16) != protocol)
			|| 	( MODBUS_NTOHS(response->header.wc_u16) != wordcount )
		)
	{
		MODBUS_DBG("hum, doesn't seem to be a response to our previous command\n");
		MODBUS_DBG("response->prefix.len_u16=%d\n",MODBUS_NTOHS(response->prefix.len_u16));
		MODBUS_DBG("response->prefix.ref_u16=%d\n",MODBUS_NTOHS(response->prefix.ref_u16));
		MODBUS_DBG("response->prefix.protocol_u16=%d\n",MODBUS_NTOHS(response->prefix.protocol_u16));
		MODBUS_DBG("response->prefix.unitid_u8=%d\n",response->prefix.unitid_u8);
		MODBUS_DBG("response->header.command_u8=%d\n",response->header.command_u8);
		MODBUS_DBG("response->header.reg_u16=%d\n",MODBUS_NTOHS(response->header.reg_u16) );
		MODBUS_DBG("response->header.wc_u16=%d\n",MODBUS_NTOHS(response->header.wc_u16));
		return -3;
	}

	return 0;
}

//---------------------------------------------------------------------------------
/** generic function to send a MODBUS F16 (write multiple registers) query
 *
 * @param[in] modus MODBUS context
 * @param[in] reg Register to access ( a register identify the remote function )
 * @param[in] data Body of the query (parameters to the remote function)
 * @param[in] count size in bytes of data
 *
 * @return 0 on success, -1 on system error, -2 on MODBUS exception (the exception code is in modbus->exception), -3 if response from remote system is incorrect
 *
 */
int modbus_send_FC16_query(struct modbus * modbus, uint16_t reg, uint16_t ref, char * data, uint16_t count)
{
	int sz = 0;
	char * buffer = NULL;
	modbus_FC16_query_old_t * query_old = NULL;
	modbus_FC16_query_new_t * query_new = NULL;

	MODBUS_DBG("%s\n",__FUNCTION__);

	// Test if new modbus access
	if ((reg >= MIN_REG_VALUE_FOR_NEW_MODBUS) || (bForceAllCompliant != 0))
	{
		sz = sizeof(modbus_query_prefix_t)+sizeof(modbus_FC16_query_new_header)+count;
	}
	else
	{
		sz = sizeof(modbus_query_prefix_t)+sizeof(modbus_FC16_query_old_header)+count;
	}

	/* allocate buffer */
	buffer = (char *)malloc (sz);

	if (buffer == NULL)
	{
		modbus->lasterrno = errno;
		return -1;
	}

	MODBUS_DBG("sizeof data is: %d\n",count);
	MODBUS_DBG("sizeof(modbus_query_prefix_t)=%d\n",sizeof(modbus_query_prefix_t));

	// Test if new modbus access
	if ((reg >= MIN_REG_VALUE_FOR_NEW_MODBUS) || (bForceAllCompliant != 0))
	{
		MODBUS_DBG("sizeof(modbus_FC16_query_new_header)=%d\n",sizeof(modbus_FC16_query_new_header));
		query_new = (modbus_FC16_query_new_t *) buffer;
		query_new->query_prefix.generic_prefix.len_u16 = MODBUS_HTONS(compute_len_field(sz));
		query_new->query_prefix.generic_prefix.protocol_u16 = MODBUS_HTONS(0);
		query_new->query_prefix.generic_prefix.ref_u16 = MODBUS_HTONS(ref);
		query_new->query_prefix.generic_prefix.unitid_u8 = 0;
		query_new->query_prefix.command_u8 = MODBUS_FC16;
		query_new->header.wc_u16= MODBUS_HTONS((uint16_t)(count/2));
		query_new->header.bc_u8 = count;
		query_new->header.reg_u16 = MODBUS_HTONS(reg);
		memcpy(query_new->data,data,count);

		MODBUS_DBG("count=%d\n",count/2);
		MODBUS_DBG("query_new->query_prefix.generic_prefix.len_u16=%d\n",MODBUS_HTONS(query_new->query_prefix.generic_prefix.len_u16));
	}
	else
	{
		MODBUS_DBG("sizeof(modbus_FC16_query_old_header)=%d\n",sizeof(modbus_FC16_query_old_header));
		query_old = (modbus_FC16_query_old_t *) buffer;
		query_old->query_prefix.generic_prefix.len_u16 = MODBUS_HTONS(compute_len_field(sz));
		query_old->query_prefix.generic_prefix.protocol_u16 = MODBUS_HTONS(0);
		query_old->query_prefix.generic_prefix.ref_u16 = MODBUS_HTONS(ref);
		query_old->query_prefix.generic_prefix.unitid_u8 = 0;
		query_old->query_prefix.command_u8 = MODBUS_FC16;
		query_old->header.wc_u16= MODBUS_HTONS((uint16_t)(count/2));
		query_old->header.bc_u16 = MODBUS_HTONS(count);
		query_old->header.reg_u16 = MODBUS_HTONS(reg);
		memcpy(query_old->data,data,count);

		MODBUS_DBG("count=%d\n",count/2);
		MODBUS_DBG("query_old->query_prefix.generic_prefix.len_u16=%d\n",MODBUS_HTONS(query_old->query_prefix.generic_prefix.len_u16));
	}

	if ( -1 == send(modbus->fd,(char*) buffer, sz, MSG_NOSIGNAL | MSG_EOR ) )
	{
		modbus->lasterrno = errno;
		MODBUS_DBG("send failed\n");
		free (buffer);
		return -1;
	}

	MODBUS_DBG("sent %d bytes\n",sz);

	free (buffer);
	return modbus_get_FC16_response(modbus, reg, (uint16_t)(count/2), 0, 0);
}
//---------------------------------------------------------------------------------
/** generic function to parse the response of a MODBUS F23 (read/write registers) query
 *
 * @param[in] modbus The MODBUS context
 * @param[in] ref The ref field (transaction identifier from client, in theis example, always 0)
 * @param[in] reg Register to access ( a register identify the remote function )
 * @param[in] protocol Protocol version - always 0
 * @param[in] data buffer to hold the result returned by the command
 * @param[in] count size in bytes of expected data - must be exact
 *
 * @return 0 on success, -1 on system error, -2 on modbus exception, -3 if the received answer is incorrect
 *
 * @note handles exception
 */
static int modbus_get_FC23_response(struct modbus * modbus, uint16_t ref,uint16_t reg, char * data, uint16_t count )
{
	char buffer[0xFFFF];
	modbus_FC23_response_new_t * response_new = (modbus_FC23_response_new_t*) buffer;
	modbus_FC23_response_old_t * response_old = (modbus_FC23_response_old_t*) buffer;
	modbus_exception_t * exception =  NULL;

	MODBUS_DBG("%s\n",__FUNCTION__);

	switch ( modbus_recv_packet(modbus, buffer, sizeof(buffer)) )
	{
		case 0:
			/* a packet was correctly received */
			break;
		case -1:
			modbus->lasterrno = errno;
			/* system error */
			return -1;
		case -2: /* the modbus query prefix is incorrect */
		case -3: /* the len field is too big for buffer */
			return -3;
		default: /* internal error */
			MODBUS_ON_INTERNAL_ERROR();
	}


	/* OK, we got the whole packet - check if this an exception */
	exception =  (modbus_exception_t *) buffer;

	if (exception->header.command_u8 >= 0x80 )
	{
		modbus_on_exception(modbus, exception);
		return -2;
	}

	// Test if new modbus access
	if ((reg >= MIN_REG_VALUE_FOR_NEW_MODBUS) || (bForceAllCompliant != 0))
	{
		/* no exception - check is this is an answer to out previous command */
		response_new = (modbus_FC23_response_new_t*) buffer;

		if ( 		(response_new->header.command_u8 != MODBUS_FC23)
				|| 	( MODBUS_NTOHS(response_new->prefix.ref_u16) != ref)
				|| 	( MODBUS_NTOHS(response_new->prefix.protocol_u16) != 0)
				|| 	( response_new->header.bc_u8 != count )
			)
		{
			MODBUS_DBG("hum, doesn't seem to be a response to our previous command\n");
			MODBUS_DBG("response->prefix.len_u16=%d\n",MODBUS_NTOHS(response_new->prefix.len_u16));
			MODBUS_DBG("response->prefix.ref_u16=%d\n",MODBUS_NTOHS(response_new->prefix.ref_u16));
			MODBUS_DBG("response->prefix.protocol_u16=%d\n",MODBUS_NTOHS(response_new->prefix.protocol_u16));
			MODBUS_DBG("response->prefix.unitid_u8=%d\n",response_new->prefix.unitid_u8);
			MODBUS_DBG("response->header.command_u8=%d\n",response_new->header.command_u8);
			return -3;
		}

		/* copy received data */
		memcpy(data, response_new->data, count);
	}
	else
	{
		/* no exception - check is this is an answer to out previous command */
		response_old = (modbus_FC23_response_old_t*) buffer;

		if ( 		(response_old->header.command_u8 != MODBUS_FC23)
				|| 	( MODBUS_NTOHS(response_old->prefix.ref_u16) != ref)
				|| 	( MODBUS_NTOHS(response_old->prefix.protocol_u16) != 0)
				|| 	( MODBUS_NTOHS(response_old->header.bc_u16) != count )
			)
		{
			MODBUS_DBG("hum, doesn't seem to be a response to our previous command\n");
			MODBUS_DBG("response->prefix.len_u16=%d\n",MODBUS_NTOHS(response_old->prefix.len_u16));
			MODBUS_DBG("response->prefix.ref_u16=%d\n",MODBUS_NTOHS(response_old->prefix.ref_u16));
			MODBUS_DBG("response->prefix.protocol_u16=%d\n",MODBUS_NTOHS(response_old->prefix.protocol_u16));
			MODBUS_DBG("response->prefix.unitid_u8=%d\n",response_old->prefix.unitid_u8);
			MODBUS_DBG("response->header.command_u8=%d\n",response_old->header.command_u8);
			return -3;
		}

		/* copy received data */
		memcpy(data, response_old->data, count);
	}


	return 0;
}
//---------------------------------------------------------------------------------
/** generic function to send a MODBUS F23 (read/write registers) query
 *
 * @param[in] modus MODBUS context
 * @param[in] regwrite Register to access ( a register identify the remote function )
 * @param[in] datawrite Body of the query (parameters to the remote function)
 * @param[in] countwrite size in bytes of data
 *
 * @return 0 on success, -1 on system error, -2 on MODBUS exception (the exception code is in modbus->exception), -3 if response from remote system is incorrect
 *
 */
int modbus_send_FC23_query(struct modbus * modbus, uint16_t ref, uint16_t regwrite,  char * datawrite, uint16_t countwrite, uint16_t regread, void * dataread, uint16_t countread)
{
	int sz = 0;
	char * buffer = NULL;
	modbus_FC23_query_old_t * query_old = NULL;
	modbus_FC23_query_new_t * query_new = NULL;

	MODBUS_DBG("%s\n",__FUNCTION__);

	// Test if new modbus access
	if ((regwrite >= MIN_REG_VALUE_FOR_NEW_MODBUS) || (bForceAllCompliant != 0))
	{
		sz = sizeof(modbus_query_prefix_t)+sizeof(modbus_FC23_query_new_header)+countwrite;
	}
	else
	{
		sz = sizeof(modbus_query_prefix_t)+sizeof(modbus_FC23_query_old_header)+countwrite;
	}

	/* allocate buffer */
	buffer = (char *)malloc (sz);

	if (buffer == NULL)
	{
		modbus->lasterrno = errno;
		return -1;
	}

	// Test if new modbus access
	if ((regwrite >= MIN_REG_VALUE_FOR_NEW_MODBUS) || (bForceAllCompliant != 0))
	{
		MODBUS_DBG("sizeof data is: %d\n",countwrite);
		MODBUS_DBG("sizeof(modbus_query_prefix_t)=%d\n",sizeof(modbus_query_prefix_t));
		MODBUS_DBG("sizeof(modbus_FC23_query_header)=%d\n",sizeof(modbus_FC23_query_new_header));
		query_new = (modbus_FC23_query_new_t *) buffer;
		query_new->query_prefix.generic_prefix.len_u16 = MODBUS_HTONS(compute_len_field(sz));
		query_new->query_prefix.generic_prefix.protocol_u16 = MODBUS_HTONS(0);
		query_new->query_prefix.generic_prefix.ref_u16 = MODBUS_HTONS(ref);
		query_new->query_prefix.generic_prefix.unitid_u8 = 0;
		query_new->query_prefix.command_u8 = MODBUS_FC23;
		query_new->header.headerFC16.wc_u16= MODBUS_HTONS((uint16_t)(countwrite/2));
		query_new->header.headerFC16.bc_u8 = countwrite;
		query_new->header.headerFC16.reg_u16 = MODBUS_HTONS(regwrite);
		memcpy(query_new->data,datawrite,countwrite);
		query_new->header.headerFC3.reg_u16 = MODBUS_HTONS(regread);
		query_new->header.headerFC3.wc_u16= MODBUS_HTONS( (uint16_t) (countread / 2 ) );
		MODBUS_DBG("countwrite=%d\n",countwrite/2);
		MODBUS_DBG("query->query_prefix.generic_prefix.len_u16=%d\n",MODBUS_HTONS(query_new->query_prefix.generic_prefix.len_u16));
	}
	else
	{
		MODBUS_DBG("sizeof data is: %d\n",countwrite);
		MODBUS_DBG("sizeof(modbus_query_prefix_t)=%d\n",sizeof(modbus_query_prefix_t));
		MODBUS_DBG("sizeof(modbus_FC23_query_header)=%d\n",sizeof(modbus_FC23_query_old_header));
		query_old = (modbus_FC23_query_old_t *) buffer;
		query_old->query_prefix.generic_prefix.len_u16 = MODBUS_HTONS(compute_len_field(sz));
		query_old->query_prefix.generic_prefix.protocol_u16 = MODBUS_HTONS(0);
		query_old->query_prefix.generic_prefix.ref_u16 = MODBUS_HTONS(ref);
		query_old->query_prefix.generic_prefix.unitid_u8 = 0;
		query_old->query_prefix.command_u8 = MODBUS_FC23;
		query_old->header.headerFC16.wc_u16= MODBUS_HTONS((uint16_t)(countwrite/2));
		query_old->header.headerFC16.bc_u16 = MODBUS_HTONS(countwrite);
		query_old->header.headerFC16.reg_u16 = MODBUS_HTONS(regwrite);
		memcpy(query_old->data,datawrite,countwrite);
		query_old->header.headerFC3.reg_u16 = MODBUS_HTONS(regread);
		query_old->header.headerFC3.wc_u16= MODBUS_HTONS( (uint16_t) (countread / 2 ) );
		MODBUS_DBG("countwrite=%d\n",countwrite/2);
		MODBUS_DBG("query->query_prefix.generic_prefix.len_u16=%d\n",MODBUS_HTONS(query_old->query_prefix.generic_prefix.len_u16));
	}

	if ( -1 == send(modbus->fd,(char*) buffer, sz, MSG_NOSIGNAL | MSG_EOR ) )
	{
		modbus->lasterrno = errno;
		MODBUS_DBG("send failed\n");
		free (buffer);
		return -1;
	}

	MODBUS_DBG("sent %d bytes\n",sz);

	free (buffer);
	return modbus_get_FC23_response(modbus,ref, regread,dataread, countread/2);

}

//---------------------------------------------------------------------------------
/* return 0 on success, -1 on system error, -2 if a parameter is incorrect, -3 if address could not be resolved */
int modbus_connect(struct modbus * modbus, char * endpoint, int type, uint16_t port )
{
	int found = 0; /* could the address be resolved ? */
	struct hostent* hostinfo;
#ifdef WIN32
	WSADATA wsaData;
#endif

	/* check parameters */
	switch(type)
	{
		case SOCK_DGRAM:
			break;
		case SOCK_STREAM:
			break;
		default:
			MODBUS_DBG("incorrect value for socket type\n");
			return -2;
	}

	//if (0==port) port = MODBUS_PORT;

#ifdef WIN32
    /* Initiates use of Ws2_32.dll by a process. */
    if (WSAStartup (0x202,&wsaData) == SOCKET_ERROR)
    {
        //sprintf(outtext, "WSAStartup failed with error %d", WSAGetLastError());
        modbus->lasterrno = errno;
        WSACleanup();
        return -1;;
	}
#endif

	if ( -1 == (modbus->fd = socket(AF_INET, type, 0)) )
	{
		modbus->lasterrno = errno;
		MODBUS_DBG("call to socket() failed\n");
		return -1;
  	}

	(void) memset( &(modbus->addr), 0, sizeof(struct sockaddr_in));

	modbus->addr.sin_family = AF_INET;

	/* try as doted network address */
    #ifdef WIN32
	found = (INADDR_NONE != (modbus->addr.sin_addr.S_un.S_addr = inet_addr(endpoint)) );
    #else
	found = inet_aton(endpoint, &(modbus->addr.sin_addr));
    #endif

	if (!found)
    {
    	/* try symbolic host name */
		found = ((hostinfo = gethostbyname(endpoint)) != NULL);
		if (found)
		{
	  		modbus->addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;
			goto fin;
		}
	}

	if (!found)
	{
		MODBUS_DBG("non-valid IP address or host/network name\n");
		return -3;
	}

	fin:
		/* network addresses are always in network order */
		modbus->addr.sin_port = htons(port);
		if ( connect(modbus->fd,(struct sockaddr*) &(modbus->addr), sizeof(modbus->addr)) < 0)
		{
			return -1;
		}
		printf("connect successful\n");
		{
			struct timeval timeout = MODBUS_NETWORK_TIMEOUT;
			if( addidata_network_set_socket_timeout(modbus->fd, timeout) )
			{
				return -1;
			}
		}
	MODBUS_DBG("configured to connect to port %d\n",ntohs(modbus->addr.sin_port));
	return 0;
}
//---------------------------------------------------------------------------------
int modbus_shutdown(struct modbus * modbus)
{
#ifdef WIN32
	return closesocket(modbus->fd);
#else
	return close(modbus->fd);
#endif
}
//---------------------------------------------------------------------------------
/** initialize a modbus structure.
 *
 * @param[in] modus MODBUS context
 */
void modbus_init(struct modbus * modbus)
{
	memset(modbus,0,sizeof(struct modbus));
}
