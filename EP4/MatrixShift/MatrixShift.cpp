#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <CL/cl.h>
#include <vector>

// Dimensões.
#define N 64

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;

class Matrix {
public:
	Matrix(int m, int n);
	Matrix(float* data, int m, int n) : data_(data), m_(m), n_(n) {}
	~Matrix() { 
		delete data_;
	}

	float* data() { return data_; }
	int m() { return m_; }
	int n() { return n_; }

	void print() {
		int i, j;
		for (i = 0; i < m_; i++) {
			for (j = 0; j < n_; j++) {
				printf("%f ", data_[i*n_ + j]);
			}
			printf("\n");
	    }
	}

	size_t size() { return m_ * n_ * sizeof(float); }
	void SaveToFile(const char* filename, const char* suffix);

	static Matrix* LoadFromFile(const char* filename);

private:
	float* data_;
	int m_;
	int n_;
};

Matrix::Matrix(int m, int n) : m_(m), n_(n) {
	data_ = (float*) malloc( m * n * sizeof(float) );
}

Matrix* Matrix::LoadFromFile(const char* filename) {
	ifstream ifs ( filename , ifstream::in );
	vector<float> raw_data;
    int m=0, n=0;
	bool gotN = false;
	string line;

	while (ifs.good()) {
        std::getline(ifs, line);
        if (line.size() <= 0)   break;
        std::istringstream linestream (line);

        m++;
        while(linestream.good()) {
			float value;
			linestream >> value;
			raw_data.push_back(value);

			if (!gotN)
				n++;
        }
		gotN = true;
	}
	float* data = (float*) malloc( m * n * sizeof(float) );
	int i;
	for (i = 0; i < m*n; i++) {
        data[i] = raw_data[i];
		/*for (j = 0; j < n; j++) {
			data[i][j] = raw_data[ i*n + j ];
		}*/
	}

	return new Matrix(data, m, n);
}

void Matrix::SaveToFile(const char* filename, const char* suffix) {
    string filePath = string(filename) + string(suffix);
    ofstream ofs (filePath.c_str());

    for (int i=0; i < m_; i++) {
        for (int j=0; j < n_; j++) {
            ofs << data_[i*n_ + j] << " ";
        }
        ofs << endl;
    }
    
    ofs.close();
}

/******/

int main(int argc, char* argv[]) {
	if (argc < 4) {
		printf("Wrong program call. Run:\n\t%s <input_matrix_file> <row_shift_amount> <column_shift_amount>\n", argv[0]);
		return 0;
	}

    // Cria os ponteiros que serão utilizados pelos arrays na memoria principal.
	Matrix* M = Matrix::LoadFromFile(argv[1]);
	Matrix* R = new Matrix(M->m(), M->n());
	int rowShift = atoi(argv[2]);
	int colShift = atoi(argv[3]);
	int matrixNumRows = M->m();
	int matrixNumCols = M->n();

    printf("InputShape=(%d, %d); RowShift(%d); ColumnShift(%d)\n", matrixNumRows, matrixNumCols, rowShift, colShift);
	cout << "Input Matrix:" << endl;
	M->print();

    // Cria os ponteiros que serão utilizados na memoria da GPU.
    cl_mem dM, dR, sRow, sCol, numRows, numCols; 
    
    // Armazena os valores de retorno.
    int returnValue;
    
    int maxSize = 0x100000; // Max size of kernel's source code. 0x100000 corresponds to 1MB

    printf("Inicializando OpenCL...\n"); 
    // Criação do ambiente OpenCL.
    // Plataforma que será utilizada.
    cl_platform_id platform = NULL;
    // Armazena quantas plataformas existem.
    cl_uint platformsNumber;
    // Busca pela primeira plataforma.
    // cl_int clGetPlatformIDs(cl_uint num_entries, cl_platform_id *platforms, cl_uint *num_platforms)
    returnValue = clGetPlatformIDs(1, &platform, &platformsNumber);
    if(returnValue != CL_SUCCESS) {
       printf("Erro: Falha ao detectar a Platforma.");
    }
 
    // Dispositivo.
    // Armazena o id do dispositivo que será usado.
    cl_device_id device;
    // Armazena quantos dispositivos existem.
    cl_uint devicesNumber;
    // Recupera o dispositivo.
    // cl_int clGetDeviceIDs( 	cl_platform_id platform, cl_device_type device_type, cl_uint num_entries, cl_device_id *devices, cl_uint *num_devices)
    // Se tem gpu ATI coloque gpu = 1.
    int gpu = 0;
    returnValue = clGetDeviceIDs(platform, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device, &devicesNumber);
    
    if (returnValue != CL_SUCCESS){
        printf("Erro: Falha ao recuperar o dispositivo.\n");
        return EXIT_FAILURE;
    }
    
    cl_context context = clCreateContext(0, 1, &device, NULL, NULL, &returnValue);
    if (!context) {
        printf("Erro: Falha ao criar o contexto.\n");
        return EXIT_FAILURE;
    }
    
    // Inicializa uma fila de comandos.
    cl_command_queue commandQueue = clCreateCommandQueue(context, device, 0, &returnValue);
    
    if (!commandQueue){
        printf("Erro: Falha ao criar a fila de comandos.\n");
        return EXIT_FAILURE;
    }
    
    // Alocação das estruturas na memória do dispositivo.
    dM = clCreateBuffer(context, CL_MEM_READ_ONLY, M->size(), NULL, &returnValue);
    dR = clCreateBuffer(context, CL_MEM_WRITE_ONLY, R->size(), NULL, &returnValue);
    sRow = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &returnValue);
    sCol = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &returnValue);
    numRows = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &returnValue);
    numCols = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &returnValue);
 
    // Faz a cópia dos arrays h_A e h_B para d_A e d_B.
    returnValue = clEnqueueWriteBuffer(commandQueue, dM, CL_TRUE, 0, M->size(), M->data(), 0, NULL, NULL);
    returnValue = clEnqueueWriteBuffer(commandQueue, sRow, CL_TRUE, 0, sizeof(int), &rowShift, 0, NULL, NULL);
    returnValue = clEnqueueWriteBuffer(commandQueue, sCol, CL_TRUE, 0, sizeof(int), &colShift, 0, NULL, NULL);
    returnValue = clEnqueueWriteBuffer(commandQueue, numRows, CL_TRUE, 0, sizeof(int), &matrixNumRows, 0, NULL, NULL);
    returnValue = clEnqueueWriteBuffer(commandQueue, numCols, CL_TRUE, 0, sizeof(int), &matrixNumCols, 0, NULL, NULL);

 
    // Abre o fonte do kernel.
    FILE *kernelFile;
    char *kernelStr;
    size_t kernelSize;
 
    kernelFile = fopen("MatrixShift_Kernel.cl", "r");
 
    kernelStr = (char*)malloc(maxSize);
    kernelSize = fread( kernelStr, 1, maxSize, kernelFile);
    fclose( kernelFile );
 
    // Cria e compila o fonte do programa.
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernelStr, (const size_t *)&kernelSize, &returnValue);
    
    if (!program){
        printf("Erro: Falha ao criar o programa.\n");
        return EXIT_FAILURE;
    }
    
    returnValue = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (returnValue != CL_SUCCESS){
        size_t len;
        char buffer[2048];
        printf("Erro: Falha ao criar o executável do programa.\n");
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        return EXIT_FAILURE;
    }
 
    // Compila o kernel.
    cl_kernel matShiftKernel = clCreateKernel(program, "MatrixShift", &returnValue);
    
    if (!matShiftKernel || returnValue != CL_SUCCESS) {
        printf("Erro: Falha ao criar o kernel.\n");
        return EXIT_FAILURE;
    }
 
 
    // Define os parâmetros do kernel.
    returnValue = 0;
    returnValue = clSetKernelArg(matShiftKernel, 0, sizeof(cl_mem), (void *)&dM);
    returnValue |= clSetKernelArg(matShiftKernel, 1, sizeof(cl_mem), (void *)&dR);
    returnValue |= clSetKernelArg(matShiftKernel, 2, sizeof(cl_mem), (void *)&sRow);
    returnValue |= clSetKernelArg(matShiftKernel, 3, sizeof(cl_mem), (void *)&sCol);
	returnValue |= clSetKernelArg(matShiftKernel, 4, sizeof(cl_mem), (void *)&numRows);
	returnValue |= clSetKernelArg(matShiftKernel, 5, sizeof(cl_mem), (void *)&numCols);
    
    if (returnValue != CL_SUCCESS){
        printf("Erro: Falha ao definir os parâmetros do kernel: %d\n", returnValue);
        exit(1);
    }
 
    // Lança a execução do kernel.
	size_t work_size[2] = { M->m(), M->n() };
    returnValue = clEnqueueNDRangeKernel(commandQueue, matShiftKernel, 2, NULL, work_size, NULL, 0, NULL, NULL);
    
    if(returnValue){
        printf("Erro: Falha na execução do kernel.\n");
        return EXIT_FAILURE;
    }
 
    // Copia o resultado d_C para h_C.
    returnValue = clEnqueueReadBuffer(commandQueue, dR, CL_TRUE, 0, R->size(), R->data(), 0, NULL, NULL);
    
    if(returnValue){
        printf("Erro: Falha na cópia do resultado.\n");
        return EXIT_FAILURE;
    }
 
    // Finaliza a fila de comandos.
    returnValue = clFlush(commandQueue);
    returnValue = clFinish(commandQueue);    
 
    printf("Output Matrix:\n");
	R->print();
	R->SaveToFile(argv[1], "_result");
 
    // Libera a memória do host.
    delete M;
	delete R;
 
    printf("Finalizando OpenCL e liberando recursos.\n");
    // Libera os recursos do dispositivo.
    returnValue = clFlush(commandQueue);
    returnValue = clFinish(commandQueue);
    returnValue = clReleaseKernel(matShiftKernel);
    returnValue = clReleaseProgram(program);
    returnValue = clReleaseMemObject(dM);
    returnValue = clReleaseMemObject(dR);
    returnValue = clReleaseMemObject(sRow);
    returnValue = clReleaseMemObject(sCol);
    returnValue = clReleaseMemObject(numRows);
    returnValue = clReleaseMemObject(numCols);
    returnValue = clReleaseCommandQueue(commandQueue);
    returnValue = clReleaseContext(context);
 
    return 0;
}
