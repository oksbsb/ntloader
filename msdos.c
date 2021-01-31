/*
 *  ntloader  --  Microsoft Windows NT6+ loader
 *  Copyright (C) 2021  A1ive.
 *
 *  ntloader is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ntloader is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ntloader.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <msdos.h>
#include <fsuuid.h>
#include <cmdline.h>
#include <ntboot.h>

int
check_msdos_partmap (void *disk,
                     int (*disk_read) (void *disk, uint64_t sector,
                                       size_t len, void *buf))
{
  int i;
  struct msdos_part_mbr mbr;
  uint64_t start_addr;
  uint32_t signature;

  if (!disk_read (disk, 0, sizeof (mbr), &mbr))
    return 0;
  if (mbr.signature != MSDOS_PART_SIGNATURE)
  {
    DBG ("MSDOS partition signature not found.\n");
    return 0;
  }

  for (i = 0; i < MSDOS_MAX_PARTITIONS; i++)
  {
    if (mbr.entries[i].type == MSDOS_PART_TYPE_GPT_DISK)
    {
      DBG ("found dummy mbr.\n");
      return 0;
    }
    if (! mbr.entries[i].length || msdos_part_is_empty (mbr.entries[i].type))
      continue;
    if (msdos_part_is_extended (mbr.entries[i].type))
    {
      /* TODO: add support for logical partition */
      continue;
    }
    DBG ("part %d ", i);
    if (check_fsuuid (disk, mbr.entries[i].start, disk_read))
    {
      start_addr = ((uint64_t) mbr.entries[i].start) << 9;
      signature = *(uint32_t *)mbr.unique_signature;
      DBG ("MBR Start=0x%llx Signature=%04X\n", start_addr, signature);
      memcpy (nt_cmdline->info.partid, &start_addr, sizeof (start_addr));
      nt_cmdline->info.partmap = 0x01;
      memcpy (nt_cmdline->info.diskid, &signature, sizeof (signature));
      return 1;
    }
  }
  return 0;
}
