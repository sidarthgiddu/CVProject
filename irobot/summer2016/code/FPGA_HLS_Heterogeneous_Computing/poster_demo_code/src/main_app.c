/******************************************************************************
 *
 * (c) Copyright 2012 Xilinx, Inc. All rights reserved.
 *
 * This file contains confidential and proprietary information of Xilinx, Inc.
 * and is protected under U.S. and international copyright and other
 * intellectual property laws.
 *
 * DISCLAIMER:
 * This disclaimer is not a license and does not grant any rights to the
 * materials distributed herewith. Except as otherwise provided in a valid
 * license issued to you by Xilinx, and to the maximum extent permitted by
 * applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
 * FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
 * IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
 * and (2) Xilinx shall not be liable (whether in contract or tort, including
 * negligence, or under any other theory of liability) for any loss or damage
 * of any kind or nature related to, arising under or in connection with these
 * materials, including for any direct, or any indirect, special, incidental,
 * or consequential loss or damage (including loss of data, profits, goodwill,
 * or any type of loss or damage suffered as a result of any action brought by
 * a third party) even if such damage or loss was reasonably foreseeable or
 * Xilinx had been advised of the possibility of the same.
 *
 * CRITICAL APPLICATIONS:
 * Xilinx products are not designed or intended to be fail-safe, or for use in
 * any application requiring fail-safe performance, such as life-support or
 * safety devices or systems, Class III medical devices, nuclear facilities,
 * applications related to the deployment of airbags, or any other applications
 * that could lead to death, personal injury, or severe property or
 * environmental damage (individually and collectively, "Critical
 * Applications"). Customer assumes the sole risk and liability of any use of
 * Xilinx products in Critical Applications, subject only to applicable laws
 * and regulations governing limitations on product liability.
 *
 * THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
 * AT ALL TIMES.
 *
 *******************************************************************************/

/*****************************************************************************
 *
 * @file main_app.c
 *
 * Implementation of controlling application.
 *
 ******************************************************************************/

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/fb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "zc702hdmi/hwi_export.h"

#include "main_app.h"
#include "img_filters.h"
#ifndef APF_NOMMAP
#include "apf_lib.h"
#define TIME_STAMP_INIT  unsigned int clock_start, clock_end;  clock_start = apf_clock_counter();  
#define TIME_STAMP  { clock_end = apf_clock_counter(); printf("elapsed time %lu \n", clock_end-clock_start); clock_start = apf_clock_counter();  }
#define CORNER_COUNT 8
#else
#define TIME_STAMP_INIT
#define TIME_STAMP
#endif


// corner processing code
// connects to the network with provided IP address and port
// if it succeds and able to open socket and connect to server,
// it will return 1, otherwise it will return 0.
char init_network()
{
	// attempt to open socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if ( socket_desc == -1 ) {
		printf("Couldn't create Socket.\n");
		return 0;
	}

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons( PORT );

	// connect to remote server
	if (connect ( socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("Connection failed.\n");
		return 0;
	}

	printf("Connected.\n");
	return 1;
}

// sends command to the network, if the network is available
// otherwise attemps to initialize the network and tries again.
void sendCommands(char * serialDataOverNetwork)
{
	if ( isNetworkAlive &&  ( send ( socket_desc, serialDataOverNetwork, strlen(serialDataOverNetwork), 0) < 0 ) ) {
		printf("Send failed.\n");
	}
	else if ( !isNetworkAlive ) {
		// if network somehow failed, then initiate again
		isNetworkAlive = init_network();
	}

	return;
}


// Function which converts value of l within the range of (lMin, lMax) to the value beta within new range i.e. (betaMin, betaMax)
// Referenced link: Part of
// http://ieeexplore.ieee.org/xpl/login.jsp?tp=&arnumber=6780120&url=http%3A%2F%2Fieeexplore.ieee.org%2Fxpls%2Fabs_all.jsp%3Farnumber%3D6780120
int rangeConversion(int betaMin, int betaMax, int l, int lMin, int lMax) {

		l = (l < lMin) ? lMin : l;
//	if (l < lMin)
//		l = lMin + 1;
		l = (l > lMax) ? lMax : l;
//	if (l > lMax)
//		l = lMax - 1;

	return (int)((double)betaMin + ((double)((double)((betaMax - betaMin) * (l - lMin)) / (double)(lMax - lMin))));
}

// Creates a byte of command which is compatible with iRobot
void iRobotCommandFormatVelocity(int velocitySpeed, int rotationAngle, char * stringDataOverNetwork) {

	*(stringDataOverNetwork + 0) = (char)((velocitySpeed & 0xFF00) >> 8);
	*(stringDataOverNetwork + 1) = (char)(velocitySpeed & 0x00FF);

	*(stringDataOverNetwork + 2) = (char)((rotationAngle & 0xFF00) >> 8);
	*(stringDataOverNetwork + 3) = (char)(rotationAngle & 0x00FF);
	*(stringDataOverNetwork + 4) = (char)0; // NULL CHARACTER
}

void corner_processing(unsigned int frame_corner_array) {

	unsigned int A[2], B[2], C[2], D[2];
	// to decide the direction (whether right directed object or left)
	// assign frame corners to appropriate points
	// X -> represents column id
	// Y -> represents row id
	// A -> Left Most point
	// B -> Top most point
	// C -> Right most point
	// D -> Bottom most point
	A[0] = *(unsigned int *)(frame_corner_array + 0 );
	A[1] = *(unsigned int *)(frame_corner_array + 4 );
	B[0] = *(unsigned int *)(frame_corner_array + 16);
	B[1] = *(unsigned int *)(frame_corner_array + 20);
	C[0] = *(unsigned int *)(frame_corner_array + 8 );
	C[1] = *(unsigned int *)(frame_corner_array + 12);
	D[0] = *(unsigned int *)(frame_corner_array + 24);
	D[1] = *(unsigned int *)(frame_corner_array + 28);

	unsigned int width_ab, width_ad;

	unsigned int currentArea;

	// Data to be sent over network
	char serialDataOverNetwork[5]; // it should be 5;

	// it is used to pass argument to atan function
	double division_ad;
	int theta;
	int area; // to measure the distance of object

	width_ab = ((A[0] - B[0]) * (A[0] - B[0])) + ((A[1] - B[1]) * (A[1] - B[1]));
	width_ad = ((A[0] - D[0]) * (A[0] - D[0])) + ((A[1] - D[1]) * (A[1] - D[1]));

	width_ab = (int)sqrt((double)width_ab);
	width_ad = (int)sqrt((double)width_ad);

	area = width_ab*width_ad;

	// if area is small, stop the robot by sending 0s once and wait for object to be large enough
	if (area <= 40)
	{
//		if (!stopCommandFlag) {
//			iRobotCommandFormatVelocity(-1, -1, serialDataOverNetwork);
//
//			sendCommands(serialDataOverNetwork);
//			
//			stopCommandFlag = 1;
//			calibration_counter = 0;
//		}
		
		stopCommandFlag = 1;
		iRobotCommandFormatVelocity(-1, -1, serialDataOverNetwork);
		sendCommands(serialDataOverNetwork);
		calibration_counter = 0;
		return;
	}

	if (stopCommandFlag) {
		if (calibration_counter <= CALIBRATION_TIME_MAX_RANGE) {
			mMax = area;
			printf("\n*********** 00000: MAX MAX MAX MAX *********\n");
			calibration_counter++;
		}
		else if (calibration_counter <= CALIBRATION_TIME_MIN_RANGE) {
			mMin = area;
			printf("\n*********** 11111: MIN MIN MIN MIN *********\n");
			calibration_counter++;
		}
		else if(calibration_counter <= CALIBRATION_COMPLETE_WAIT_TIME) {
			printf("\n*********** 22222: WAIT WAIT WAIT *********\n");
			calibration_counter++;
		}
		else {
			printf("\n*********** Ready *******************\n");
			stopCommandFlag = 0;
		}
	}

	// if object is out of screen, then return otherwise proceed sending command
	// if both are same, don't pursue it further
	if (stopCommandFlag || (mMin >= mMax))	{
		printf("mMin is Greater than mMax\n");
		return;
	}

	// After area > 100 is detected. Proceed with following code
	currentArea = area;

	// get the argument for arctan function
	if ((D[0] - A[0]) != 0)
		division_ad = (double)(D[1] - A[1]) / (D[0] - A[0]);
	else
		division_ad = 50; // for big nubmber, theta will be 89 degree.

	// calculate the arctan. It will always be between 0 to 90.
	theta = (int)(atan(division_ad) * 180 / PI);

	char isRight = 0;
	if (width_ab >= width_ad){

		// left turn
		// left turn requires theta to be subtracted from 90.
		theta = 90 - theta;

		// parameters for conversion
		betaMax = MAX_TURN_LEFT;
		betaMin = MIN_TURN_LEFT;
	}
	else {
		// right turn

		isRight = 1;
		// parameters for conversion
		betaMax = MIN_TURN_RIGHT;
		betaMin = MAX_TURN_RIGHT;
	}

	lMax = MAX_RECT_ANGLE;  // 89
	lMin = MIN_RECT_ANGLE;  // 1

  if ( lMax <= lMin ) return;
  
  // If theta is out of range, return		
  if ( theta >= lMax || theta <= lMin ) return;
  
  // If currentArea is out of range, return
  if ( currentArea >= mMax || currentArea <= mMin ) return;
  
	// alpha (velocitySpeed) and m => for velocity
	// beta (rotationAngle) and l => for radius
	int velocitySpeed = rangeConversion(alphaMin, alphaMax, currentArea, mMin, mMax); // alpha
	int rotationAngle = rangeConversion(betaMin, betaMax, theta, lMin, lMax); // betaMin + ((betaMax - betaMin) * (l - lMin) / (lMax - lMin));

	iRobotCommandFormatVelocity(velocitySpeed, rotationAngle, serialDataOverNetwork); 
	printf("Thta = %d, Vel = %d, Rot = %d, Area = %d, mMin = %d, mMax = %d, isR = %d\n", theta, velocitySpeed, rotationAngle, currentArea, mMin, mMax, isRight);

	// send the command
	sendCommands(serialDataOverNetwork);
	return;
}

void sw_sobel_processing(unsigned int prev_buffer, unsigned int in_buffer, unsigned int out_buffer, unsigned int frame_corner)
{
	int param0=0, param1=0, param2=0;

	printf("Time for FPGA = ");
	TIME_STAMP_INIT
		img_process( (unsigned int *)in_buffer, (unsigned int *)out_buffer, (unsigned int *)frame_corner,  param0, param1, param2);
	TIME_STAMP

	printf("Corners= %u, %u, %u, %u, %u, %u, %u, %u \n", *(unsigned int *)(frame_corner + 0), *(unsigned int *)(frame_corner + 4), *(unsigned int *)(frame_corner + 8), *(unsigned int *)(frame_corner + 12), *(unsigned int *)(frame_corner + 16), *(unsigned int *)(frame_corner + 20), *(unsigned int *)(frame_corner + 24), *(unsigned int *)(frame_corner + 28)); 

	printf("Time for CPU = ");
	clock_start = apf_clock_counter();  
	// after getting the corners from FPGA, process and send it to arudino using socket network
	corner_processing(frame_corner);
	TIME_STAMP
}


void *thread_sw_sync()
{
	unsigned int output_frame[MAX_BUFFER];
	unsigned int input_frame[MAX_BUFFER];

	unsigned int virt_output_frame[MAX_BUFFER];
	unsigned int virt_input_frame[MAX_BUFFER];
	unsigned int virt_frame_corner[MAX_BUFFER];

	int i = 0;
	int offset = 0;

	// starting indices
	unsigned int map_len = NUMPADCOLS * NUMROWS * sizeof(int);
	unsigned int corner_len = CORNER_COUNT * sizeof(int);
	int fd = open("/dev/mem", O_RDWR);

	unsigned char* virtual_addr_in;
	unsigned char* virtual_addr_out;
	unsigned int* corners_out; // (0, 1) = (ax, ay); (2, 3) = (bx, by) ... (6, 7) = (dx, dy)
	
	printf("First time Initializing the Network");
	// connect to server
	isNetworkAlive = init_network();
	printf("Network Connected");

	// initialize with maximum speed.
	alphaMax = MAX_SPEED_FRONT;
	alphaMin = MAX_SPEED_BACK;

	int infrm_index = 2 ,outfrm_index = 0, accel_prev_index=0, accel_in_index = 1, accel_out_index = 1, corner_index=1;

	for (i = 0; i<MAX_BUFFER; i++)
	{
		output_frame[i] = gLayerBase[DISPLAY_LAYER] + offset;
		input_frame[i] = gLayerBase[INPUT_LAYER] + offset;
		offset += ibufferoffset;
		virtual_addr_in = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)input_frame[i]);
		virtual_addr_out = (unsigned char*)mmap(NULL, map_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)output_frame[i]);

		corners_out = (unsigned int *)apf_alloc(corner_len); // 8 location for 2 coordinates of all 4 corners

		virt_input_frame[i] = (unsigned int)virtual_addr_in;
		virt_output_frame[i] = (unsigned int)virtual_addr_out;
		virt_frame_corner[i] = (unsigned int)corners_out;

#ifndef APF_NOMMAP
		apf_mmap(input_frame[i], map_len, virtual_addr_in);
		apf_mmap(output_frame[i], map_len, virtual_addr_out);
#endif
	}
	
	close (fd);

	setCVC_TPGBuffer(outfrm_index,infrm_index);
	sw_sobel_processing(virt_input_frame[accel_prev_index], virt_input_frame[accel_in_index], virt_output_frame[accel_out_index], virt_frame_corner[corner_index] );
	while(1)
	{
		outfrm_index++;
		outfrm_index %= MAX_BUFFER;

		infrm_index++;
		infrm_index  %= MAX_BUFFER;

		setCVC_TPGBuffer(outfrm_index,infrm_index);

		accel_prev_index = accel_in_index;
		accel_in_index++;
		accel_in_index  %= MAX_BUFFER;

		accel_out_index++;
		accel_out_index  %= MAX_BUFFER;

		corner_index++;
		corner_index %= MAX_BUFFER;

		sw_sobel_processing(virt_input_frame[accel_prev_index], virt_input_frame[accel_in_index], virt_output_frame[accel_out_index], virt_frame_corner[corner_index]);
	} 
	// do the following on exit
	// apf_munmap(virtual_addr_in);
	// apf_munmap(virtual_addr_out);
	// munmap((void *)virtual_addr_out, map_len);
	// munmap((void *)virtual_addr_in, map_len);
	//	apf_free(corners);
//	apf_free(virt_frame_corner);
}

int main(int argc, char **argv)
{
	init_all();
	thread_sw_sync(); // Sample code - loop forever, exit with Ctrl-C

	return EXIT_SUCCESS;
}
