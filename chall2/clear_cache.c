/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* **********************************************************************/
/* This program is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License as       */
/* published by the Free Software Foundation; either version 2 of the   */
/* License, or (at your option) any later version.                      */
/*                                                                      */
/* This program is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    */
/* General Public License for more details.                             */
/*                                                                      */
/* Written and (c) by EURECOM, Aurelien Francillon                      */
/* Contact <aurelien.francillon@eurecom.fr> for comment, bug reports    */
/* and possible alternative licensing of this program                   */
/************************************************************************/


/**
 * @file   clear_cache.c
 * @author Aurel
 * @date   Mon Nov 14 13:17:18 2011
 *
 * @brief simple program to clear cache, equivalent to the simple script :
 *   sync; echo 3 > /proc/sys/vm/drop_caches 
 * but in C, can be made suid.
 *
 */
#include <stdio.h>
#include <unistd.h>


int main(int argc, char **argv ){
  FILE* fp;
  int i=0;
  sync(); // sync fs (man 2 sync)
  
  fp = fopen("/proc/sys/vm/drop_caches", "w");
  if (fp== NULL){
    printf("failed to open /proc/sys/vm/drop_caches\n");
    return 2;
  }

  // sometimes the first call does not actually clears the cache, 
  // insist a bit! 
  while (i++<10){
    if(fputs("3\n",fp)==EOF){
      printf("write failed \n");
      return 2;
    }
  }
  fclose(fp);

  return 0;  
}
