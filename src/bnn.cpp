#include "bnn.h"



void BNN_3(activation_t input_activation[],
	weight_t weight_layer1,
	weight_t weight_layer2,
	weight_t weight_layer3,
	wt_fc weight_FC[],
	out_t output,
	value_t batch_mean[],
	value_t batch_var[],
	value_t batch_threshold[],
	fc_result batch_fc[]
){

	//Simple Storage Allocation
	conv_result conv_res[(out1_row*out1_col)]; //Store Intermediate Result
	activation_t IO_buffer[out2_row*out2_col]; //Buffer for input and output
	data_fc out3[out3_row];

	//First Layer
	conv(out1_row,(weight_row*weight_col),input_activation, weight_layer1, conv_res);
	batch_norm(out1_row, out1_col, weight_row, weight_col, conv_res, IO_buffer, batch_mean[0], batch_var[0], batch_threshold[0]);

	//Print Output from first layer
#ifndef __SYNTHESIS__
	std::cout << "Convolution Layer 1 Result:" << std::endl;
	for(int i = 0; i < out1_row; i++){
		for(int j = 0; j < out1_col; j++){
			 for(dataSize_t ii = 0; ii < out2_row; ii++){	//ii - Row index of window topleft
				 dataSize_t p = (i-ii); //Row index of element within the window
				 if((p >= 0) && (p < weight_row)){ //Check no out-of-bounds
					 for(dataSize_t jj = 0; jj < out2_col; jj++){ //jj - Column index of window topleft
						dataSize_t q = (j-jj); //Column index of element within the window
						if((q >= 0)  && (q < weight_col)){ //Check no out-of-bounds
							if(j) std::cout << ",";
							//Finds the first occurrence of the element in the packed array
							std::cout << IO_buffer[ii*out2_col+jj][p*weight_col+q];
							break;
						}
					}
					 break;
				 }
			 }
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
#endif
/*
#ifndef __SYNTHESIS__ //Print IO Buffer
	for(int i = 0; i < out2_row*out2_col;i++){
		std::cout <<  "IO Buffer element " << i << ": " << IO_buffer[i].to_string(2) << std::endl;//Binary Output
	}
#endif
*/

	//Second Layer
	conv(out2_row,(weight_row*weight_col),IO_buffer, weight_layer2, conv_res);
	batch_norm(out2_row, out2_col,weight_row, weight_col, conv_res, IO_buffer, batch_mean[1], batch_var[1], batch_threshold[1]);

	//Print Output from second layer
#ifndef __SYNTHESIS__
	std::cout << "Convolution Layer 2 Result:" << std::endl;
	for(int i = 0; i < out2_row; i++){
		for(int j = 0; j < out2_col; j++){
			 for(dataSize_t ii = 0; ii < out3_row; ii++){	//ii - Row index of window topleft
				 dataSize_t p = (i-ii); //Row index of element within the window
				 if((p >= 0) && (p < weight_row)){ //Check no out-of-bounds
					 for(dataSize_t jj = 0; jj < out3_col; jj++){ //jj - Column index of window topleft
						dataSize_t q = (j-jj); //Column index of element within the window
						if((q >= 0)  && (q < weight_col)){ //Check no out-of-bounds
							if(j) std::cout << ",";
							//Finds the first occurrence of the element in the packed array
							std::cout << IO_buffer[ii*out3_col+jj][p*weight_col+q];
							break;
						}
					}
					 break;
				 }
			 }
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
#endif

	//Third Layer
	conv(out3_row,(weight_row*weight_col),IO_buffer, weight_layer3, conv_res);
	batch_norm(out3_row, out3_col, 1, out3_col, conv_res, IO_buffer, batch_mean[2], batch_var[2], batch_threshold[2]);

	//Print Output from third layer
#ifndef __SYNTHESIS__
	std::cout << "Convolution Layer 3 Result:" << std::endl;
	for(int i = 0; i < out3_row; i++){
		for(int j = 0; j < out3_col; j++){
			if(j) std::cout << ",";
			std::cout << IO_buffer[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
#endif

	//Fully-Connected Layer
	fc(IO_buffer, weight_FC, output, batch_fc[0],batch_fc[1], batch_fc[2]);
}
