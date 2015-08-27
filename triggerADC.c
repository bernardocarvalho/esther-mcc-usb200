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

#include "pmd.h"
#include "usb-20X.h"

#define MAX_COUNT     (0xffff)
#define FALSE 0
#define TRUE 1


usb_dev_handle *udev = NULL;

/*
  http://stackoverflow.com/questions/1641182/how-can-i-catch-a-ctrl-c-event-c
  SIGNAL HANDLER for Ctrl-C, mainly
*/
void sig_handler(int s){

  printf("\n, Caught signal %d. Closing ADC board\n",s);
  //  close_atca();
  //free( dmaBuff);
  usbAInScanStop_USB20X(udev);
  usbAInScanClearFIFO_USB20X(udev);
  usleep(500000);
  cleanup_USB20X(udev);
  exit(1); 
}

int main (int argc, char **argv)
{

  float table_AIN[NCHAN_USB20X][2];

  //int ch;
  int i,k;
  //__u8 input;
  //int temp;
  //__u8 options;
  //char serial[9];
  __u8 channel;
  //__u16 value;
  __u32 count= 512; /*samples per scan read, */
  __u32 initScans=2;
  __u32 scans=500; /*~ 512/frequency per scan*/ 
  double frequency, volt;
  int ret;
  // __u16 sdataIn[512]; // holds 16 bit unsigned analog input data
  //  int aOutEnabled = 0;

  __u16 *dataBuff;
  char str[64];
  FILE * fd_data, * fd_volt;
  int shotNum =0, triggerLevel=2000;

  //  char myChar = getchar(); 
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = sig_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
 

  if(argc > 2){
    triggerLevel = atoi(argv[2]);
  }

  if(argc > 1){
    shotNum = atoi(argv[1]);
  }
  /*  else{
    printf("%s  [shotNum triggerLevel]\n", argv[0]);
    return -1;
  }
  */

  udev = NULL;
  if ((udev = usb_device_find_USB_MCC(USB201_PID))) {
    printf("Success, found a USB 201!\n");
  }
 else {
    printf("Failure, did not find a USB 20X!\n");
    return 0;
  }

  // some initialization
  /* Open output File */
  sprintf(str,"rawdata-%d.bin", shotNum);
  fd_data =  fopen(str,"wb"); 	
  sprintf(str,"voltdata-%d.bin", shotNum);
  fd_volt =  fopen(str,"wb"); 
  /*  fd_adc = open(str, O_RDWR );
  if (fd_adc < 0){
    printf("can open %s\n", str);
    return -1;
  }
  */
  //print out the wMaxPacketSize.  Should be 512
  printf("MaxPacketSize = %d\n", usb_get_max_packet_size(udev,0));

  usbBuildGainTable_USB20X(udev, table_AIN);
  /*
  for (i = 0; i < NCHAN_USB20X; i++) {
    printf("Calibration Table: %d   Slope = %f   Offset = %f\n", i, table_AIN[i][0], table_AIN[i][1]);
  }
  */
  
  printf(" USB-20X Analog Input Scan.\n");
  usbAInScanStop_USB20X(udev);
  dataBuff=malloc(count * scans * sizeof(__u16 ));
  //  printf("Enter number of scans (less than 512): ");
  //scanf("%d", &count);
  //	printf("Input channel 0-7: ");
  //scanf("%hhd", &channel);
  channel=0;
  //	printf("Enter sampling frequency [Hz]: ");
  //scanf("%lf", &frequency);
  frequency=50000;
  usbAInScanStop_USB20X(udev);
  usbAInScanClearFIFO_USB20X(udev);
  // sleep(1);
  usleep(500000);
  printf("Start Acq,\n");
  usbAInScanStart_USB20X(udev, 0, frequency, (0x1<<channel), 0, 0, 0);
  do   {
    for (i = 0; i < initScans; i++)      
      ret = usbAInScanRead_USB20X(udev, count, 1, &dataBuff[count*i]); 
    //printf(" = %#x, %d  \n",  dataBuff[count*initScans-1], dataBuff[count*initScans-1]);  
  }
  while (dataBuff[count*initScans-1] <= triggerLevel );
  //while (dataBuff[count*initScans-1] <= triggerLevel && (myChar = getchar()) != EOF && myChar != '\n');
  printf("Trigger Acq\n");

  for (i = initScans; i < scans; i++) {
    ret = usbAInScanRead_USB20X(udev, count, 1, &dataBuff[count*i]);
    //  printf("Number samples read = %d\n", ret/2);
  }
  //	ret = usbAInScanRead_USB20X(udev, count, 1, sdataIn);
  usbAInScanStop_USB20X(udev);
  printf("Stop Acq \n");

  for (i = 0; i < scans; i++){ 
    fwrite(&dataBuff[count*i], sizeof(__u16 ), count, fd_data);
    for (k = 0; k < count; k++){
      volt=volts_USB20X(udev,dataBuff[count*i +k]);
      fwrite(&volt, sizeof(double), 1, fd_volt);
    }
  }
  //printf("Number samples read = %d\n", ret/2);
  for (i = count-10; i < count; i++) {
    dataBuff[i] = rint(dataBuff[i]*table_AIN[channel][0] + table_AIN[channel][1]);
    printf("Sample[%d] = %#x, %d  Volts = %lf\n",
	   i, dataBuff[i], dataBuff[i], volts_USB20X(udev,dataBuff[i]));
  }
  fclose(fd_data);  fclose(fd_volt);
  free(dataBuff);
  cleanup_USB20X(udev);
  
  return 0;
}

