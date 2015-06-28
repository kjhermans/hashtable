/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifndef _HD_FS_H_
#define _HD_FS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define HD_MAX_PATH 512

typedef struct hd_fs {
  hd_t hd;
  char cwd[HD_MAX_PATH];
} hd_fs_t;

typedef struct hd_file {
  hd_fs_t* fs;
  char path[HD_MAX_PATH];
} hd_file_t;



#ifdef __cplusplus
}
#endif

#endif
