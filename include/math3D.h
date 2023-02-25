// Library to define operations of 3D objects such as: inequalities, functions(R^3->R)

#ifndef _MATH3D_LIB
#define _MATH3D_LIB

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <limits>
#include <cmath>

#include "../include/error.h"

// data type of a function that maps R^3 into R
typedef double (*doubleFunction3D)(double, double, double);

// Inequality is an object that describes inequalities of the
// type Ax^2+ax+By^2+by+Cz^2+Cz+r >(=<) 0, where the symbol
// >,<,>=,<= is also specifiable(> is the default).
// To be noted that the map should contain the symbols:
// "x^2","x","y^2","y","z^2","z","r" with their respective
// coefficients. If omitted the coefficient is set to 0.
// In addition it should be present a key to specify the
// inequality type between: ">",">=","<","<=", only one
// is admitted, if more are present the first in this
// list will be the one selected. If none are present the
// default is ">".
// To be noted that the presence of the inequality symbol
// is only present in the map to create the inequality(and
// thus in the shared library map too), but in the Inequality
// object it'ss then moved into the "disequality" variable.
class Inequality{
	public:
		// constructors
		Inequality();
		Inequality(const std::map<std::string,double>&);
		Inequality(const Inequality&);
		// destructor
		~Inequality();

		// functions to load inequalities
		void loadInequality(const std::map<std::string,double>&);
		void loadInequality(const Inequality&);

		// functions to change inequality form
		void standardize();
		
		// functions regarding the execution of the Inequality,
		// which returns the state of the inequality(0,1)
		int isCallable() const;
		int operator()(const double&, const double&, const double&) const;
		
		// function to get coefficient
		const std::map<std::string,double>& getCoefficient() const;
		const double& getCoefficient(const std::string&) const;
		const double& operator[](const std::string&) const;
		const std::string& getDisequality() const;

	private:
		int isLoaded; // flag that indicates if Inequality is properly loaded
		std::map<std::string,double> coefficient; // map of coefficient as described above
		std::string disequality; // inequality symbol (">",">=","<","<=")
};

// Function3D is an object that describes a 3D function(R^3->R). In
// particular the function is evaluated only in a domain, given by
// the intersection of two inequalities. Elsewhere it's defined as 0.
// To be noted that with "domain" it will be referred the area over
// which the function is effectively evaluated, and it can be thought
// as if the function itself is multiplied by a characteristic function
// of value 1 in the "domain" and 0 elsewhere. In this way the function
// can be thought as extended, to have actual domain over R^3, making
// evaluations outside the domain mathematically allowed.
class Function3D{
	public:
		// constructors
		Function3D();
		Function3D(const Function3D &function);
		Function3D(const Inequality&, const Inequality&, const doubleFunction3D&);
		Function3D(const std::map<std::string,double>&,
					const std::map<std::string,double>&, const doubleFunction3D&);	
		// destructor
		~Function3D();
		
		// function to load the Function3D
		void loadFunction3D(const Inequality&, const Inequality&, const doubleFunction3D&);
		void loadFunction3D(const std::map<std::string,double>&,
							const std::map<std::string,double>&, const doubleFunction3D&);
		
		// functions regarding the execution of the Function3D
		int isCallable() const;
		int isInDomain(const double &x, const double &y, const double &z) const;
		double operator()(const double&, const double&, const double&) const;

		// function to set the state of the Function3D
		void setState(const int&);
		// function to access the inequalities
		const Inequality& getInequality(const int&) const;

	protected:
		int isLoaded; // flag that indicates if Function3D is properly loaded
		Inequality first; // first inequality
		Inequality second; // second inequality
		doubleFunction3D function; // function(R^3->R)
};

// Point3D is an object that describes a point in 3 dimensions.
class Point3D{
	public:
		// constructors
		Point3D();
		Point3D(const double&, const double&, const double&);
		Point3D(const Point3D&);
		
		// destructor
		~Point3D();

		// coordinates (public access)
		double x;
		double y;
		double z;
};

// Parallelepiped is an object that describes parallelepiped objects in 3D.
// The vertex is the "leftmost" point(aka the point with minimum x,y and z).
// The xwidth,ywidth and zwidth are the width of each side, in the respective
// coordinate directions.
class Parallelepiped{
	public:
		// constructors
		Parallelepiped();
		Parallelepiped(const Point3D&, const double&, const double&, const double&);
		Parallelepiped(const Parallelepiped&);
		
		// destructor
		~Parallelepiped();

		// coordinates
		Point3D vertex;
		double xwidth;
		double ywidth;
		double zwidth;
};

// Integral3D is an object that performs integrals. It takes in input a Function3D,
// which posseses 2 inequalities. The class calculate the smaller rectangular domain that
// contains such domain, and on that it performs the integral.
// The class is capable of managing also ubounded domains. The technique on which it integrate
// is the Romberg's algorithm, with adaptive integration.
class Integral3D{
	public:
		// constructor
		Integral3D();
		
		// destructor
		~Integral3D();

		// evaluate integral of Function3D passed
		double operator()(const Function3D&, const double&, double&, const int& = 3, const int& = 3);

	private:
		// private functions to perform math operations

		// functions related to the evaluation of the integral
		double improperRombergIntegral(const Function3D&, const Parallelepiped&, const double&,
										double&, const int&, const int&, const int& = 0, const int& = 0,
										const int& = 0, const int& = 0);
		double evaluateImproperRombergIntegral(const Function3D&, const Parallelepiped&, const double&,
												double&, const int&, const int&, const int&, const int&, const int&);
		double rombergIntegral(const Function3D&, const Parallelepiped&, const double&, double&, const int&,
								const int&, const int& = 0) const;
		double directionedTrapezoidIntegral(const Function3D&, const Parallelepiped&, const int&,
												const int& = 0, const double& = 0, const double& = 0) const;

		// functions related to the change of variable
		double standardChangeOfVariableFunction(const double&) const;
		double standardInverseChangeOfVariableFunction(const double&) const;
		double changeVariable(const double&, const double&) const;
		void setChangeOfVariable(const double& = 0, const double& = 0, const double& = 0);
		double changeOfVariableDifferential(const double&, const double&, const double&) const;

		// functions related to the domain management
		Parallelepiped rectanglifyDomain(const Function3D&) const;
		void applyInequality(const Inequality&, double&, double&, double&, double&, double&, double&) const;
		void getRange(const Inequality&, const std::string&, double&, double&) const;
		void changeDomainOfIntegration(Parallelepiped&, const double&, const double&, const double&) const;
		void splitDomain(const Parallelepiped&, std::vector<Parallelepiped>&) const;


		double isXSubstituted;
		double isYSubstituted;
		double isZSubstituted;
};

#endif // end of library guardian