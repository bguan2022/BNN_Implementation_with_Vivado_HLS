#include <stdio.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
//#include <string>

#include "bnn.h"


using namespace std;

int read_in_activation(int length, int width, string file_name, int* a);
int read_in_weight(int length, int width, string file_name, int* w);

int main(int argc, char *argv[])
{

    int activation_length = 28;
    int activation_width = 28;
    int weight_length = 5;
    int weight_width = 5;
    int window_length= weight_length*weight_width;

    //The first layer is a 28x28 input
    int *activation = new int[activation_length*activation_width];

    //The weight for the first layer is 5x5 input
    int *weight1 = new int[weight_length*weight_width];

    //The weight for the second layer
    int *weight2 = new int[weight_length*weight_width];

    //The weight for the third later
    int *weight3 = new int[weight_length*weight_width];

    //The weight for fully-connected layer
    int *weightFC = new int[fc_weight_row*fc_weight_col];


    out_t output;
    string input_dir = "E:/EE/ESE_5960/Project_2/Test_Inputs/";
    string input_filename = "input_1.txt";
    string l1weight_filename= "weight_1.txt";
    string l2weight_filename= "weight_1.txt";
    string l3weight_filename= "weight_1.txt";
    string fcweight_filename="fc_weight.txt";

    cout << "--------------------------------------------------------------" << endl;
    cout << "Activation Read:" << endl << endl;
    //Read in the two dimension binary input activation and we expand it to a one dimension vector
    //int read_in_activation(int length, int width, char* file_name, int* a)
    read_in_activation(activation_length, activation_width, input_dir+input_filename, activation);

    //Transform Input according to convolution window
    activation_t * in = new activation_t[out1_row*out1_col];

    cout << endl << "--------------------------------------------------------------" << endl << endl;

	for(int i = 0; i < out1_row; i++){
		for (int j = 0; j < out1_col; j++){
			for (int ii = 0; ii < weight_row; ii++){
				for (int jj = 0;  jj < weight_col;jj++){
					in[i*out1_col+j][ii*weight_col+jj]=activation[(i+ii)*activation_length+j+jj];
				}
			}
			activation_t val = in[i*out1_col+j];
			//string inHex = val.to_string(16);
			cout << "Packed Input Window (row,col)-" << "("<< i << ","<< j << "): " << hex << val; //Hexadecimal
			//Do not combine previous and next line or the hexadecimal output will be reversed
			cout << "	Reversed Binary: " << val.reverse().to_string(2) << endl;//Binary Output
		}
	}

	cout << endl << "--------------------------------------------------------------" << endl;
	cout << "Weight1 Read:" << endl << endl;
	//Read in the two dimension weights and we expand it to a one dimension vector
	//int read_in_weight(int length, int width, char* file_name, int* w)

	read_in_weight(weight_length, weight_width, input_dir+l1weight_filename, weight1);
	cout << "Weight2 Read:" << endl;

	read_in_weight(weight_length, weight_width, input_dir+l2weight_filename, weight2);
	cout << "Weight3 Read:" << endl;
	read_in_weight(weight_length, weight_width, input_dir+l3weight_filename, weight3);
	cout << "FC Weight Read:" << endl;
	read_in_weight(fc_weight_col, fc_weight_row,input_dir+fcweight_filename, weightFC);
	cout << endl << "--------------------------------------------------------------" << endl;

	weight_t weight_layer1;
	weight_t weight_layer2;
	weight_t weight_layer3;
	wt_fc *weight_FC = new wt_fc[out3_row*fc_weight_col];

	for(int i = 0; i < weight_row; i++){
		for (int j=0;j< weight_col;j++){
			weight_layer1[i*weight_col+j] = weight1[i*weight_col+j];
			weight_layer2[i*weight_col+j] = weight2[i*weight_col+j];
			weight_layer3[i*weight_col+j] = weight3[i*weight_col+j];
		}
	}
	cout << endl << "--------------------------------------------------------------" << endl;
	cout << endl << "Flattened Layer 1 Weight: " << weight_layer1.to_string(2) << endl;
	cout << "Flattened Layer 2 Weight: " << weight_layer2.to_string(2) << endl;
	cout << "Flattened Layer 3 Weight: " << weight_layer3.to_string(2) << endl;


	 //Uncomment when FC weight file available
	cout << "Flattened Fully-Connected Weight:" << endl;
	for(int i = 0; i < fc_weight_col; i++){
		for(int j = 0; j < out3_row; j++){
			for (int ii = 0;ii < out3_col;ii++){
				weight_FC[i*out3_col+j][ii]=weightFC[i*fc_weight_row+j*out3_row+ii];
			}
			cout << weight_FC[i*out3_col+j].to_string(2);

		}
		cout << "	" << endl;
	}

	value_t mean[3] = {0, 0, 0};
	value_t var[3] ={1, 1, 1};
	value_t threshold[3]={0, 0, 0};
	fc_result batch_fc[3] = {0,1,0};

	cout << "start BNN3 " << endl << endl;
    BNN_3(in, weight_layer1,weight_layer2,weight_layer3, weight_FC,output,mean,var,threshold, batch_fc);
    cout << "DONE BNN3 " << endl;

	cout << endl << "--------------------------------------------------------------" << endl;
	cout << "Final Result:" << endl << endl;

	//Print output
	cout << "Output Value:	" << output << endl;
	for(int i = 0; i < fc_weight_col; i++){
		cout << "Output Bit " << i << ": " << output[i] << endl;
	}
    return 0;
}


int read_in_activation(int length, int width, string file_name, int* a){
    ifstream file(file_name);
    if (file.is_open()){
            for(int i = 0; i < width; i++){
                for(int j = 0; j < length; j++){
                char tep;
                // locate the position of activation. 0,1 takes 1 position; ','takes 1 position; '\n' takes 2 positions
                file.seekg(((i*length + j)*2 ),ios::beg) >> tep;
                //cout << "activation " << "(" << i << "," << j << ")" << tep <<endl;
                cout << tep;
                a[i*length + j] = tep - '0';

                }
                cout<<" " <<endl;
            }
    }
    file.close();
    return 1;
}

int read_in_weight(int length, int width, string file_name, int* w){
    ifstream file(file_name);
    if (file.is_open()){
            for(int i = 0; i < width; i++){
                for(int j = 0; j < length; j++){
                char tep;
                // locate the position of activation. 0,1 takes 1 position; ','takes 1 position; '\n' takes 2 positions
                file.seekg(((i*length + j)*2 ),ios::beg) >> tep;
                //cout << "weight " << "(" << i << "," << j << ")" << tep <<endl;
                cout << tep;
                w[i*length + j] = tep - '0';
                }
                cout<<" " <<endl;
            }

    }
    file.close();
    return 1;
}
