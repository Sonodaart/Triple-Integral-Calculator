#include "../include/main.h"

// function that check if the given input is an integer
int isInteger(const char *array){
	int i;
	if(array[0]=='\0'){
		return 0;
	}
	if(array[0]=='-' and array[1]=='1' and array[2]=='\0'){
		return 1;
	}
	for(i=0;array[i]!='\0';++i){
		if(array[i]<'0' or array[i]>'9'){
			return 0;
		}
	}
	return 1;
}

// function that checks if given input is a double
int isDouble(const char *array){
	int dot = 0;
	int i;
	if(array[0]=='\0'){
		return 0;
	}
	for(i=0;array[i]!='\0';++i){
		if(array[i]=='.' and dot==0){
			dot = 1;
		}else if(array[i]<'0' or array[i]>'9'){
			return 0;
		}
	}
	return 1;
}

// function to load a double
void loadDouble(const char *array, double &value, const std::string &variableName){
	if(!isDouble(array)){
		std::cerr << USAGE_LOG << variableName << " error should be a positive double." << std::endl;
		std::cerr << FATAL_ERROR_LOG << "exiting program." << std::endl;
	}
	value = std::atof(array);
}

// function to load integer into maxn value
void loadInteger(const char *array, int &value, const std::string &variableName){
	if(!isInteger(array)){
		std::cerr << USAGE_LOG << variableName << "should be an integer >=-1." << std::endl;
		std::cerr << FATAL_ERROR_LOG << "exiting program." << std::endl;
	}
	value = std::atoi(array);
}

int main(int argc, char *argv[]) {
	int maxn, maxr;
	double error;
	// setting parameters to default value
	error = maxn = maxr = -1;
	if(argc < 2){
		std::cerr << USAGE_LOG << argv[0] << "<shared library name(*.so)>" << std::endl;
		std::cerr << FATAL_ERROR_LOG << "argument is mandatory, exiting program." << std::endl;
		return 1;
	}
	if(argc == 2){
		std::cerr << CONSOLE_LOG << "using default parameters: error: " <<  DEFAULT_ERROR
					<< ", MAXN=" << DEFAULT_MAXN << ", MAXR=" << DEFAULT_MAXR << std::endl;
	}else if(argc == 3){
		loadDouble(argv[2],error,"error");
		std::cerr << CONSOLE_LOG << "using default parameters: MAXN=" << DEFAULT_MAXN
			<< ", MAXR=" << DEFAULT_MAXR << std::endl;
	}else if(argc == 4){
		loadDouble(argv[2],error,"error");
		loadInteger(argv[3],maxn,"MAXN");
		std::cerr << CONSOLE_LOG << "using default parameters: MAXR=" << DEFAULT_MAXR << std::endl;
	}else{
		loadDouble(argv[2],error,"error");
		loadInteger(argv[3],maxn,"MAXN");
		loadInteger(argv[4],maxr,"MAXR");
	}

	// link dynamic library's function into DynamicFunction object
	DynamicFunction dfunction(argv[1]);
	// checking for eventual loading errors
	if(!dfunction.isLibraryLoaded()){
		std::cerr << ERROR_LOG << "failed to load shared library." << std::endl;
		std::cerr << FATAL_ERROR_LOG << "exiting program." << std::endl;
	}

	Integral3D integral;
	double r,integralError;

	// calculaing and displaying integral
	r = integral(dfunction,integralError,error,maxn,maxr);
	std::cout << "Result: " << r << " \u00B1 " << integralError << std::endl;
	return 0;
}