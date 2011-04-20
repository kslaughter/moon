/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * miconv.cpp: 
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2011 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 */

#include <config.h>

#include <glib.h>
#include <string.h>
#include <errno.h>

#include "miconv.h"

namespace Moonlight {

enum Endian {
	LittleEndian,
	BigEndian
};

typedef int (* Decoder) (char **inbytes, size_t *inbytesleft, gunichar *outchar);
typedef int (* Encoder) (gunichar c, char **outbytes, size_t *outbytesleft);

static int decode_utf32be (char **inbytes, size_t *inbytesleft, gunichar *outchar);
static int encode_utf32be (gunichar c, char **outbytes, size_t *outbytesleft);

static int decode_utf32le (char **inbytes, size_t *inbytesleft, gunichar *outchar);
static int encode_utf32le (gunichar c, char **outbytes, size_t *outbytesleft);

static int decode_utf16be (char **inbytes, size_t *inbytesleft, gunichar *outchar);
static int encode_utf16be (gunichar c, char **outbytes, size_t *outbytesleft);

static int decode_utf16le (char **inbytes, size_t *inbytesleft, gunichar *outchar);
static int encode_utf16le (gunichar c, char **outbytes, size_t *outbytesleft);

static int decode_utf32 (char **inbytes, size_t *inbytesleft, gunichar *outchar);
static int encode_utf32 (gunichar c, char **outbytes, size_t *outbytesleft);

static int decode_utf16 (char **inbytes, size_t *inbytesleft, gunichar *outchar);
static int encode_utf16 (gunichar c, char **outbytes, size_t *outbytesleft);

static int decode_utf8 (char **inbytes, size_t *inbytesleft, gunichar *outchar);
static int encode_utf8 (gunichar c, char **outbytes, size_t *outbytesleft);

static struct {
	const char *name;
	Decoder decoder;
	Encoder encoder;
} charsets[] = {
	{ "UTF-32BE", decode_utf32be, encode_utf32be },
	{ "UTF-32LE", decode_utf32le, encode_utf32le },
	{ "UTF-16BE", decode_utf16be, encode_utf16be },
	{ "UTF-16LE", decode_utf16le, encode_utf16le },
	{ "UTF-32",   decode_utf32,   encode_utf32   },
	{ "UTF-16",   decode_utf16,   encode_utf16   },
	{ "UTF-8",    decode_utf8,    encode_utf8    },
};

struct _miconv_t {
	Decoder decode;
	Encoder encode;
	gunichar c;
};


miconv_t
miconv_open (const char *to, const char *from)
{
	Decoder decoder = NULL;
	Encoder encoder = NULL;
	miconv_t cd;
	guint i;
	
	if (!to || !from)
		return (miconv_t) -1;
	
	for (i = 0; i < G_N_ELEMENTS (charsets); i++) {
		if (!strcmp (charsets[i].name, from))
			decoder = charsets[i].decoder;
		
		if (!strcmp (charsets[i].name, to))
			encoder = charsets[i].encoder;
	}
	
	if (encoder == NULL || decoder == NULL)
		return (miconv_t) -1;
	
	cd = (miconv_t) g_malloc (sizeof (*cd));
	cd->decode = decoder;
	cd->encode = encoder;
	cd->c = -1;
	
	return cd;
}

int
miconv_close (miconv_t cd)
{
	g_free (cd);
	return 0;
}

int
miconv (miconv_t cd, char **inbytes, size_t *inbytesleft,
	char **outbytes, size_t *outbytesleft)
{
	size_t inleft, outleft;
	char *inptr, *outptr;
	gunichar c;
	int rc = 0;
	
	if (outbytes == NULL || outbytesleft == NULL) {
		/* reset converter */
		cd->c = -1;
		return 0;
	}
	
	inleft = inbytesleft ? *inbytesleft : 0;
	inptr = inbytes ? *inbytes : NULL;
	outleft = *outbytesleft;
	outptr = *outbytes;
	c = cd->c;
	
	do {
		if (c == (gunichar) -1 && cd->decode (&inptr, &inleft, &c) == -1) {
			rc = -1;
			break;
		}
		
		if (cd->encode (c, &outptr, &outleft) == -1) {
			rc = -1;
			break;
		}
		
		c = -1;
	} while (inleft > 0 && outleft > 0);
	
	if (inbytesleft)
		*inbytesleft = inleft;
	
	if (inbytes)
		*inbytes = inptr;
	
	*outbytesleft = outleft;
	*outbytes = outptr;
	cd->c = c;
	
	return rc;
}


static int
decode_utf32_be_or_le (Endian endian, char **inbytes, size_t *inbytesleft, gunichar *outchar)
{
	gunichar *inptr = (gunichar *) *inbytes;
	size_t inleft = *inbytesleft;
	gunichar c;
	
	if (inleft < 4) {
		errno = EINVAL;
		return -1;
	}
	
	if (endian == BigEndian)
		c = GUINT32_FROM_BE (*inptr);
	else
		c = GUINT32_FROM_LE (*inptr);
	
	inleft -= 4;
	inptr++;
	
	if (c >= 2147483648UL) {
		errno = EILSEQ;
		return -1;
	}
	
	*inbytes = (char *) inptr;
	*inbytesleft = inleft;
	*outchar = c;
	
	return 0;
}

static int
decode_utf32be (char **inbytes, size_t *inbytesleft, gunichar *outchar)
{
	return decode_utf32_be_or_le (BigEndian, inbytes, inbytesleft, outchar);
}

static int
decode_utf32le (char **inbytes, size_t *inbytesleft, gunichar *outchar)
{
	return decode_utf32_be_or_le (LittleEndian, inbytes, inbytesleft, outchar);
}

static int
decode_utf32 (char **inbytes, size_t *inbytesleft, gunichar *outchar)
{
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
	return decode_utf32_be_or_le (LittleEndian, inbytes, inbytesleft, outchar);
#else
	return decode_utf32_be_or_le (BigEndian, inbytes, inbytesleft, outchar);
#endif
}

static int
encode_utf32_be_or_le (Endian endian, gunichar c, char **outbytes, size_t *outbytesleft)
{
	gunichar *outptr = (gunichar *) *outbytes;
	size_t outleft = *outbytesleft;
	
	if (outleft < 4) {
		errno = E2BIG;
		return -1;
	}
	
	if (endian == BigEndian)
		*outptr++ = GUINT32_TO_BE (c);
	else
		*outptr++ = GUINT32_TO_LE (c);
	
	outleft -= 4;
	
	*outbytes = (char *) outptr;
	*outbytesleft = outleft;
	
	return 0;
}

static int
encode_utf32be (gunichar c, char **outbytes, size_t *outbytesleft)
{
	return encode_utf32_be_or_le (BigEndian, c, outbytes, outbytesleft);
}

static int
encode_utf32le (gunichar c, char **outbytes, size_t *outbytesleft)
{
	return encode_utf32_be_or_le (LittleEndian, c, outbytes, outbytesleft);
}

static int
encode_utf32 (gunichar c, char **outbytes, size_t *outbytesleft)
{
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
	return encode_utf32_be_or_le (LittleEndian, c, outbytes, outbytesleft);
#else
	return encode_utf32_be_or_le (BigEndian, c, outbytes, outbytesleft);
#endif
}

static int
decode_utf16_be_or_le (Endian endian, char **inbytes, size_t *inbytesleft, gunichar *outchar)
{
	gunichar2 *inptr = (gunichar2 *) *inbytes;
	size_t inleft = *inbytesleft;
	gunichar2 c;
	gunichar u;
	
	if (inleft < 2) {
		errno = EINVAL;
		return -1;
	}
	
	if (endian == BigEndian)
		u = GUINT16_FROM_BE (*inptr);
	else
		u = GUINT16_FROM_LE (*inptr);
	
	inleft -= 2;
	inptr++;
	
	if (u >= 0xdc00 && u <= 0xdfff) {
		errno = EILSEQ;
		return -1;
	} else if (u >= 0xd800 && u <= 0xdbff) {
		if (inleft < 2) {
			errno = EINVAL;
			return -1;
		}
		
		if (endian == BigEndian)
			c = GUINT16_FROM_BE (*inptr);
		else
			c = GUINT16_FROM_LE (*inptr);
		
		inleft -= 2;
		inptr++;
		
		if (c < 0xdc00 || c > 0xdfff) {
			errno = EILSEQ;
			return -1;
		}
		
		u = ((u - 0xd800) << 10) + (c - 0xdc00) + 0x0010000UL;
	}
	
	*inbytes = (char *) inptr;
	*inbytesleft = inleft;
	*outchar = u;
	
	return 0;
}

static int
decode_utf16be (char **inbytes, size_t *inbytesleft, gunichar *outchar)
{
	return decode_utf16_be_or_le (BigEndian, inbytes, inbytesleft, outchar);
}

static int
decode_utf16le (char **inbytes, size_t *inbytesleft, gunichar *outchar)
{
	return decode_utf16_be_or_le (LittleEndian, inbytes, inbytesleft, outchar);
}

static int
decode_utf16 (char **inbytes, size_t *inbytesleft, gunichar *outchar)
{
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
	return decode_utf16_be_or_le (LittleEndian, inbytes, inbytesleft, outchar);
#else
	return decode_utf16_be_or_le (BigEndian, inbytes, inbytesleft, outchar);
#endif
}

static int
encode_utf16_be_or_le (Endian endian, gunichar c, char **outbytes, size_t *outbytesleft)
{
	gunichar2 *outptr = (gunichar2 *) *outbytes;
	size_t outleft = *outbytesleft;
	gunichar2 ch;
	gunichar c2;
	
	if (outleft < 2) {
		errno = E2BIG;
		return -1;
	}
	
	if (c <= 0xffff && (c < 0xd800 || c > 0xdfff)) {
		ch = (gunichar2) c;
		
		if (endian == BigEndian)
			*outptr++ = GUINT16_TO_BE (ch);
		else
			*outptr++ = GUINT16_TO_LE (ch);
		
		outleft -= 2;
	} else if (outleft < 4) {
		errno = E2BIG;
		return -1;
	} else {
		c2 = c - 0x10000;
		
		ch = (gunichar2) ((c2 >> 10) + 0xd800);
		if (endian == BigEndian)
			*outptr++ = GUINT16_TO_BE (ch);
		else
			*outptr++ = GUINT16_TO_LE (ch);
		
		ch = (gunichar2) ((c2 & 0x3ff) + 0xdc00);
		if (endian == BigEndian)
			*outptr++ = GUINT16_TO_BE (ch);
		else
			*outptr++ = GUINT16_TO_LE (ch);
		
		outleft -= 4;
	}
	
	*outbytes = (char *) outptr;
	*outbytesleft = outleft;
	
	return 0;
}

static int
encode_utf16be (gunichar c, char **outbytes, size_t *outbytesleft)
{
	return encode_utf16_be_or_le (BigEndian, c, outbytes, outbytesleft);
}

static int
encode_utf16le (gunichar c, char **outbytes, size_t *outbytesleft)
{
	return encode_utf16_be_or_le (LittleEndian, c, outbytes, outbytesleft);
}

static int
encode_utf16 (gunichar c, char **outbytes, size_t *outbytesleft)
{
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
	return encode_utf16_be_or_le (LittleEndian, c, outbytes, outbytesleft);
#else
	return encode_utf16_be_or_le (BigEndian, c, outbytes, outbytesleft);
#endif
}

static int
decode_utf8 (char **inbytes, size_t *inbytesleft, gunichar *outchar)
{
	size_t inleft = *inbytesleft;
	char *inptr = *inbytes;
	size_t i, len = 0;
	unsigned char c;
	gunichar u;
	
	c = *inptr++;
	
	if (c < 0x80) {
		/* simple ascii case */
		len = 1;
	} else if (c < 0xe0) {
		c &= 0x1f;
		len = 2;
	} else if (c < 0xf0) {
		c &= 0x0f;
		len = 3;
	} else if (c < 0xf8) {
		c &= 0x07;
		len = 4;
	} else if (c < 0xfc) {
		c &= 0x03;
		len = 5;
	} else if (c < 0xfe) {
		c &= 0x01;
		len = 6;
	} else {
		errno = EILSEQ;
		return -1;
	}
	
	if (len > inleft) {
		errno = EINVAL;
		return -1;
	}
	
	u = c;
	for (i = 1; i < len; i++) {
		u = (u << 6) | ((*inptr) & 0x3f);
		inptr++;
	}
	
	*inbytesleft = inleft - len;
	*inbytes = inptr;
	*outchar = u;
	
	return 0;
}

static int
encode_utf8 (gunichar c, char **outbytes, size_t *outbytesleft)
{
	size_t outleft = *outbytesleft;
	char *outptr = *outbytes;
	size_t len, i;
	int base;
	
	if (c < 128UL) {
		base = 0;
		len = 1;
	} else if (c < 2048UL) {
		base = 192;
		len = 2;
	} else if (c < 65536UL) {
		base = 224;
		len = 3;
	} else if (c < 2097152UL) {
		base = 240;
		len = 4;
	} else if (c < 67108864UL) {
		base = 248;	
		len = 5;
	} else if (c < 2147483648UL) {
		base = 252;
		len = 6;
	} else {
		errno = EINVAL;
		return -1;
	}
	
	if (outleft < len) {
		errno = E2BIG;
		return -1;
	}
	
	for (i = len - 1; i > 0; i--) {
		/* mask off 6 bits worth and add 128 */
		outptr[i] = 128 + (c & 0x3f);
		c >>= 6;
	}
	
	/* first character has a different base */
	outptr[0] = base + c;
	
	*outbytesleft = outleft - len;
	*outbytes = outptr + len;
	
	return 0;
}

};