#include "../include/main.h"

int isWellFormatted(const char *array){
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

void loadError(const char *array, double &error){
	if(!isDouble(array)){
		std::cerr << USAGE_LOG << "error should be a positive double." << std::endl;
		std::cerr << FATAL_ERROR_LOG << "exiting program." << std::endl;
	}
	error = std::atof(array);
}

void loadMaxn(const char *array, int &maxn){
	if(!isWellFormatted(array)){
		std::cerr << USAGE_LOG << "MAXN should be an integer >=-1." << std::endl;
		std::cerr << FATAL_ERROR_LOG << "exiting program." << std::endl;
	}
	maxn = std::atoi(array);
}

void loadMaxr(const char *array, int &maxr){
	if(!isWellFormatted(array)){
		std::cerr << USAGE_LOG << "MAXR should be an integer >=-1." << std::endl;
		std::cerr << FATAL_ERROR_LOG << "exiting program." << std::endl;
	}
	maxr = std::atoi(array);
}

int main(int argc, char *argv[]) {
	int maxn, maxr;
	double error;
	error = maxn = maxr = -1;
	if(argc < 2){
		std::cerr << USAGE_LOG << argv[0] << "<shared library name(*.so)>" << std::endl;
		std::cerr << FATAL_ERROR_LOG << "argument is mandatory, exiting program." << std::endl;
		return 1;
	}
	if(argc == 2){
		std::cerr << CONSOLE_LOG << "using default parameters: error: " <<  DEFAULT_ERROR
					<< "MAXN=" << DEFAULT_MAXN << ", MAXR=" << DEFAULT_MAXR << std::endl;
	}else if(argc == 3){
		loadError(argv[2],error);
		std::cerr << CONSOLE_LOG << "using default parameters: MAXN=" << DEFAULT_MAXN
			<< ", MAXR=" << DEFAULT_MAXR << std::endl;
	}else if(argc == 4){
		loadError(argv[2],error);
		loadMaxn(argv[3],maxn);
		std::cerr << CONSOLE_LOG << "using default parameters: MAXR=" << DEFAULT_MAXR << std::endl;
	}else{
		loadError(argv[2],error);
		loadMaxn(argv[3],maxn);
		loadMaxr(argv[4],maxr);
	}


	DynamicFunction dfunction(argv[1]);
	if(!dfunction.isLibraryLoaded()){
		std::cerr << ERROR_LOG << "failed to load shared library." << std::endl;
		std::cerr << FATAL_ERROR_LOG << "exiting program." << std::endl;
	}

	Integral3D integral;

	double r,integralError;
	r = integral(dfunction,integralError,error,maxn,maxr);
	std::cout << "Result: " << r << " \u00B1 " << integralError << std::endl;
	return 0;
}