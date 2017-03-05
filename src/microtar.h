/**
 * Copyright (c) 2017 c0mpute
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `microtar.c` for details.
 */

#ifndef MICROTAR_H
#define MICROTAR_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>

#define MTAR_VERSION "0.1.0"
#define TMAGIC   "ustar"
#define TVERSION "00"

enum {
  MTAR_ESUCCESS     =  0,
  MTAR_EFAILURE     = -1,
  MTAR_EOPENFAIL    = -2,
  MTAR_EREADFAIL    = -3,
  MTAR_EWRITEFAIL   = -4,
  MTAR_ESEEKFAIL    = -5,
  MTAR_EBADCHKSUM   = -6,
  MTAR_ENULLRECORD  = -7,
  MTAR_ENOTFOUND    = -8
};

enum {
  MTAR_TREG   = '0',
  MTAR_TLNK   = '1',
  MTAR_TSYM   = '2',
  MTAR_TCHR   = '3',
  MTAR_TBLK   = '4',
  MTAR_TDIR   = '5',
  MTAR_TFIFO  = '6'
};

typedef struct {
  unsigned mode;
  unsigned uid;
  unsigned gid;
  unsigned size;
  unsigned mtime;
  unsigned type;
  unsigned devminor;
  unsigned devmajor;
  char name[100];
  char linkname[100];
} mtar_header_t;


typedef struct mtar_t mtar_t;

struct mtar_t {
  int (*read)(mtar_t *tar, void *data, unsigned size);
  int (*write)(mtar_t *tar, const void *data, unsigned size);
  int (*seek)(mtar_t *tar, unsigned pos);
  int (*close)(mtar_t *tar);
  void *stream;
  unsigned pos;
  unsigned remaining_data;
  unsigned last_header;
};


const char* mtar_strerror(int err);

int mtar_open(mtar_t *tar, const char *filename, const char *mode);
int mtar_fdopen(mtar_t *tar, int fd, const char *mode);
int mtar_close(mtar_t *tar);

int mtar_seek(mtar_t *tar, unsigned pos);
int mtar_rewind(mtar_t *tar);
int mtar_next(mtar_t *tar);
int mtar_find(mtar_t *tar, const char *name, mtar_header_t *h);
int mtar_read_header(mtar_t *tar, mtar_header_t *h);
int mtar_read_data(mtar_t *tar, void *ptr, unsigned size);

int mtar_write_header(mtar_t *tar, const mtar_header_t *h);
int mtar_write_file_header(mtar_t *tar, const char *name, unsigned size, const struct stat *st);
int mtar_write_dir_header(mtar_t *tar, const char *name);
int mtar_write_data(mtar_t *tar, const void *data, unsigned size);
int mtar_write_file(mtar_t *tar, char *fname);
int mtar_write_files(mtar_t *tar, char *pathname);

int mtar_create(mtar_t *tar, char *dstfile, char *pathname, char *permissions);
int mtar_create_fd(mtar_t *tar, int fd, char *pathname, char *permissions);

int mtar_finalize(mtar_t *tar);


#endif
