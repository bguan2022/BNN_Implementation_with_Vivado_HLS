#include "utility.h"

void PopCount(ap_uint<(weight_row*weight_col)> in, conv_result & out)
 {
   out = 0;
   POP: for ( int i = 0; i < in.length(); i++ ) {
     out += in[i];
   }
 }

void conv(dataSize_t result_size, dataSize_t window_size, activation_t* a, weight_t & w, conv_result* o)
{
	activation_t temp;
	CONV: for(dataSize_t i = 0; i < result_size; i++){
		temp = ~(a[i]^w);
		PopCount(temp, o[i]);
	}

}

void batch_norm(dataSize_t in_rows, dataSize_t in_cols, dataSize_t weight_rows, dataSize_t weight_cols, conv_result* data_in, activation_t* data_out, value_t mean, value_t var, value_t threshold)
{
	value_t buffer;
	//Compute Output dimension w/ sliding window size of next layer
	dataSize_t out_rows = in_rows-weight_rows+1;
	dataSize_t out_cols = in_cols-weight_cols+1;
	ROW: for (dataSize_t i = 0; i < in_rows;i++){
		COL: for (dataSize_t j = 0; j < in_cols; j++){
			buffer = 2*data_in[i*in_cols+j]-weight_window_size; //Convert from # of 1s to actual numeric result //try shift instead of *2
			buffer = (buffer-mean)/var;
			if(buffer >= threshold){
				buffer = 1;
			}
			else{
				buffer = 0;
			}
			//Place Binary Result into the output
			ROW_WINDOW_POS: for(dataSize_t ii = 0; ii < out_rows; ii++){	//ii - Row index of window topleft
				COL_WINDOW_POS: for(dataSize_t jj = 0; jj < out_cols; jj++){ //jj - Column index of window topleft
					dataSize_t p = (i-ii); //Row index of element within the window
					dataSize_t q = (j-jj); //Column index of element within the window
					if((p >= 0) && (q >= 0) && (p < weight_rows) && (q < weight_cols)){ //Check no out-of-bounds
						data_out[ii*out_cols+jj][p*weight_cols+q] = buffer;
					}
				}
			}
		}
	}
}

//Fully-connected Layer
void fc(activation_t* a, wt_fc* w, out_t & o, fc_result mean, fc_result var, fc_result threshold){
	fc_result mac_buffer;
	data_fc xnor_buffer;
	FC: for(dataSize_t i = 0; i < fc_weight_col;i++){
		mac_buffer = 0;
		//Calculation of one output - Assume Output-Stationary
		MAC: for(dataSize_t j = 0; j < out3_row;j++){
			xnor_buffer= ~(a[j]^w[i*out3_row+j]); //Multiplication
			//PopCount
			POPCOUNT: for(dataSize_t k = 0; k < out3_col; k++){
				mac_buffer += xnor_buffer[k];
			}
		}
		//Batch-Normalization
		mac_buffer = 2*mac_buffer - fc_weight_row; //11-bit needed for *2 operation
		mac_buffer = (mac_buffer-mean)/var;
		if(mac_buffer >= threshold){
			o[i] = 1;
		}
		else{
			o[i] = 0;
		}
	}
}
