#include "../include/main.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << USAGE_LOG << argv[0] << " <shared library name(*.so)>" << std::endl;
		std::cerr << FATAL_ERROR_LOG << " argument is mandatory, exiting program." << std::endl;
		return 1;
	}

	DynamicFunction dfunction(argv[1]);
	if(!dfunction.isLibraryLoaded()){
		std::cerr << ERROR_LOG << " failed to load shared library." << std::endl;
		std::cerr << FATAL_ERROR_LOG << " exiting program." << std::endl;
	}

	Integral3D integral;

	double error;
	double r = integral(dfunction,1,error);
	std::cout << "Result: " << r << " \u00B1 " << error << std::endl;
	// dloader.loadLinkedFunction();
	// int result = dfunction(1,2,3);
	return 0;
}