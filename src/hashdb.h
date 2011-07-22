/* Copyright (C) 2011 Aigui Liu
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, visit the http://fsf.org website.
 */

#ifndef _HASHDB_H
#define _HASHDB_H

#include <stdint.h>
#include "bloom.h"
#include "hashtable.h"

#define HASHDB_KEY_MAX_SZ	128
#define HASHDB_VALUE_MAX_SZ	256
#define HASHDB_DEFAULT_TNUM	1000000
#define HASHDB_DEFAULT_BNUM	100000
#define HASHDB_DEFAULT_CNUM	100000

typedef struct hash_entry {
	char *key;		/* key of <key, value> */
	void *value;		/* value of <key, value> */
	uint32_t ksize;		/* size of the key */
	uint32_t vsize;		/* size of the value */
	uint32_t tsize;		/* total size of the entry */
	uint32_t hash;		/* second hash value */
	uint64_t off;		/* offset of the entry */
	uint64_t left;		/* offset of the left child */
	uint64_t right;		/* offset of the right child */
}HASH_ENTRY;
#define HASH_ENTRY_SZ sizeof(HASH_ENTRY)

typedef struct hashdb
{
	char *dbname;		/* hashdb filename */
	int fd;			/* hashdb fd */
	uint64_t tnum;		/* number of total items */
	uint32_t bnum;		/* number of hash buckets */
	uint16_t cnum;		/* number of cache items */
	BLOOM *bloom;		/* bloom filter */
	HASH_ENTRY *cache;	/* hash item cache */
}HASHDB;
#define HASHDB_SZ  sizeof(HASHDB)

typedef struct hashdb_header {
	uint16_t magic;		/* magic number */
	uint16_t cnum;		/* number of cache items */
	uint32_t bnum;		/* number of hash buckets */
	uint64_t tnum;		/* number of total items */
	uint64_t boff;		/* offset of bloom filter */
	uint64_t hoff;		/* offset of hash backets */
	uint64_t voff;		/* offset of hash values */
}HASHDB_HDR;

HASHDB *hashdb_new(uint64_t tnum, uint32_t bnum, uint16_t cnum);
int hashdb_open(HASHDB *db, const char *path);
int hashdb_close(HASHDB *db);
int hashdb_set(HASHDB *db, char *key, void *value);
int hashdb_get(HASHDB *db, char *key, void *value);
int hashdb_unlink(HASHDB *db);

#endif
