/*
 * px.h
 *		Header file for pgcrypto.
 *
 * Copyright (c) 2001 Marko Kreen
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.	IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: px.h,v 1.1.1.2 2003/04/17 22:33:53 sailesh Exp $
 */

#ifndef __PX_H
#define __PX_H

#include <sys/types.h>
#include <sys/param.h>
#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif

#ifndef BYTE_ORDER
#error BYTE_ORDER must be defined as LITTLE_ENDIAN or BIG_ENDIAN
#endif


#if 1

#define px_alloc(s) palloc(s)
#define px_realloc(p, s) prealloc(p, s)
#define px_free(p)	pfree(p)

#else

void	   *xalloc(size_t s);
void	   *xrealloc(void *p, size_t s);
void		xfree(void *p);

#define px_alloc(s) xalloc(s)
#define px_realloc(p, s) xrealloc(p, s)
#define px_free(p)	xfree(p)
#endif

/* max len of 'type' parms */
#define PX_MAX_NAMELEN		128

/* max salt returned */
#define PX_MAX_SALT_LEN		128


typedef struct px_digest PX_MD;
typedef struct px_alias PX_Alias;
typedef struct px_hmac PX_HMAC;
typedef struct px_cipher PX_Cipher;
typedef struct px_combo PX_Combo;

struct px_digest
{
	unsigned	(*result_size) (PX_MD * h);
	unsigned	(*block_size) (PX_MD * h);
	void		(*reset) (PX_MD * h);
	void		(*update) (PX_MD * h, const uint8 *data, unsigned dlen);
	void		(*finish) (PX_MD * h, uint8 *dst);
	void		(*free) (PX_MD * h);
	/* private */
	union
	{
		unsigned	code;
		const void *ptr;
	}			p;
};

struct px_alias
{
	char	   *alias;
	char	   *name;
};

struct px_hmac
{
	unsigned	(*result_size) (PX_HMAC * h);
	unsigned	(*block_size) (PX_HMAC * h);
	void		(*reset) (PX_HMAC * h);
	void		(*update) (PX_HMAC * h, const uint8 *data, unsigned dlen);
	void		(*finish) (PX_HMAC * h, uint8 *dst);
	void		(*free) (PX_HMAC * h);
	void		(*init) (PX_HMAC * h, const uint8 *key, unsigned klen);

	PX_MD	   *md;
	/* private */
	struct
	{
		uint8	   *ipad;
		uint8	   *opad;
	}			p;
};

struct px_cipher
{
	unsigned	(*block_size) (PX_Cipher * c);
	unsigned	(*key_size) (PX_Cipher * c);	/* max key len */
	unsigned	(*iv_size) (PX_Cipher * c);

	int			(*init) (PX_Cipher * c, const uint8 *key, unsigned klen, const uint8 *iv);
	int			(*encrypt) (PX_Cipher * c, const uint8 *data, unsigned dlen, uint8 *res);
	int			(*decrypt) (PX_Cipher * c, const uint8 *data, unsigned dlen, uint8 *res);
	void		(*free) (PX_Cipher * c);
	/* private */
	void	   *ptr;
	int			pstat;			/* mcrypt uses it */
};

struct px_combo
{
	int			(*init) (PX_Combo * cx, const uint8 *key, unsigned klen,
									 const uint8 *iv, unsigned ivlen);
	int			(*encrypt) (PX_Combo * cx, const uint8 *data, unsigned dlen,
										uint8 *res, unsigned *rlen);
	int			(*decrypt) (PX_Combo * cx, const uint8 *data, unsigned dlen,
										uint8 *res, unsigned *rlen);
	unsigned	(*encrypt_len) (PX_Combo * cx, unsigned dlen);
	unsigned	(*decrypt_len) (PX_Combo * cx, unsigned dlen);
	void		(*free) (PX_Combo * cx);

	PX_Cipher  *cipher;
	unsigned	padding;
};

int			px_find_digest(const char *name, PX_MD ** res);
int			px_find_hmac(const char *name, PX_HMAC ** res);
int			px_find_cipher(const char *name, PX_Cipher ** res);
int			px_find_combo(const char *name, PX_Combo ** res);

int			px_get_random_bytes(uint8 *dst, unsigned count);

const char *px_resolve_alias(const PX_Alias * aliases, const char *name);

#define px_md_result_size(md)		(md)->result_size(md)
#define px_md_block_size(md)		(md)->block_size(md)
#define px_md_reset(md)			(md)->reset(md)
#define px_md_update(md, data, dlen)	(md)->update(md, data, dlen)
#define px_md_finish(md, buf)		(md)->finish(md, buf)
#define px_md_free(md)			(md)->free(md)

#define px_hmac_result_size(hmac)	(hmac)->result_size(hmac)
#define px_hmac_block_size(hmac)	(hmac)->block_size(hmac)
#define px_hmac_reset(hmac)		(hmac)->reset(hmac)
#define px_hmac_init(hmac, key, klen)	(hmac)->init(hmac, key, klen)
#define px_hmac_update(hmac, data, dlen) (hmac)->update(hmac, data, dlen)
#define px_hmac_finish(hmac, buf)	(hmac)->finish(hmac, buf)
#define px_hmac_free(hmac)		(hmac)->free(hmac)


#define px_cipher_key_size(c)		(c)->key_size(c)
#define px_cipher_block_size(c)		(c)->block_size(c)
#define px_cipher_iv_size(c)		(c)->iv_size(c)
#define px_cipher_init(c, k, klen, iv)	(c)->init(c, k, klen, iv)
#define px_cipher_encrypt(c, data, dlen, res) \
					(c)->encrypt(c, data, dlen, res)
#define px_cipher_decrypt(c, data, dlen, res) \
					(c)->decrypt(c, data, dlen, res)
#define px_cipher_free(c)		(c)->free(c)


#define px_combo_encrypt_len(c, dlen)	(c)->encrypt_len(c, dlen)
#define px_combo_decrypt_len(c, dlen)	(c)->decrypt_len(c, dlen)
#define px_combo_init(c, key, klen, iv, ivlen) \
					(c)->init(c, key, klen, iv, ivlen)
#define px_combo_encrypt(c, data, dlen, res, rlen) \
					(c)->encrypt(c, data, dlen, res, rlen)
#define px_combo_decrypt(c, data, dlen, res, rlen) \
					(c)->decrypt(c, data, dlen, res, rlen)
#define px_combo_free(c)		(c)->free(c)

#endif   /* __PX_H */
