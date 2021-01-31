#ifndef _CMDLINE_H
#define _CMDLINE_H

/*
 * Copyright (C) 2014 Michael Brown <mbrown@fensystems.co.uk>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**
 * @file
 *
 * Command line
 *
 */

#include <stdint.h>
#include <bcd.h>

struct nt_args
{
  int quiet;
  int pause;
  int pause_quiet;
  int text_mode;
  char uuid[17];
  char path[256];
  char initrd_path[256];

  enum bcd_type type;
  struct bcd_disk_info info;
  void *bcd_data;
  uint32_t bcd_len;
  wchar_t path16[256];
};

extern struct nt_args *nt_cmdline;

extern void process_cmdline (char *cmdline);

#endif /* _CMDLINE_H */
