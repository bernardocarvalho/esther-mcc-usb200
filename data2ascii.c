/*
 *
 *  Copyright (c) 2013   Warren Jasper <wjasper@tx.ncsu.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>
#include <sys/types.h>
#include <asm/types.h>



#define MAX_COUNT     (0xffff)
#define FALSE 0
#define TRUE 1



int main (int argc, char **argv)
{

  //  float table_AIN[NCHAN_USB20X][2];

  //int ch;
  //  int i;
  //__u8 input;
  //int temp;
  //__u8 options;
  //char serial[9];

  //__u16 value;
  __u32 count= 512; /*samples per scan read, */

  double  volt;
  int ret;
  // __u16 sdataIn[512]; // holds 16 bit unsigned analog input data
  //  int aOutEnabled = 0;

  //  __u16 *dataBuff;
  char str[64];
  FILE * fd_volt, *fd_out;
  int shotNum =0;


  //  char myChar = getchar(); 
  //  struct sigaction sigIntHandler;


  if(argc > 1){
    shotNum = atoi(argv[1]);
  }
   else{
    printf("%s  [shotNum NSamples]\n", argv[0]);
    return -1;
  }
  

  // some initialization
  /* Open output File */
  sprintf(str,"data-%d.txt", shotNum);
  fd_out =  fopen(str,"w"); 	
  printf("Open %s for writing\n", str);
  sprintf(str,"voltdata-%d.bin", shotNum);
  fd_volt =  fopen(str,"rb"); 
  printf("Open %s for Reading\n", str);
  /*  fd_adc = open(str, O_RDWR );
  if (fd_adc < 0){
    printf("can open %s\n", str);
    return -1;
  }
  */
  while(fread(&volt, sizeof(double), 1, fd_volt) == 1) {
  //  for(i=0; i<10; i++) {

    //  ret=fread(&volt, sizeof(double), 1, fd_volt);// == sizeof(double)){
    
    fprintf(fd_out, "%lf\n", volt);
    //printf("%d %lf\n", ret, volt);
    //convert to big-endian.
    //write to output file.
  }
  fclose(fd_out);  
  fclose(fd_volt);
  //  free(dataBuff);
  //cleanup_USB20X(udev);
  
  return 0;
}

