/**
 * @file aes.c
 *
 * AES implementation
 */


#ifdef _WIN32
	#include <winsock2.h>
    #include "aes.h"
#else
	#include <arpa/inet.h>
	#include <aes.h>
#endif
#include <string.h>

#define rot1(x) (((x) << 24) | ((x) >> 8))
#define rot2(x) (((x) << 16) | ((x) >> 16))
#define rot3(x) (((x) <<  8) | ((x) >> 24))

/* This cute trick does 4 'mul by two' at once.
 * The key to this is that we need to xor with 0x1b if the top bit is set.
 * a 1xxx xxxx   0xxx 0xxx First we mask the 7bit,
 * b 1000 0000   0000 0000 then we shift right by 7 puting the 7bit in 0bit,
 * c 0000 0001   0000 0000 we then subtract (c) from (b)
 * d 0111 1111   0000 0000 and now we and with our mask
 * e 0001 1011   0000 0000
 */
#define mt  0x80808080
#define ml  0x7f7f7f7f
#define mh  0xfefefefe
#define mm  0x1b1b1b1b
#define mul2(x,t)	((t)=((x)&mt), \
			((((x)+(x))&mh)^(((t)-((t)>>7))&mm)))

#define inv_mix_col(x,f2,f4,f8,f9) (\
			(f2)=mul2(x,f2), \
			(f4)=mul2(f2,f4), \
			(f8)=mul2(f4,f8), \
			(f9)=(x)^(f8), \
			(f8)=((f2)^(f4)^(f8)), \
			(f2)^=(f9), \
			(f4)^=(f9), \
			(f8)^=rot3(f2), \
			(f8)^=rot2(f4), \
			(f8)^rot1(f9))

/* some macros to do endian independent byte extraction */
#define n2l(c,l) l=ntohl(*c); c++
#define l2n(l,c) *c++=htonl(l)

/*
 * AES S-box
 */
static const unsigned char aes_sbox[256] =
{
	0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,
	0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
	0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,
	0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
	0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,
	0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
	0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,
	0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
	0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,
	0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
	0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,
	0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
	0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,
	0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
	0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,
	0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
	0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,
	0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
	0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,
	0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,
	0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,
	0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
	0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,
	0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
	0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,
	0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
	0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,
	0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
	0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,
	0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
	0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,
	0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16,
};

/*
 * AES is-box
 */
static const unsigned char aes_isbox[256] =
{
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,
    0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,
    0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,
    0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,
    0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,
    0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,
    0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,
    0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,
    0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,
    0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,
    0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,
    0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,
    0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,
    0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,
    0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,
    0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,
    0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

static const unsigned char Rcon[30]=
{
	0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,
	0x1b,0x36,0x6c,0xd8,0xab,0x4d,0x9a,0x2f,
	0x5e,0xbc,0x63,0xc6,0x97,0x35,0x6a,0xd4,
	0xb3,0x7d,0xfa,0xef,0xc5,0x91,
};


/* Perform doubling in Galois Field GF(2^8) using the irreducible polynomial
   x^8+x^4+x^3+x+1 */
static unsigned char AES_xtime(unsigned long x)
{
	return (unsigned char)(x = (x&0x80) ? (x<<1)^0x1b : x<<1);
}

/* ----- static functions ----- */
/**
 * Encrypt a single block (16 bytes) of data
 */
static void AES_encrypt(const str_AES *ps_AES, unsigned long *data)
{
    /* To make this code smaller, generate the sbox entries on the fly.
     * This will have a really heavy effect upon performance.
     */
    unsigned long tmp[4];
    unsigned long tmp1, old_a0, a0, a1, a2, a3, row;
    int curr_rnd;
    int rounds = ps_AES->w_Rounds;
    const unsigned long *k = ps_AES->pdw_Key;

    /* Pre-round key addition */
    for (row = 0; row < 4; row++)
    {
        data[row] ^= *(k++);
    }

    /* Encrypt one block. */
    for (curr_rnd = 0; curr_rnd < rounds; curr_rnd++)
    {
        /* Perform ByteSub and ShiftRow operations together */
        for (row = 0; row < 4; row++)
        {
            a0 = (unsigned long)aes_sbox[(data[row%4]>>24)&0xFF];
            a1 = (unsigned long)aes_sbox[(data[(row+1)%4]>>16)&0xFF];
            a2 = (unsigned long)aes_sbox[(data[(row+2)%4]>>8)&0xFF];
            a3 = (unsigned long)aes_sbox[(data[(row+3)%4])&0xFF];

            /* Perform MixColumn iff not last round */
            if (curr_rnd < (rounds - 1))
            {
                tmp1 = a0 ^ a1 ^ a2 ^ a3;
                old_a0 = a0;

                a0 ^= tmp1 ^ AES_xtime(a0 ^ a1);
                a1 ^= tmp1 ^ AES_xtime(a1 ^ a2);
                a2 ^= tmp1 ^ AES_xtime(a2 ^ a3);
                a3 ^= tmp1 ^ AES_xtime(a3 ^ old_a0);

            }

            tmp[row] = ((a0 << 24) | (a1 << 16) | (a2 << 8) | a3);
        }

        /* KeyAddition - note that it is vital that this loop is separate from
           the MixColumn operation, which must be atomic...*/
        for (row = 0; row < 4; row++)
        {
            data[row] = tmp[row] ^ *(k++);
        }
    }
}

/**
 * Decrypt a single block (16 bytes) of data
 */
static void AES_decrypt(const str_AES *ps_AES, unsigned long *data)
{
    unsigned long tmp[4];
    unsigned long xt0,xt1,xt2,xt3,xt4,xt5,xt6;
    unsigned long a0, a1, a2, a3, row;
    int curr_rnd;
    int rounds = ps_AES->w_Rounds;
    unsigned long *k = (unsigned long*)ps_AES->pdw_Key + ((rounds+1)*4);

    /* pre-round key addition */
    for (row=4; row > 0;row--)
    {
        data[row-1] ^= *(--k);
    }

    /* Decrypt one block */
    for (curr_rnd=0; curr_rnd < rounds; curr_rnd++)
    {
        /* Perform ByteSub and ShiftRow operations together */
        for (row = 4; row > 0; row--)
        {
            a0 = aes_isbox[(data[(row+3)%4]>>24)&0xFF];
            a1 = aes_isbox[(data[(row+2)%4]>>16)&0xFF];
            a2 = aes_isbox[(data[(row+1)%4]>>8)&0xFF];
            a3 = aes_isbox[(data[row%4])&0xFF];

            /* Perform MixColumn iff not last round */
            if (curr_rnd<(rounds-1))
            {
                /* The MDS cofefficients (0x09, 0x0B, 0x0D, 0x0E)
                   are quite large compared to encryption; this
                   operation slows decryption down noticeably. */
                xt0 = AES_xtime(a0^a1);
                xt1 = AES_xtime(a1^a2);
                xt2 = AES_xtime(a2^a3);
                xt3 = AES_xtime(a3^a0);
                xt4 = AES_xtime(xt0^xt1);
                xt5 = AES_xtime(xt1^xt2);
                xt6 = AES_xtime(xt4^xt5);

                xt0 ^= a1^a2^a3^xt4^xt6;
                xt1 ^= a0^a2^a3^xt5^xt6;
                xt2 ^= a0^a1^a3^xt4^xt6;
                xt3 ^= a0^a1^a2^xt5^xt6;
                tmp[row-1] = ((xt0<<24)|(xt1<<16)|(xt2<<8)|xt3);
            }
            else
                tmp[row-1] = ((a0<<24)|(a1<<16)|(a2<<8)|a3);
        }

        for (row = 4; row > 0; row--)
        {
            data[row-1] = tmp[row-1] ^ *(--k);
        }
    }
}


/**
 * Set up AES with the key/iv and cipher size.
 */
void AES_SetKey(str_AES *ps_AES, const unsigned char *pb_Key,const unsigned char *pb_IV)
{
    int i, ii;
    unsigned long *W, tmp, tmp2;
    const unsigned char *ip;

    ps_AES->w_Rounds = 14;
    ps_AES->w_KeySize = 8;
    W = ps_AES->pdw_Key;

    for (i=0; i<ps_AES->w_KeySize; i+=2)
    {
        W[i+0]=	((unsigned long)pb_Key[ 0]<<24)|
                ((unsigned long)pb_Key[ 1]<<16)|
                ((unsigned long)pb_Key[ 2]<< 8)|
                ((unsigned long)pb_Key[ 3]    );
        W[i+1]=	((unsigned long)pb_Key[ 4]<<24)|
                ((unsigned long)pb_Key[ 5]<<16)|
                ((unsigned long)pb_Key[ 6]<< 8)|
                ((unsigned long)pb_Key[ 7]    );
        pb_Key += 8;
    }

    ip = Rcon;
    ii = 4 * (ps_AES->w_Rounds+1);
    for (i = ps_AES->w_KeySize; i<ii; i++)
    {
        tmp = W[i-1];
        if ((i % ps_AES->w_KeySize) == 0)
        {
            tmp2 =(unsigned long)aes_sbox[(tmp    )&0xff]<< 8;
            tmp2|=(unsigned long)aes_sbox[(tmp>> 8)&0xff]<<16;
            tmp2|=(unsigned long)aes_sbox[(tmp>>16)&0xff]<<24;
            tmp2|=(unsigned long)aes_sbox[(tmp>>24)     ];
            tmp=tmp2^(((unsigned int)*ip)<<24);
            ip++;
        }
        if ((ps_AES->w_KeySize == 8) && ((i % ps_AES->w_KeySize) == 4))
        {
            tmp2 =(unsigned long)aes_sbox[(tmp    )&0xff]    ;
            tmp2|=(unsigned long)aes_sbox[(tmp>> 8)&0xff]<< 8;
            tmp2|=(unsigned long)aes_sbox[(tmp>>16)&0xff]<<16;
            tmp2|=(unsigned long)aes_sbox[(tmp>>24)     ]<<24;
            tmp=tmp2;
        }

        W[i]=W[i-ps_AES->w_KeySize]^tmp;
    }

    /* copy the iv across */
    memcpy(ps_AES->pb_IV, pb_IV, 16);
}

/**
 * Change a key for decryption.
 */
void AES_ConvertKey(str_AES *ps_AES)
{
    int i;
    unsigned long *k,w,t1,t2,t3,t4;

    k = ps_AES->pdw_Key;
    k += 4;
    for (i=ps_AES->w_Rounds*4; i>4; i--)
    {
        w= *k;
        w = inv_mix_col(w,t1,t2,t3,t4);
        *k++ =w;
    }
}


/**
 * Encrypt a byte sequence (with a block size 16) using the AES cipher.
 */
void AES_CBC_Encrypt(str_AES *ps_AES, const unsigned char *pb_Array, unsigned char *pb_EncryptedArray, int i_Length)
{
    unsigned long tin0, tin1, tin2, tin3;
    unsigned long tout0, tout1, tout2, tout3;
    unsigned long tin[4];
    unsigned long *pdw_IV = (unsigned long *)ps_AES->pb_IV;
    unsigned long *pdw_Array = (unsigned long *)pb_Array;
    unsigned long *pdw_EncryptedArray = (unsigned long *)pb_EncryptedArray;

    n2l(pdw_IV, tout0);
    n2l(pdw_IV, tout1);
    n2l(pdw_IV, tout2);
    n2l(pdw_IV, tout3);
    pdw_IV -= 4;

    for (i_Length -= 16; i_Length >= 0; i_Length -= 16)
    {
        n2l(pdw_Array, tin0);
        n2l(pdw_Array, tin1);
        n2l(pdw_Array, tin2);
        n2l(pdw_Array, tin3);
        tin[0] = tin0^tout0;
        tin[1] = tin1^tout1;
        tin[2] = tin2^tout2;
        tin[3] = tin3^tout3;

        AES_encrypt(ps_AES, tin);

        tout0 = tin[0];
        l2n(tout0, pdw_EncryptedArray);
        tout1 = tin[1];
        l2n(tout1, pdw_EncryptedArray);
        tout2 = tin[2];
        l2n(tout2, pdw_EncryptedArray);
        tout3 = tin[3];
        l2n(tout3, pdw_EncryptedArray);
    }

    l2n(tout0, pdw_IV);
    l2n(tout1, pdw_IV);
    l2n(tout2, pdw_IV);
    l2n(tout3, pdw_IV);
}

/**
 * Decrypt a byte sequence (with a block size 16) using the AES cipher.
 */
void AES_CBC_Decrypt(str_AES *ps_AES, const unsigned char *pb_Array, unsigned char *pb_DecryptedArray, int i_Length)
{
    unsigned long tin0, tin1, tin2, tin3;
    unsigned long xor0,xor1,xor2,xor3;
    unsigned long tout0,tout1,tout2,tout3;
    unsigned long data[4];
    unsigned long *pdw_IV = (unsigned long *)ps_AES->pb_IV;
    unsigned long *pdw_Array = (unsigned long *)pb_Array;
    unsigned long *pdw_DecryptedArray = (unsigned long *)pb_DecryptedArray;

    n2l(pdw_IV ,xor0);
    n2l(pdw_IV, xor1);
    n2l(pdw_IV, xor2);
    n2l(pdw_IV, xor3);
    pdw_IV -= 4;

    for (i_Length-=16; i_Length >= 0; i_Length -= 16)
    {
        n2l(pdw_Array, tin0);
        n2l(pdw_Array, tin1);
        n2l(pdw_Array, tin2);
        n2l(pdw_Array, tin3);

        data[0] = tin0;
        data[1] = tin1;
        data[2] = tin2;
        data[3] = tin3;

        AES_decrypt(ps_AES, data);

        tout0 = data[0]^xor0;
        tout1 = data[1]^xor1;
        tout2 = data[2]^xor2;
        tout3 = data[3]^xor3;

        xor0 = tin0;
        xor1 = tin1;
        xor2 = tin2;
        xor3 = tin3;

        l2n(tout0, pdw_DecryptedArray);
        l2n(tout1, pdw_DecryptedArray);
        l2n(tout2, pdw_DecryptedArray);
        l2n(tout3, pdw_DecryptedArray);
    }

    l2n(xor0, pdw_IV);
    l2n(xor1, pdw_IV);
    l2n(xor2, pdw_IV);
    l2n(xor3, pdw_IV);
}


