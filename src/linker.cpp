#include "../include/linker.h"


//===================== DynamicFunction Class =====================//
// constructor that only initialise names, but doesn't link to a shared library
DynamicFunction::DynamicFunction() : functionName(DEFAULT_FUNCTION_NAME),
										inequality1Name(DEFAULT_INEQUALITY1_NAME), 
										inequality2Name(DEFAULT_INEQUALITY2_NAME), handle(nullptr){
}

// constructor that initialise names and link to a shared library(.so) given,
// and also(if requested, and by default it is) loads the Function3D from the library
DynamicFunction::DynamicFunction(char fileName[], const int &loadFunctionFlag) : libraryName(fileName),
								functionName(DEFAULT_FUNCTION_NAME), inequality1Name(DEFAULT_INEQUALITY1_NAME),
								inequality2Name(DEFAULT_INEQUALITY2_NAME), handle(nullptr){
	loadLibrary(fileName);
	if(loadFunctionFlag){
		loadLinkedFunction();
	}
}

// constructor that initialise names and link to a shared library(.so) given,
// and also(if requested, and by default it is) loads the Function3D from the library
DynamicFunction::DynamicFunction(const std::string &fileName, const int &loadFunctionFlag)
								: libraryName(fileName), functionName(DEFAULT_FUNCTION_NAME), 
									inequality1Name(DEFAULT_INEQUALITY1_NAME),
									inequality2Name(DEFAULT_INEQUALITY2_NAME), handle(nullptr){
	loadLibrary(fileName);
	if(loadFunctionFlag){
		loadLinkedFunction();
	}
}

// destructor that deallocate the loaded library
DynamicFunction::~DynamicFunction(){
	if(closeLibrary()){
		std::cerr << WARNING_LOG << " failed closing the shared library." << std::endl;
	}
}

// function to deallocate currently open(if exists) linked shared library
// to be noted that this makes unusable the Function3D previously loaded
// to be noted that the function returns the state of the operation(0,1)
int DynamicFunction::closeLibrary(){
	if(!isLibraryLoaded()){
		// if it's non open no need to close it
		return 0;
	}
	if (dlclose(handle) != 0){
		std::cerr << ERROR_LOG << " an unexpected error has occurred while closing the handle: "
					<< dlerror() << std::endl;
		handle = nullptr;
		setState(0);
		return 1;
	}
	handle = nullptr;
	setState(0);
	return 0;
}

// function to load a shared library(.so) given the name of the file
// to be noted that the function returns the state of the operation(0: okay, 1: error)
int DynamicFunction::loadLibrary(char fileName[]){
	if(isLibraryLoaded()){
		if(closeLibrary()){
			std::cerr << WARNING_LOG << " failed closing the last library while opening new library."
						<< std::endl;
		}
	}
	libraryName = fileName;
	// RTLD_LAZY means that unresolved symbols are not resolved until used
	handle = dlopen(libraryName.c_str(), RTLD_LAZY);
	if (!isLibraryLoaded()){
		std::cerr << ERROR_LOG << " cannot open shared library: " << dlerror() << std::endl;
		handle = nullptr;
		return 1;
	}
	return 0;
}

// function to load a shared library(.so) given the name of the file
// to be noted that the function returns the state of the operation(0,1)
int DynamicFunction::loadLibrary(const std::string &fileName){
	return loadLibrary(libraryName.c_str());
}

// function to check wether DynamicFunction is currently linked to
// a shared library
int DynamicFunction::isLibraryLoaded(){
	return handle!=nullptr;
}

// function to load the "double f(double,double,double)" and 2 maps, in
// order to create the Function3D object
int DynamicFunction::loadLinkedFunction(){
	if (!isLibraryLoaded()){
		std::cerr << ERROR_LOG <<  " shared library is missing, or not properly initialised." << std::endl;
		return 1;
	}
	// Load the symbol for the function and 2 maps
	doubleFunction3D f = (doubleFunction3D) dlsym(handle, functionName.c_str());
	std::map<std::string,double> *temp1 = (std::map<std::string,double>*) dlsym(handle, inequality1Name.c_str());
	std::map<std::string,double> *temp2 = (std::map<std::string,double>*) dlsym(handle, inequality2Name.c_str());
	// check wether symbols have been properly resolved
	if (!f){
		std::cerr << ERROR_LOG << " cannot load symbol " << functionName << ": " << dlerror() << std::endl;
		return 1;
	}
	if(!temp1){
		std::cerr << ERROR_LOG << " cannot load symbol " << inequality1Name << ": " << dlerror() << std::endl;
		return 1;
	}
	if(!temp2){
		std::cerr << ERROR_LOG << " cannot load symbol " << inequality2Name << ": " << dlerror() << std::endl;
		return 1;
	}
	loadFunction3D(*temp1,*temp2,f);
	return 0;
}

// function to get the name of the function being looked for in the shared library
std::string DynamicFunction::getFunctionName() const{
	return functionName;
}

// function to get the name of the first inequality being looked for in the shared library
std::string DynamicFunction::getInequality1Name() const{
	return inequality1Name;
}

// function to get the name of the second inequality being looked for in the shared library
std::string DynamicFunction::getInequality2Name() const{
	return inequality2Name;
}

// function to change the name of the function being looked for in the shared library
void DynamicFunction::setFunctionName(char name[]){
	functionName = name;
}

// function to change the name of the function being looked for in the shared library
void DynamicFunction::setFunctionName(const std::string &name){
	functionName = name;
}

// function to change the name of the first inequality being looked for in the shared library
void DynamicFunction::setInequality1Name(char name[]){
	inequality1Name = name;
}

// function to change the name of the first inequality being looked for in the shared library
void DynamicFunction::setInequality1Name(const std::string &name){
	inequality1Name = name;
}

// function to change the name of the second inequality being looked for in the shared library
void DynamicFunction::setInequality2Name(char name[]){
	inequality2Name = name;
}

// function to change the name of the second inequality being looked for in the shared library
void DynamicFunction::setInequality2Name(const std::string &name){
	inequality2Name = name;
}