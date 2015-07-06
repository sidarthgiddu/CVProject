#include <stdio.h>
#include "ap_video.h"
#include "frame_size.h"
#include "img_cores.h"
#include "hls_stream.h"

#define ABSDIFF(x,y)    ((x>y)? x - y : y - x)
#define ABS(x)          ((x>0)? x : -x)

 ap_uint<1> frame_history[MEDIAN_HISTORY_FRAME_NUMBER][(NUMROWS*NUMCOLS/(SCALE_DOWN_FACTOR*SCALE_DOWN_FACTOR))];
// ap_uint<1> frame_history[MEDIAN_HISTORY_FRAME_NUMBER * NUMROWS*NUMCOLS/(SCALE_DOWN_FACTOR*SCALE_DOWN_FACTOR)];

 void median_filter_time(unsigned short median_time_in[NUMROWS*NUMCOLS/(SCALE_DOWN_FACTOR*SCALE_DOWN_FACTOR)],unsigned short median_time_out[NUMROWS*NUMCOLS/(SCALE_DOWN_FACTOR * SCALE_DOWN_FACTOR)]) {
#pragma AP INTERFACE ap_fifo port=median_time_in
#pragma AP INTERFACE ap_fifo port=median_time_out

#pragma AP array_partition variable=frame_history block factor=7

	 int row, col;
	 unsigned char pixel_in, pixel_out_char;
	 unsigned int pixel_out_color_int;
	 ap_uint<1> pixel_in_bit, pixel_out;
	 ap_uint<1> bit_history[MEDIAN_HISTORY_FRAME_NUMBER];
#pragma AP array_partition variable=bit_history block factor=7

	 ap_uint<MEDIAN_HISTORY_FRAME_NUMBER> bit_pattern;
	 ap_uint<1> idx_bit;
	 static unsigned int frame_number = 0;

	 unsigned char idx, i;

	 int numRows = NUMROWS>>SCALE_DOWN_FACTOR_SHIFT;
	 int numCols = NUMCOLS>>SCALE_DOWN_FACTOR_SHIFT;
	 int frameSize = numRows * numCols;
	 for(row = 0; row < numRows; row++) {
		 for(col = 0; col < numCols; col++) {
#pragma AP PIPELINE II = 1
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS DEPENDENCE array inter false
			 pixel_in 		= median_time_in[row * numCols + col];

			 pixel_in_bit = 0;
			 if(pixel_in == 255) pixel_in_bit = 1;

			 //			frame_history[(frame_number * frameSize) + (row * numCols) + col] = pixel_in_bit;
			 frame_history[frame_number][(row * numCols) + col] = pixel_in_bit;
			 /*
			    bit_history[frame_number] = pixel_in_bit;

			    idx = frame_number + 1;
			 // only iterate for remaining frame because current frame value is already
			 // stored in bit_history
			 for(i = 0; i < (MEDIAN_HISTORY_FRAME_NUMBER - 1); i++) {
#pragma AP PIPELINE II = 1
if( idx == MEDIAN_HISTORY_FRAME_NUMBER) {
idx = 0;
}      

bit_history[idx] = frame_history[idx * frameSize + row * numCols + col];			
			 //				bit_history[idx] = frame_history[idx * ((row * numCols) + col)];

			 idx++;
			 }
			 */


			 // Loop is partitioned, so, we can access without conflicts
			 for(i = 0; i < MEDIAN_HISTORY_FRAME_NUMBER; i++) {
#pragma AP UNROLL
				 bit_history[i] = frame_history[i][(row * numCols) + col];
			 }

// TODO: Parameterize if possible
bit_pattern = (bit_history[0], bit_history[1], bit_history[2], bit_history[3], bit_history[4], bit_history[5], bit_history[6]);

switch(bit_pattern) {
	case 0b0000000:  pixel_out = 0; break;
	case 0b0000001:  pixel_out = 0; break;
	case 0b0000010:  pixel_out = 0; break;
	case 0b0000011:  pixel_out = 0; break; 
	case 0b0000100:  pixel_out = 0; break;
	case 0b0000101:  pixel_out = 0; break;
	case 0b0000110:  pixel_out = 0; break;
	case 0b0000111:  pixel_out = 0; break;
	case 0b0001000:  pixel_out = 0; break;
	case 0b0001001:  pixel_out = 0; break;
	case 0b0001010:  pixel_out = 0; break;
	case 0b0001011:  pixel_out = 0; break;
	case 0b0001100:  pixel_out = 0; break;
	case 0b0001101:  pixel_out = 0; break;
	case 0b0001110:  pixel_out = 0; break;
			 //				case 0b0001111:  pixel_out = 1; break;
	case 0b0010000:  pixel_out = 0; break;
	case 0b0010001:  pixel_out = 0; break;
	case 0b0010010:  pixel_out = 0; break;
	case 0b0010011:  pixel_out = 0; break;
	case 0b0010100:  pixel_out = 0; break;
	case 0b0010101:  pixel_out = 0; break;
	case 0b0010110:  pixel_out = 0; break;
			 //				case 0b0010111:  pixel_out = 1; break;
	case 0b0011000:  pixel_out = 0; break;
	case 0b0011001:  pixel_out = 0; break;
	case 0b0011010:  pixel_out = 0; break;
			 //				case 0b0011011:  pixel_out = 1; break;
	case 0b0011100:  pixel_out = 0; break;
			 //				case 0b0011101:  pixel_out = 1; break;
			 //				case 0b0011110:  pixel_out = 1; break;
			 //				case 0b0011111:  pixel_out = 1; break;


	case 0b0100000:  pixel_out = 0; break;
	case 0b0100001:  pixel_out = 0; break;
	case 0b0100010:  pixel_out = 0; break;
	case 0b0100011:  pixel_out = 0; break; 
	case 0b0100100:  pixel_out = 0; break;
	case 0b0100101:  pixel_out = 0; break;
	case 0b0100110:  pixel_out = 0; break;
			 //				case 0b0100111:  pixel_out = 1; break;
	case 0b0101000:  pixel_out = 0; break;
	case 0b0101001:  pixel_out = 0; break;
	case 0b0101010:  pixel_out = 0; break;
			 //				case 0b0101011:  pixel_out = 1; break;
	case 0b0101100:  pixel_out = 0; break;
			 //				case 0b0101101:  pixel_out = 1; break;
			 //				case 0b0101110:  pixel_out = 1; break;
			 //				case 0b0101111:  pixel_out = 1; break;
	case 0b0110000:  pixel_out = 0; break;
	case 0b0110001:  pixel_out = 0; break;
	case 0b0110010:  pixel_out = 0; break;
			 //				case 0b0110011:  pixel_out = 1; break;
	case 0b0110100:  pixel_out = 0; break;
			 //				case 0b0110101:  pixel_out = 1; break;
			 //				case 0b0110110:  pixel_out = 1; break;
			 //				case 0b0110111:  pixel_out = 1; break;
	case 0b0111000:  pixel_out = 0; break;
			 //				case 0b0111001:  pixel_out = 1; break;
			 //				case 0b0111010:  pixel_out = 1; break;
			 //				case 0b0111011:  pixel_out = 1; break;
			 //				case 0b0111100:  pixel_out = 1; break;
			 //				case 0b0111101:  pixel_out = 1; break;
			 //				case 0b0111110:  pixel_out = 1; break;
			 //				case 0b0111111:  pixel_out = 1; break;

	case 0b1000000:  pixel_out = 0; break;
	case 0b1000001:  pixel_out = 0; break;
	case 0b1000010:  pixel_out = 0; break;
	case 0b1000011:  pixel_out = 0; break; 
	case 0b1000100:  pixel_out = 0; break;
	case 0b1000101:  pixel_out = 0; break;
	case 0b1000110:  pixel_out = 0; break;
			 //				case 0b1000111:  pixel_out = 1; break;
	case 0b1001000:  pixel_out = 0; break;
	case 0b1001001:  pixel_out = 0; break;
	case 0b1001010:  pixel_out = 0; break;
			 //				case 0b1001011:  pixel_out = 1; break;
	case 0b1001100:  pixel_out = 0; break;
			 //				case 0b1001101:  pixel_out = 1; break;
			 //				case 0b1001110:  pixel_out = 1; break;
			 //				case 0b1001111:  pixel_out = 1; break;
	case 0b1010000:  pixel_out = 0; break;
	case 0b1010001:  pixel_out = 0; break;
	case 0b1010010:  pixel_out = 0; break;
			 //				case 0b1010011:  pixel_out = 1; break;
	case 0b1010100:  pixel_out = 0; break;
			 //				case 0b1010101:  pixel_out = 1; break;
			 //				case 0b1010110:  pixel_out = 1; break;
			 //				case 0b1010111:  pixel_out = 1; break;
	case 0b1011000:  pixel_out = 0; break;
			 //				case 0b1011001:  pixel_out = 1; break;
			 //				case 0b1011010:  pixel_out = 1; break;
			 //				case 0b1011011:  pixel_out = 1; break;
			 //				case 0b1011100:  pixel_out = 1; break;
			 //				case 0b1011101:  pixel_out = 1; break;
			 //				case 0b1011110:  pixel_out = 1; break;
			 //				case 0b1011111:  pixel_out = 1; break;

	case 0b1100000:  pixel_out = 0; break;
	case 0b1100001:  pixel_out = 0; break;
	case 0b1100010:  pixel_out = 0; break;
			 //				case 0b1100011:  pixel_out = 1; break; 
	case 0b1100100:  pixel_out = 0; break;
			 //				case 0b1100101:  pixel_out = 1; break;
			 //				case 0b1100110:  pixel_out = 1; break;
			 //				case 0b1100111:  pixel_out = 1; break;
	case 0b1101000:  pixel_out = 0; break;
			 //				case 0b1101001:  pixel_out = 1; break;
			 //				case 0b1101010:  pixel_out = 1; break;
			 //				case 0b1101011:  pixel_out = 1; break;
			 //				case 0b1101100:  pixel_out = 1; break;
			 //				case 0b1101101:  pixel_out = 1; break;
			 //				case 0b1101110:  pixel_out = 1; break;
			 //				case 0b1101111:  pixel_out = 1; break;
	case 0b1110000:  pixel_out = 0; break;
			 //				case 0b1110001:  pixel_out = 1; break;
			 //				case 0b1110010:  pixel_out = 1; break;
			 //				case 0b1110011:  pixel_out = 1; break;
			 //				case 0b1110100:  pixel_out = 1; break;
			 //				case 0b1110101:  pixel_out = 1; break;
			 //				case 0b1110110:  pixel_out = 1; break;
			 //				case 0b1110111:  pixel_out = 1; break;
			 //				case 0b1111000:  pixel_out = 1; break;
			 //				case 0b1111001:  pixel_out = 1; break;
			 //				case 0b1111010:  pixel_out = 1; break;
			 //				case 0b1111011:  pixel_out = 1; break;
			 //				case 0b1111100:  pixel_out = 1; break;
			 //				case 0b1111101:  pixel_out = 1; break;
			 //				case 0b1111110:  pixel_out = 1; break;
			 //				case 0b1111111:  pixel_out = 1; break;


	default: pixel_out = 1; break;
}

pixel_out_char = 0;

if (pixel_out == 1) pixel_out_char = 255;

if ( row < numRows && col < numCols ) {
	median_time_out[row * numCols + col] = pixel_out_char;
}
} // end of inner for loop (columns)
} // end of outer for loop (rows)

frame_number++;

// reset the frame counter to 0. It wraps arround and behaves as FIFO during previous frame eviction
if (frame_number >= MEDIAN_HISTORY_FRAME_NUMBER) {
	frame_number = 0;
}
}  // end of function
