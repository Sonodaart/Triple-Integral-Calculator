// Library for dynamically linking shared libraries,
// and managing the Function3D expected to be loaded
// from the library.

#ifndef _LINKER_LIB
#define _LINKER_LIB

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <dlfcn.h>

#include "../include/error.h"
#include "../include/math3D.h"

#define DEFAULT_FUNCTION_NAME "f"
#define DEFAULT_INEQUALITY1_NAME "first"
#define DEFAULT_INEQUALITY2_NAME "second"

// DynamicFunction is an object that is able to read
// from a shared library, and is specialised in loading
// 3 objects: a function "double f(double,double,double)"
// and 2 maps named first and second.
// The class is an extension of a Function3D, this means
// that it can be also used just as a regular Function3D.
class DynamicFunction : public Function3D{
	public:
		// constructors
		DynamicFunction();
		DynamicFunction(char[], const int& = 1);
		DynamicFunction(const std::string&, const int& = 1);
		// destructor
		~DynamicFunction();

		// functions to manage library linking
		int closeLibrary();
		int loadLibrary(char[]);
		int loadLibrary(const std::string&);
		int isLibraryLoaded();

		// functions to manage loading of Function3D
		int loadLinkedFunction();
		/* inherited functions are left commented */
		// void loadFunction3D(const Inequality&, const Inequality&, const doubleFunction3D&);
		// void loadFunction3D(const std::map<std::string,double>&,
		// 					const std::map<std::string,double>&, const doubleFunction3D&);

		// functions regarding evaluation of function (from Function3D)
		/* inherited functions are left commented */
		// void setState(const int&);
		// int isCallable() const;
		// int isInDomain(const double &x, const double &y, const double &z) const;
		// double operator()(const double&, const double&, const double&);

		// functions to get elements name
		std::string getFunctionName() const;
		std::string getInequality1Name() const;
		std::string getInequality2Name() const;

		// function to personalize elements name
		void setFunctionName(char[]);
		void setFunctionName(const std::string&);
		void setInequality1Name(char[]);
		void setInequality1Name(const std::string&);
		void setInequality2Name(char[]);
		void setInequality2Name(const std::string&);

	private:
		std::string libraryName; // current linked library name
		std::string functionName; // name of the function to be loaded
		std::string inequality1Name; // name of the first inequality to be loaded
		std::string inequality2Name; // name of the second inequality to be loaded
		void* handle; // handle of the shared library loaded
};


#endif // end of library guardian