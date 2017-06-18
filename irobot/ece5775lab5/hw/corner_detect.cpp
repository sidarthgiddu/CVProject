#include <stdio.h>
#include "ap_video.h"
#include "frame_size.h"
#include "image_cores.h"
#include "hls_stream.h"

//Main function for Corner Detection
//This function includes corner detection of extreme points
void corner_detect(ap_uint<8> median_out[NUMROWS*NUMCOLS], ap_uint<8> corner_data_out[NUMROWS*NUMCOLS], unsigned int com_in[6], unsigned int com_out[6], unsigned int frame_corners[2*CORNER_ELEMENTS])
{
#pragma AP INTERFACE ap_fifo port=median_out
#pragma AP INTERFACE ap_fifo port=corner_data_out
#pragma AP INTERFACE ap_fifo port=frame_corners
	int row;
	int col;

	unsigned char pixel_in;
	unsigned char pixel_out;
	bool isValid;

	// initial corner location
	ap_uint<11> ymax_red = 0;
	ap_uint<11> xmax_red = 0;
	ap_uint<11> xmin_red = NUMCOLS;
	ap_uint<11> ymin_red = NUMROWS;
	ap_uint<11> ymax_blue = 0;
	ap_uint<11> xmax_blue = 0;
	ap_uint<11> xmin_blue = NUMCOLS;
	ap_uint<11> ymin_blue = NUMROWS;
	ap_uint<11> temp_corners_red[CORNER_ELEMENTS] = {0};
	ap_uint<11> temp_corners_blue[CORNER_ELEMENTS] = {0};
//#pragma AP array_partition variable=temp_corners complete dim=0

//	ap_uint<88> temp;


	static ap_uint<11> prevCorner_red[CORNER_ELEMENTS] = {0};
	static ap_uint<11> prevCorner_blue[CORNER_ELEMENTS] = {0};
//#pragma AP array_partition variable=prevCorner complete dim=0

	static ap_uint<11> fifo_frame_corners_red[CORNER_HISTORY_FRAME_NUMBER][CORNER_ELEMENTS] = {0};
	static ap_uint<11> fifo_frame_corners_blue[CORNER_HISTORY_FRAME_NUMBER][CORNER_ELEMENTS] = {0};
//#pragma AP array_partition variable=fifo_frame_corners complete dim=0

	//static ap_uint<11> sorted_corners_red[CORNER_HISTORY_FRAME_NUMBER][CORNER_ELEMENTS] = {0};
	//static ap_uint<11> sorted_corners_blue[CORNER_HISTORY_FRAME_NUMBER][CORNER_ELEMENTS] = {0};
//#pragma AP array_partition variable=sorted_corners complete dim=0

	static unsigned int corner_frame_counter_red = 0;
	static unsigned int corner_frame_counter_blue = 0;

	int i,j;
	//ap_uint<2> isPoint;
	
  for(row = 0; row < NUMROWS; row++) {
    for(col = 0; col < NUMCOLS; col++) {
      corner_data_out[row * NUMCOLS + col] = median_out[row * NUMCOLS + col];
#pragma AP PIPELINE II = 1
#pragma HLS DEPENDENCE array inter false      

	    if ( ( (row & 3) == 0 ) && ( ( (col & 3) == 0) ) ) {
		    pixel_in = median_out[ row * NUMCOLS + col ];
		    isValid = (pixel_in > 0);
		    // calculate corners
        if(isValid) 
		    {
          if (pixel_in == 1){ // red
		          if ( col < xmin_red ) {
			          xmin_red = col;
			          temp_corners_red[0] = col;
			          temp_corners_red[1] = row;
		          }

		          if( col >= xmax_red ) {
			          xmax_red = col;
			          temp_corners_red[2] = col;
			          temp_corners_red[3] = row;
		          }

		          if( row <= ymin_red ) {
			          ymin_red = row;
			          temp_corners_red[4] = col;
			          temp_corners_red[5] = row;	
		          }

		          if( row > ymax_red ) {
			          ymax_red = row;
			          temp_corners_red[6] = col;
			          temp_corners_red[7] = row;
		          }
		        }
		        else if (pixel_in == 2){ // blue
              if ( col < xmin_blue ) {
				          xmin_blue = col;
				          temp_corners_blue[0] = col;
				          temp_corners_blue[1] = row;
			          }

			          if( col >= xmax_blue ) {
				          xmax_blue = col;
				          temp_corners_blue[2] = col;
				          temp_corners_blue[3] = row;
			          }

			          if( row <= ymin_blue ) {
				          ymin_blue = row;
				          temp_corners_blue[4] = col;
				          temp_corners_blue[5] = row;	
			          }

			          if( row > ymax_blue ) {
				          ymax_blue = row;
				          temp_corners_blue[6] = col;
				          temp_corners_blue[7] = row;
			          }
          }
        }
        
	    // If there is a red color object, it will be overlapped by white color at every 4 pixels
	    //corner_data_out[row * NUMPADCOLS + col] =  pixval;
      }
      //else {
	      //corner_data_out[ row * NUMPADCOLS + col] = 0;
      //}
	
    } // end of col iteration
    
    // pad the remaining column entries with 0s (these values are outside screen resolution, so we don't care
    /*for (col = NUMCOLS; col < NUMPADCOLS; col++) {
#pragma AP PIPELINE II = 1
      corner_data_out[ row * NUMPADCOLS + col] = 128;
    } // print out all the corners at the end*/

  } // end of row iteration
  
  for ( i = 0; i < 6; i++){
  #pragma AP UNROLL
    com_out[i] = com_in[i];
  }
  
  frame_corners[0] = temp_corners_red[0]; //  A(X) = A[0]	
  frame_corners[1] = temp_corners_red[1]; //  A(y) = A[1]	
  frame_corners[2] = temp_corners_red[2]; //  C(X) = C[0]
  frame_corners[3] = temp_corners_red[3]; //  C(Y) = C[1]	
  frame_corners[4] = temp_corners_red[4]; //  B(X) = B[0]	
  frame_corners[5] = temp_corners_red[5]; //  B(Y) = B[1]	
  frame_corners[6] = temp_corners_red[6]; //  D(X) = D[0]	
  frame_corners[7] = temp_corners_red[7]; //  D(Y) = D[1]
  frame_corners[8] = temp_corners_blue[0]; //  A(X) = A[0]	
  frame_corners[9] = temp_corners_blue[1]; //  A(y) = A[1]	
  frame_corners[10] = temp_corners_blue[2]; //  C(X) = C[0]
  frame_corners[11] = temp_corners_blue[3]; //  C(Y) = C[1]	
  frame_corners[12] = temp_corners_blue[4]; //  B(X) = B[0]	
  frame_corners[13] = temp_corners_blue[5]; //  B(Y) = B[1]	
  frame_corners[14] = temp_corners_blue[6]; //  D(X) = D[0]	
  frame_corners[15] = temp_corners_blue[7]; //  D(Y) = D[1]
  
	/*  
  ap_uint<11> temp_corner0_red;
  ap_uint<11> temp_corner1_red;
  ap_uint<11> temp_corner2_red;
  ap_uint<11> temp_corner3_red;
  ap_uint<11> temp_corner4_red;
  ap_uint<11> temp_corner5_red;
  ap_uint<11> temp_corner6_red;
  ap_uint<11> temp_corner7_red;
  
  ap_uint<11> temp_corner0_blue;
  ap_uint<11> temp_corner1_blue;
  ap_uint<11> temp_corner2_blue;
  ap_uint<11> temp_corner3_blue;
  ap_uint<11> temp_corner4_blue;
  ap_uint<11> temp_corner5_blue;
  ap_uint<11> temp_corner6_blue;
  ap_uint<11> temp_corner7_blue; 
	
	
  temp_corner0_red = temp_corners_red[0];
  temp_corner1_red = temp_corners_red[1];
  temp_corner2_red = temp_corners_red[2];
  temp_corner3_red = temp_corners_red[3];
  temp_corner4_red = temp_corners_red[4];
  temp_corner5_red = temp_corners_red[5];
  temp_corner6_red = temp_corners_red[6];
  temp_corner7_red = temp_corners_red[7];
  prevCorner_red[0] = temp_corner0_red;
  prevCorner_red[1] = (temp_corner0_red > 0) ? ((unsigned int)temp_corner1_red) : 0;
  prevCorner_red[2] = (temp_corner0_red > 0) ? ((unsigned int)temp_corner2_red) : 0;
  prevCorner_red[3] = (temp_corner0_red > 0) ? ((unsigned int)temp_corner3_red) : 0;
  prevCorner_red[4] = (temp_corner0_red > 0) ? ((unsigned int)temp_corner4_red) : 0;
  prevCorner_red[5] = (temp_corner0_red > 0) ? ((unsigned int)temp_corner5_red) : 0;
  prevCorner_red[6] = (temp_corner0_red > 0) ? ((unsigned int)temp_corner6_red) : 0;
  prevCorner_red[7] = (temp_corner0_red > 0) ? ((unsigned int)temp_corner7_red) : 0;
  
  temp_corner0_blue = temp_corners_blue[0];
  temp_corner1_blue = temp_corners_blue[1];
  temp_corner2_blue = temp_corners_blue[2];
  temp_corner3_blue = temp_corners_blue[3];
  temp_corner4_blue = temp_corners_blue[4];
  temp_corner5_blue = temp_corners_blue[5];
  temp_corner6_blue = temp_corners_blue[6];
  temp_corner7_blue = temp_corners_blue[7];
  prevCorner_blue[0] = temp_corner0_blue;
  prevCorner_blue[1] = (temp_corner0_blue > 0) ? ((unsigned int)temp_corner1_blue) : 0;
  prevCorner_blue[2] = (temp_corner0_blue > 0) ? ((unsigned int)temp_corner2_blue) : 0;
  prevCorner_blue[3] = (temp_corner0_blue > 0) ? ((unsigned int)temp_corner3_blue) : 0;
  prevCorner_blue[4] = (temp_corner0_blue > 0) ? ((unsigned int)temp_corner4_blue) : 0;
  prevCorner_blue[5] = (temp_corner0_blue > 0) ? ((unsigned int)temp_corner5_blue) : 0;
  prevCorner_blue[6] = (temp_corner0_blue > 0) ? ((unsigned int)temp_corner6_blue) : 0;
  prevCorner_blue[7] = (temp_corner0_blue > 0) ? ((unsigned int)temp_corner7_blue) : 0;
	// frame_corners[0 -> 7] = assign calculated corners
	
	
	// bubble sort - naive
	unsigned int corner_id;
	unsigned int temp_num;
	unsigned int num_length = CORNER_HISTORY_FRAME_NUMBER;
	
  // iterate for each 8 corners - UNROLL the loop because it can happen in parallel
  for ( corner_id = 0; corner_id < CORNER_ELEMENTS; corner_id++) {
#pragma AP UNROLL
#pragma AP PIPELINE II=1
    sorted_corners_red[corner_frame_counter_red][corner_id] = prevCorner_red[corner_id];

    // after copying data, we can sort it using sorted_corners history
    for (i = 0; i <= num_length; i++) {
#pragma AP PIPELINE II = 1
	    for (j = 1; j < num_length; j++ ) {
#pragma AP PIPELINE II = 1
		    // swap the value if current is less than previous
		    if ( sorted_corners_red[j - 1][corner_id] > sorted_corners_red[j][corner_id] ) {
			    temp_num = sorted_corners_red[j][corner_id];
			    sorted_corners_red[j][corner_id] = sorted_corners_red[j - 1][corner_id];
			    sorted_corners_red[ j - 1 ][corner_id] = temp_num;
		    }
	    }
    }
  }
  
  // iterate for each 8 corners - UNROLL the loop because it can happen in parallel
  for ( corner_id = 0; corner_id < CORNER_ELEMENTS; corner_id++) {
#pragma AP UNROLL
#pragma AP PIPELINE II=1
    sorted_corners_blue[corner_frame_counter_blue][corner_id] = prevCorner_blue[corner_id];

    // after copying data, we can sort it using sorted_corners history
    for (i = 0; i <= num_length; i++) {
#pragma AP PIPELINE II = 1
	    for (j = 1; j < num_length; j++ ) {
#pragma AP PIPELINE II = 1
		    // swap the value if current is less than previous
		    if ( sorted_corners_blue[j - 1][corner_id] > sorted_corners_blue[j][corner_id] ) {
			    temp_num = sorted_corners_blue[j][corner_id];
			    sorted_corners_blue[j][corner_id] = sorted_corners_blue[j - 1][corner_id];
			    sorted_corners_blue[ j - 1 ][corner_id] = temp_num;
		    }
	    }
    }
  }
	  
	corner_frame_counter_red++;
	if (corner_frame_counter_red >= CORNER_HISTORY_FRAME_NUMBER )
		corner_frame_counter_red = 0;
	else if (corner_frame_counter_red == 5)
	  corner_frame_counter_red = 6;
	
	corner_frame_counter_blue++;
	if (corner_frame_counter_blue >= CORNER_HISTORY_FRAME_NUMBER )
		corner_frame_counter_blue = 0;
	else if (corner_frame_counter_blue == 5)
	  corner_frame_counter_blue = 6;

  frame_corners[0] = sorted_corners_red[5][0]; //  A(X) = A[0]	
  frame_corners[1] = sorted_corners_red[5][1]; //  A(y) = A[1]	
  frame_corners[2] = sorted_corners_red[5][2]; //  C(X) = C[0]
  frame_corners[3] = sorted_corners_red[5][3]; //  C(Y) = C[1]	
  frame_corners[4] = sorted_corners_red[5][4]; //  B(X) = B[0]	
  frame_corners[5] = sorted_corners_red[5][5]; //  B(Y) = B[1]	
  frame_corners[6] = sorted_corners_red[5][6]; //  D(X) = D[0]	
  frame_corners[7] = sorted_corners_red[5][7]; //  D(Y) = D[1]
  frame_corners[8] = sorted_corners_blue[5][0]; //  A(X) = A[0]	
  frame_corners[9] = sorted_corners_blue[5][1]; //  A(y) = A[1]	
  frame_corners[10] = sorted_corners_blue[5][2]; //  C(X) = C[0]
  frame_corners[11] = sorted_corners_blue[5][3]; //  C(Y) = C[1]	
  frame_corners[12] = sorted_corners_blue[5][4]; //  B(X) = B[0]	
  frame_corners[13] = sorted_corners_blue[5][5]; //  B(Y) = B[1]	
  frame_corners[14] = sorted_corners_blue[5][6]; //  D(X) = D[0]	
  frame_corners[15] = sorted_corners_blue[5][7]; //  D(Y) = D[1]
  */
  		
}
