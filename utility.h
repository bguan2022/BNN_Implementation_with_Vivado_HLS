#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "ap_int.h"

#ifndef __SYNTHESIS__ //Enable Printing of intermediate values if not synthesized
	#include <iostream>
#endif

#define input_row 28
#define input_col 28

#define out1_row 24
#define out1_col 24

#define out2_row 20
#define out2_col 20

#define out3_row 16
#define out3_col 16

#define weight_row 5
#define weight_col 5
#define weight_window_size 25

#define fc_weight_row 256
#define fc_weight_col 10

// Type Definitions - width of each data
typedef ap_int<6> dataSize_t; //Size variables 5-bit
typedef ap_uint<weight_col*weight_row> weight_t; // Size of a weight 25-bit
typedef ap_uint<weight_col*weight_row> activation_t; // Size of a window 25-bit

typedef ap_uint<input_col> input1_t;	//Layer 1 Activation 28-bit
typedef ap_uint<out1_col> data_layer2;   //Layer 1 Output Layer 2 Input 24-bit
typedef ap_uint<out2_col> data_layer3; //Layer 2 Output Layer 3 Input 20-bit

typedef ap_uint<out3_col> data_fc;	//Data width for fully-connected layer (Row Stationary) 16-bit
typedef ap_uint<out3_col> wt_fc;	//Weight width for fully-connected layer (Row Stationary) 16-bit
typedef ap_uint<fc_weight_col> out_t; //10-bit Output

typedef ap_uint<5> conv_result; //Intermediate Conv Result (before batch norm) # of 1s in binary representation
typedef ap_int<6> value_t; //Actual numeric value of Conv Result
typedef ap_int<11> fc_result; //Numeric FC Multiplication Result

//Helper function declarations
//Pass-by-value vs pass-by reference? Will it make a difference during synthesis

void PopCount(ap_uint<(weight_row*weight_col)> in, int & out);
void conv(dataSize_t result_size, dataSize_t window_size, activation_t* a, weight_t & w, conv_result* o);
void batch_norm(dataSize_t in_rows, dataSize_t in_cols, dataSize_t weight_rows, dataSize_t weight_cols, conv_result* data_in, activation_t * data_out, value_t mean, value_t var, value_t threshold);
void fc(activation_t* a, wt_fc* w, out_t & o, fc_result mean, fc_result var, fc_result threshold);

#endif
