#ifndef __BNN_H__
#define __BNN_H__

#include "utility.h"

void BNN_3(
		activation_t input_activation[],	//Assume the input has been processed and flattened
		weight_t weight_layer1,
		weight_t weight_layer2,
		weight_t weight_layer3,
		wt_fc weight_FC[],
		out_t & output,
		/* Potential Output for intermediate values
		 *int output_layer1[(input_row-weight_row+1)*(input_row-weight_row+1)]
		 *,int output_layer2[(input_row-2*(weight_row-1))*(input_row-2*(weight_row-1)]
		 *,int output_layer3[(input_row-3*(weight_row-1))*(input_row-3*(weight_row-1))]
		 */
		value_t batch_mean[],
		value_t batch_var[],
		value_t batch_threshold[],
		fc_result batch_fc[] //{mean,variance,threshold}
		);

#endif // __BNN_H__ not defined
