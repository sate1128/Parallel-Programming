// OpenCL tutorial 1
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif


cl_program load_program(cl_context context, const char* filename)
{
	std::ifstream in(filename, std::ios_base::binary);
	if(!in.good()) {
		return 0;
	}

	// get file length
	in.seekg(0, std::ios_base::end);
	size_t length = in.tellg();
	in.seekg(0, std::ios_base::beg);

	// read program source
	std::vector<char> data(length + 1);
	in.read(&data[0], length);
	data[length] = 0;

	// create and build program 
	const char* source = &data[0];
	cl_program program = clCreateProgramWithSource(context, 1, &source, 0, 0);
	if(program == 0) {
		return 0;
	}

	if(clBuildProgram(program, 0, 0, 0, 0, 0) != CL_SUCCESS) {
		return 0;
	}

	return program;
}


int main()
{
	cl_int err;
	cl_uint num;
	err = clGetPlatformIDs(0, 0, &num);
	if(err != CL_SUCCESS) {
		std::cerr << "Unable to get platforms\n";
		return 0;
	}

	std::vector<cl_platform_id> platforms(num);
	err = clGetPlatformIDs(num, &platforms[0], &num);
	if(err != CL_SUCCESS) {
		std::cerr << "Unable to get platform ID\n";
		return 0;
	}

	cl_context_properties prop[] = { CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>(platforms[0]), 0 };
	cl_context context = clCreateContextFromType(prop, CL_DEVICE_TYPE_DEFAULT, NULL, NULL, NULL);
	if(context == 0) {
		std::cerr << "Can't create OpenCL context\n";
		return 0;
	}

	size_t cb;
	clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &cb);
	std::vector<cl_device_id> devices(cb / sizeof(cl_device_id));
	clGetContextInfo(context, CL_CONTEXT_DEVICES, cb, &devices[0], 0);

	cl_command_queue queue = clCreateCommandQueue(context, devices[0], 0, 0);
	if(queue == 0) {
		std::cerr << "Can't create command queue\n";
		clReleaseContext(context);
		return 0;
	}
	
	//read
	unsigned int i=0, a, input_size;
	std::fstream inFile("input", std::ios_base::in);
	std::ofstream outFile("0656109.out", std::ios_base::out);

	inFile >> input_size;
	unsigned int *image = new unsigned int[input_size];
	unsigned int *histogram_results = new unsigned int[256*3];
	memset (histogram_results, 0x0, 256 * 3 * sizeof(unsigned int));
	while( inFile >> a ) {
		image[i++] = a;
	}
	const unsigned int DATA_SIZE = input_size;
	
	size_t work_size = 1024;
	//clGetDeviceInfo(devices[0], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(work_size), work_size, 0);
	//std::cout << "Size: " << work_size << "\n";
	

	cl_mem cl_image = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(unsigned int) * DATA_SIZE, &image[0], NULL);
	cl_mem cl_res = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(unsigned int) * 256 * 3, NULL, NULL);
	if(cl_image == 0 || cl_res == 0) {
		std::cerr << "Can't create OpenCL buffer\n";
		clReleaseMemObject(cl_image);
		clReleaseMemObject(cl_res);
		clReleaseCommandQueue(queue);
		clReleaseContext(context);
		return 0;
	}

	cl_program program = load_program(context, "histogram.cl");
	if(program == 0) {
		std::cerr << "Can't load or build program\n";
		clReleaseMemObject(cl_image);
		clReleaseMemObject(cl_res);
		clReleaseCommandQueue(queue);
		clReleaseContext(context);
		return 0;
	}

	cl_kernel histogram = clCreateKernel(program, "histogram", 0);
	if(histogram == 0) {
		std::cerr << "Can't load kernel\n";
		clReleaseProgram(program);
		clReleaseMemObject(cl_image);
		clReleaseMemObject(cl_res);
		clReleaseCommandQueue(queue);
		clReleaseContext(context);
		return 0;
	}

	clSetKernelArg(histogram, 0, sizeof(cl_mem), &cl_image);
	clSetKernelArg(histogram, 1, sizeof(cl_mem), &cl_res);
	clSetKernelArg(histogram, 2, sizeof(unsigned int), &DATA_SIZE);
	clSetKernelArg(histogram, 3, sizeof(unsigned int), &work_size);

	err = clEnqueueNDRangeKernel(queue, histogram, 1, 0, &work_size, 0, 0, 0, 0);

	if(err == CL_SUCCESS) {
		err = clEnqueueReadBuffer(queue, cl_res, CL_TRUE, 0, sizeof(unsigned int) * 256 * 3, &histogram_results[0], 0, 0, 0);
	}

	clReleaseKernel(histogram);
	clReleaseProgram(program);
	clReleaseMemObject(cl_image);
	clReleaseMemObject(cl_res);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
	
	for(unsigned int i = 0; i < 256 * 3; ++i) {
		if (i % 256 == 0 && i != 0)
			outFile << std::endl;
		outFile << histogram_results[i]<< ' ';
	}

	inFile.close();
	outFile.close();

	return 0;
}
