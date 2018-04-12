__kernel void histogram(__global const unsigned int* image, __global unsigned int* result, const unsigned int data_size, const unsigned int size)
{
	int idx = get_global_id(0);
	unsigned int index;
	
	if(idx<256*3){
		result[idx] = 0;
	}

	for(int i=idx; i<data_size; i+=size){
		int color = i % 3;
		if(color==0){
			index = image[i];
			atomic_inc(result+index);
		}
		else if(color==1){
			index = image[i]+256;
			atomic_inc(result+index);
		}
		else{
			index = image[i]+512;
			atomic_inc(result+index);
		}
	}
}