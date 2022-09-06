Goal:
  To implement Binay Neural Network (BNN) in C++. Then use Vivado HLS get readble RTL code. Use built-in optimization tool in Vivado to optomize design for better timing and performance.
  
  
Steps:
  Input -> Layer_1 (convolution) -> Bathch Normalization -> Layer 2 -> Bathch Normalization -> Layer 3 -> Bathch Normalization -> Fully Connected Layer 
  
Optimization Techniques:
  Loop-unrolling (more parallelism)
  Pipelineing  (more reousece/power intensive but improves timing)
  
![Screen Shot 2022-09-06 at 2 29 28 PM](https://user-images.githubusercontent.com/42010432/188743981-318342a6-3678-40ac-880d-8f8065e4dd98.png)
