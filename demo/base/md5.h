/* MD5lib.h - md5 library
 */

/* Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All
rights reserved.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

/* The following makes MD default to MD5 if it has not already been
  defined with C compiler flags.
 */

#ifndef _DEMO_BASE_MD5LIB_H_
#define _DEMO_BASE_MD5LIB_H_

#include <stdio.h>
#include <string.h>
#include <time.h>

namespace demo {

namespace base {

/* GLOBAL.H - RSAREF types and constants
 */

/* PROTOTYPES should be set to one if and only if the compiler supports
  function argument prototyping.
  The following makes PROTOTYPES default to 0 if it has not already
  been defined with C compiler flags.
 */
#if 1
#ifndef PROTOTYPES
#define PROTOTYPES 1
#endif
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
  returns an empty list.
 */
#if 1
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif
#endif

char *MDString PROTO_LIST((char *, int));
char *MDFile PROTO_LIST((char *));
// char* MDString (char *string, int len);
// char* MDFile(char *filename);
char *hmac_md5(char *text, char *key);

typedef struct {
  UINT4 state[4];           /* state (ABCD) */
  UINT4 count[2];           /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64]; /* input buffer */
} MD5_CTX;

void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputLen);

void MD5Final(unsigned char digest[16], MD5_CTX *context);
void MD5Transform(UINT4[4], unsigned char[64]);
void Encode(unsigned char *, UINT4 *, unsigned int);
void Decode(UINT4 *, unsigned char *, unsigned int);
void MD5_memcpy(POINTER, POINTER, unsigned int);
void MD5_memset(POINTER, int, unsigned int);

}  // namespace base

}  // namespace demo

#endif  // _DEMO_BASE_MD5LIB_H_
