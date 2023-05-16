#ifndef __MODBUS_H__
#define __MODBUS_H__

#ifdef _WIN32
#include <stdint_compat.h>
typedef int socklen_t;
#include <winsock2.h>
#define __INLINE__ __inline
#define __FUNCTION__ "Function"
#define MSG_WAITALL 0
#define MSG_NOSIGNAL 0
#define MSG_EOR 0
#else
#include <stdint.h> // for ISO C99 fixed width types ( uint32_t etc ... )
#define __INLINE__ inline
#endif

extern unsigned char bForceAllCompliant;

#ifdef _WIN32
#define __PACKED
#pragma pack(1)
#else
/*
	 	GCC specific:
		The `packed' attribute specifies that a variable or structure field
	    should have the smallest possible alignment--one byte for a
	    variable, and one bit for a field, unless you specify a larger
	    value with the `aligned' attribute.
 */
#define __PACKED __attribute__ ((packed))
#endif

#define MIN_REG_VALUE_FOR_NEW_MODBUS 1000

typedef struct
{
	uint16_t ref_u16; /* transaction identifier from client - not used by server but msut be present in responses and exceptions */
	uint16_t protocol_u16; /* protocol identifier - must be 0 - not transformed */
	uint16_t len_u16 ; /* length field for the following bytes (including unitid) */
	uint8_t unitid_u8; /* unit identifier (default to 0) */
} __PACKED modbus_generic_prefix_t;

/* a query prefix: generic prefix + command */
typedef struct
{
	modbus_generic_prefix_t generic_prefix;
	uint8_t command_u8;
} __PACKED  modbus_query_prefix_t;

/** given the size of a complete packet in bytes, returns the value of the length field
* i.e. packet_size minus ref, protocol and len
* */
__INLINE__ static uint16_t compute_len_field(unsigned long packet_size)
{
	return ((uint16_t) packet_size-6);
}

// FC3 ------------------------------------------------------------------

/* header for FC3 (read multiple register) query  */
typedef struct
{
	uint16_t reg_u16 ; /* register ( = reference number) */
	uint16_t wc_u16 ; /* word count (1-125) */
} __PACKED modbus_FC3_query_header;

/* a complete MODBUS FC3 query */
typedef struct
{
	modbus_query_prefix_t query_prefix;
	modbus_FC3_query_header header;
} __PACKED modbus_FC3_query_t;

/* header for FC3 (read multiple register) response */
typedef struct
{
	uint8_t command_u8; /* command number = 0x03 */
	uint16_t bc_u16; /* byte count (4-255) */
} __PACKED modbus_FC3_response_old_header;

/* a complete FC3 response */
typedef struct
{
	modbus_generic_prefix_t prefix;
	modbus_FC3_response_old_header header;
	uint32_t data[]; /* variable lenght field */
} __PACKED modbus_FC3_response_old_t;

/* header for FC3 (read multiple register) response */
typedef struct
{
	uint8_t command_u8; /* command number = 0x03 */
	uint8_t bc_u8; /* byte count (4-255) */
} __PACKED modbus_FC3_response_new_header;

/* a complete FC3 response */
typedef struct
{
	modbus_generic_prefix_t prefix;
	modbus_FC3_response_new_header header;
	uint32_t data[]; /* variable lenght field */
} __PACKED modbus_FC3_response_new_t;


// FC16 ------------------------------------------------------------------

/* header for FC16 (write multiple register) query  */
typedef struct
{
	uint16_t reg_u16 ; /* register */
	uint16_t wc_u16 ; /* word count (1-100) */
	uint16_t bc_u16; /* byte count (2*wc) */
} __PACKED modbus_FC16_query_old_header;

/* a complete MODBUS FC16 query */
typedef struct
{
	modbus_query_prefix_t query_prefix;
	modbus_FC16_query_old_header header;
	char data[]; /* variable lenght field */
} __PACKED  modbus_FC16_query_old_t;

/* header for FC16 (write multiple register) query  */
typedef struct
{
	uint16_t reg_u16 ; /* register */
	uint16_t wc_u16 ; /* word count (1-100) */
	uint8_t bc_u8; /* byte count (2*wc) */
} __PACKED modbus_FC16_query_new_header;

/* a complete MODBUS FC16 query */
typedef struct
{
	modbus_query_prefix_t query_prefix;
	modbus_FC16_query_new_header header;
	char data[]; /* variable lenght field */
} __PACKED  modbus_FC16_query_new_t;


/* Litel struct to get the FC16 query reg  */
typedef struct
{
	modbus_query_prefix_t query_prefix;
	uint16_t reg_u16 ; /* register */
} __PACKED modbus_FC16_query_reg_t;


/* header for FC16 (write multiple register) response */
typedef struct
{
	uint8_t command_u8; /* command number = 0x10 */
	uint16_t reg_u16; /* register */
	uint16_t wc_u16; /* word count (1-100) */
} __PACKED modbus_FC16_response_header;

/* a complete MODBUS FC16 response */
typedef struct
{
	modbus_generic_prefix_t prefix;
	modbus_FC16_response_header header;
} __PACKED modbus_FC16_response_t;


// FC23 ------------------------------------------------------------------
/* header for FC23 (read/write registers) query  */
typedef struct
{
	modbus_FC3_query_header headerFC3;
	modbus_FC16_query_old_header headerFC16;
} __PACKED modbus_FC23_query_old_header;

/* a complete MODBUS FC23 query */
typedef struct
{
	modbus_query_prefix_t query_prefix;
	modbus_FC23_query_old_header header;
	char data[]; /* variable lenght field */
} __PACKED  modbus_FC23_query_old_t;

/* header for FC23 (read/write registers) query  */
typedef struct
{
	modbus_FC3_query_header headerFC3;
	modbus_FC16_query_new_header headerFC16;
} __PACKED modbus_FC23_query_new_header;

/* a complete MODBUS FC23 query */
typedef struct
{
	modbus_query_prefix_t query_prefix;
	modbus_FC23_query_new_header header;
	char data[]; /* variable lenght field */
} __PACKED  modbus_FC23_query_new_t;

/* Litel struct to get the FC23 query reg  */
typedef struct
{
	modbus_query_prefix_t query_prefix;
	modbus_FC3_query_header headerFC3;
	uint16_t reg_FC16_u16;
} __PACKED  modbus_FC23_query_reg_t;


/* a complete MODBUS FC23 response */
typedef struct
{
	modbus_generic_prefix_t prefix;
	modbus_FC3_response_old_header header;
	uint32_t data[]; /* variable lenght field */
} __PACKED modbus_FC23_response_old_t;


/* a complete MODBUS FC23 response */
typedef struct
{
	modbus_generic_prefix_t prefix;
	modbus_FC3_response_new_header header;
	uint32_t data[]; /* variable lenght field */
} __PACKED modbus_FC23_response_new_t;

// Exceptions ------------------------------------------------------------------

/* exception code
 * They are defined in the MODBUS/TCP standard except for  MODBUS_REMOTE_EXECUTION_ERROR
 * */
typedef
enum
{
	MODBUS_OK = 0x0, /* never actuall sent */
	MODBUS_ILLEGAL_FUNCTION = 0x1, /* function code is not allowable action for the slave */
	MODBUS_ILLEGAL_DATA_ADDRESS = 0x2, /* data address received in query is not allowable */
	MODBUS_ILLEGAL_DATA_VALUE = 0x3, /* incorrect value int the query data field or the length is incorrect */
	MODBUS_ILLEGAL_DATA_RESPONSE_LENGTH = 0x4, /* the request as framed would generate a response whose size exceeds the available MODBUS datasize. */
	MODBUS_ACKNOWLEDGE = 0x5, /* specialized use in conjunction with programming commands */
	MODBUS_DSLAVE_DEVICE_BUSY = 0x6, /* specialized use in conjunction with programming commands */
	MODBUS_NEGATIVE_ACKNOWLEDGE = 0x07, /* specialized use in conjunction with programming commands */
	MODBUS_MEMORY_PARITY_ERROR = 0x08, /* the extended file area failed to pass a consistency check */
	MODBUS_REMOTE_EXECUTION_ERROR = 0x09, /* the remote function performed incorrectly */
	MODBUS_GATEWAY_PATH_UNAVAILABLE = 0x0A, /* used with modbus plus gateway */
	MODBUS_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND = 0x0B /* used with modbus plus gateway */
} modbus_exception_code_t;

/* header for exception */
typedef struct
{
	uint8_t command_u8; /* command number + 0x80 */
	uint8_t reason_u8; /* exception code */
} __PACKED modbus_exception_header;

/* a complete exception */
typedef struct
{
	modbus_generic_prefix_t prefix;
	modbus_exception_header header;
} __PACKED modbus_exception_t;

// Query(Commands) ID ---------------------------------------------------------------------

enum
{
	MODBUS_FC3 = 0x3, /* Read Multiple register */
	MODBUS_FC16 = 0x10, /* Write Multiple register */
	MODBUS_FC23 = 0x17 /* Read/Write registers */
};

#ifdef _WIN32
#pragma pack()
#endif

#endif // __MODBUS_H__
