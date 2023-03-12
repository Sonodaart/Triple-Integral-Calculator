#include "../include/math3D.h"

//===================== Inequality Class =====================//
// constructor that only initialise disequality and isLoaded flag to default values
Inequality::Inequality() : isLoaded(0), disequality(DEFAULT_INEQUALITY){
}

// constructor that loads coefficient given
Inequality::Inequality(const std::map<std::string,double> &_coefficient) : isLoaded(1), disequality(DEFAULT_INEQUALITY){
	loadInequality(_coefficient);
}

// constructor that loads inequality object coefficients given
Inequality::Inequality(const Inequality &inequality) : isLoaded(1), disequality(DEFAULT_INEQUALITY){
	loadInequality(inequality);
}

// empty destructor
Inequality::~Inequality(){
}

// function that loads a map of coefficient(also considering disequality symbols)
void Inequality::loadInequality(const std::map<std::string,double> &_coefficient){
	std::vector<std::string> names = {"x^2","x","y^2","y","z^2","z","r"};
	std::map<std::string,double>::const_iterator it;
	unsigned int i;
	// load each value, and if not is set to 0
	for(i=0;i<names.size();++i){
		it = _coefficient.find(names[i]);
		if(it == _coefficient.end()){
			// if not present is set to 0
			coefficient[names[i]] = 0;
		}else{
			coefficient[names[i]] = _coefficient.at(names[i]);
		}
	}

	// check if any of the disequalities are present, by
	// default the constructor already assigned it to be ">"
	std::vector<std::string> disequalities = {">",">=","<","<="};
	for(i=0;i<disequalities.size();++i){
		it = _coefficient.find(disequalities[i]);
		if(it != _coefficient.end()){
			disequality = disequalities[i];
			break;
		}
	}
	isLoaded = 1;
}

// function that loads a map of coefficient(also considering disequality symbols)
// from an Inequality object
void Inequality::loadInequality(const Inequality &inequality){
	loadInequality(inequality.getCoefficient());
	disequality = inequality.disequality;
	isLoaded = 1;
}

// functions that change the coefficients, in order to put the inequality
// in a standard form with either the > or the >=
void Inequality::standardize(){
	if(disequality[0]=='<'){
		if(disequality=="<="){
			disequality = ">=";
		}else if(disequality=="<"){
			disequality = ">";
		}
		// changing inequality type is done by multiplying
		// everything by -1
		std::map<std::string,double>::const_iterator it;
		for(auto &element : coefficient){
			element.second = -element.second;
		}
	}
}


// function that check if the Inequality is callable by looking at the state flag
int Inequality::isCallable() const{
	return isLoaded==1;
}

// operator() that execute the Inequality calculation, and return wether it's
// it's a true statement(1) or not(0).
int Inequality::operator()(const double &x, const double &y, const double &z) const{
	if(!isCallable()){
		std::cerr << WARNING_LOG << "trying to call non initialised inequality." << std::endl;
		return -1;
	}
	double value = coefficient.at("x^2")*x*x+coefficient.at("x")*x+
					coefficient.at("y^2")*y*y+coefficient.at("y")*y+
					coefficient.at("z^2")*z*z+coefficient.at("z")*z + coefficient.at("r");
	if(disequality == ">"){
		return value>0;
	}else if(disequality == ">="){
		return value>=0;
	}else if(disequality == "<"){
		return value<0; 
	}else if(disequality == "<="){
		return value<=0;
	}
	return -1;
}

// function that returns the map of coefficient of the Inequality object
const std::map<std::string,double>& Inequality::getCoefficient() const{
	return coefficient;
}

// function that returns the coefficient relative to a string, i.e. "x^2" return
// the coefficient of the x^2 term
const double& Inequality::getCoefficient(const std::string &name) const{
	if(coefficient.find(name) == coefficient.end()){
		std::cerr << WARNING_LOG << "trying to retrieve non-existing coefficient in Inequality." << std::endl;
	}
	return coefficient.at(name);
}

// function that returns the coefficient relative to a string, i.e. "x^2" return
// the coefficient of the x^2 term
const double& Inequality::operator[](const std::string &name) const{
	return getCoefficient(name);
}

// function that returns the type of inequality: ">",">=","<","<="
const std::string& Inequality::getDisequality() const{
	return disequality;
}


//===================== Function3D Class =====================//
// constructor that only initialise the state flag to 0
Function3D::Function3D() : isLoaded(0){
}

// constructor that create a deep copy of a given function
Function3D::Function3D(const Function3D &function) : isLoaded(1), first(function.first), second(function.second),
														function(function.function){
}


// constructor that loads a Function3D from 2 Inequalities and a function
Function3D::Function3D(const Inequality &_first, const Inequality &_second, 
						const doubleFunction3D &_function)
						: isLoaded(1), first(_first), second(_second), function(_function){
}

// constructor that loads a Function3D from 2 maps and a function
Function3D::Function3D(const std::map<std::string,double> &_first,
						const std::map<std::string,double> &_second,
						const doubleFunction3D &_function)
						: isLoaded(1), first(_first), second(_second), function(_function){
}

// destructor
Function3D::~Function3D(){
}

// function that takes 2 Inequalities and a function to create the Function3D
void Function3D::loadFunction3D(const Inequality &_first, const Inequality &_second, 
								const doubleFunction3D &_function){
	first.loadInequality(_first);
	second.loadInequality(_second);
	function = _function;
	isLoaded = 1;
}

// function that takes 2 maps and a function to create the Function3D
void Function3D::loadFunction3D(const std::map<std::string,double> &_first,
								const std::map<std::string,double> &_second, 
								const doubleFunction3D &_function){
	first.loadInequality(_first);
	second.loadInequality(_second);
	function = _function;
	isLoaded = 1;
}

// function that looks if Function3D is callable
int Function3D::isCallable() const{
	if(!first.isCallable() || !second.isCallable()){
		return 0;
	}
	return isLoaded==1;
}

// function that checks if a (x,y,z) point satisfy both inequalities, thus falling in the domain
int Function3D::isInDomain(const double &x, const double &y, const double &z) const{
	if(!first.isCallable() || !second.isCallable()){
		std::cerr << WARNING_LOG << "trying to call function non initialised domain." << std::endl;
		return 0;
	}
	return first(x,y,z) && second(x,y,z);
}

// operator() that evaluates the Function3D on a given (x,y,z) point
double Function3D::operator()(const double &x, const double &y, const double &z) const{
	if(!isCallable()){
		std::cerr << WARNING_LOG << "trying to call non initialised function." << std::endl;
		return std::numeric_limits<double>::quiet_NaN();
	}
	if(!isInDomain(x,y,z)){
		// the function can be extended to R^3, defining it to 0
		// in all non-domain points
		return 0;
	}
	// std::cout << ">"<<x<<" "<<y<<" "<<z<<" "<<function(x,y,z)<<std::endl;
	return function(x,y,z);
}

// function that set the state flag(if it's properly loaded) of Function3D
void Function3D::setState(const int &state){
	isLoaded = state;
}

const Inequality& Function3D::getInequality(const int &n) const{
	if(n==1){
		return first;
	}else if(n==2){
		return second;
	}else{
		std::cerr << WARNING_LOG << "asking for non-existent inequality. First is returned." << std::endl;
		return first;
	}
}



//===================== Point3D Class =====================//
// empty constructor
Point3D::Point3D() : x(0), y(0), z(0){
}

// constructor that creates a point of given coordinates
Point3D::Point3D(const double &_x, const double &_y, const double &_z) : x(_x), y(_y), z(_z){
}

// constructor that creates a deep copy of another point
Point3D::Point3D(const Point3D &p) : x(p.x), y(p.y), z(p.z){
}

// empty destructor
Point3D::~Point3D(){
}


//===================== Parallelepiped Class =====================//
// empty constructor, default parameters are set
Parallelepiped::Parallelepiped() : vertex(Point3D()), xwidth(0), ywidth(0), zwidth(0){
}

// constructor that takes in input a Point3D and 3 doubles representing respectively the vertex
// and the 3 side lengths
Parallelepiped::Parallelepiped(const Point3D &_vertex, const double &_xwidth, const double &_ywidth,
								const double &_zwidth) : vertex(_vertex), xwidth(_xwidth),
								ywidth(_ywidth), zwidth(_zwidth){
}

// constructor that creates a deep copy of another Parallelepiped
Parallelepiped::Parallelepiped(const Parallelepiped &p) : vertex(p.vertex), xwidth(p.xwidth),
								ywidth(p.ywidth), zwidth(p.zwidth){
}

// empty destructor
Parallelepiped::~Parallelepiped(){
}


//===================== Parallelepiped Class =====================//
// default constructor that set default values to variables
Integral3D::Integral3D() : approximationFlag(ERROR_INTEGRATION_FLAG_BASE_STATE){}

// empty destructor
Integral3D::~Integral3D(){}

// this function takes in input a function, a minimum tolerance "epsilon", a variable in which the final error
// is stored in, and the max number of Romberg's step and recursions(if those are -1 default value is used)
double Integral3D::operator()(const Function3D &function, double &finalError, double epsilon, int MAXN, int MAXR){
	if(epsilon==-1){
		epsilon = DEFAULT_ERROR;
	}
	if(MAXN==-1){
		MAXN = DEFAULT_MAXN;
	}
	if(MAXR==-1){
		MAXR = DEFAULT_MAXR;
	}
	Parallelepiped domain = rectanglifyDomain(function);
	std::cout << ">>>" << domain.vertex.x << " " << domain.vertex.y << " " << domain.vertex.z << std::endl;
	std::cout << ">>>" << domain.xwidth << " " << domain.ywidth << " " << domain.zwidth << std::endl;
	finalError = 0; // reset error
	// if domain has at least one coordinate that doesn't have width, it's
	// 2 dimensional, and 3D integrals on 2D surfaces are 0
	if(domain.xwidth==0 or domain.ywidth==0 or domain.zwidth==0){
		return 0;
	}
	approximationFlag = ERROR_INTEGRATION_FLAG_BASE_STATE;
	double r = improperRombergIntegral(function,domain,epsilon,finalError,MAXN,MAXR);
	if(approximationFlag == ERROR_INTEGRATION_FLAG_TRIGGERED_STATE){
		std::cerr << WARNING_LOG << "at least one recursion reached maximum depth."
					<< " Error may be greater than the one required." << std::endl;
	}
	return r;
}

//========= private functions =========//

// functions related to the evaluation of the integral:

// Function that manages the integral domains and calls other functions to actually compute the integral.
// What it does is checking if the domains are unbounded, and if they are it splits the domain into a part that
// is finite, and up to other 8 unbounded parts. The unbounded parts (for a given coordinate) are of the type (-inf,a)
// or (a,+infy), and since each coordinate can assume up to 2 unbounded intervals, the maximum number is if all the 3 coordinates
// assume both this interval. From this 2*2*2=8.
// "axis" refers to the coordinate that is being observed. The order is 0="x" -> 1="y" -> 2="z".
// xSubstitution,ySubstitution,zSubstitution are three variables to remember if it's the domain being watched is unbounded.
// They assume the values {-1,0,1}, where -1: unbounded from left, 0 bounded, 1 unbounded from right. It's called
// substitution because later in the code being unbounded will be resolved via a coordinante substitution.
double Integral3D::improperRombergIntegral(const Function3D &function, const Parallelepiped &domain,
											const double &epsilon, double &finalError, const int &MAXN, const int &MAXR,
											const int &axis, const int &xSubstitution, const int &ySubstitution){
	if(axis==0){
		// check if the width is 2*infinity, to see if it's unbounded
		// on both sides
		if(domain.xwidth>=2*COORDINATE_INFINITY){
			// domain is split into (-inf,-a), (-a,a), (a,+inf)
			Parallelepiped dLeft(domain),dMid(domain),dRight(domain);
			dLeft.xwidth = dLeft.xwidth/2-MAX_BOUNDED_SIZE;

			dMid.vertex.x = (dMid.vertex.x+dMid.xwidth/2)-MAX_BOUNDED_SIZE;
			dMid.xwidth = 2*MAX_BOUNDED_SIZE;

			dRight.vertex.x = MAX_BOUNDED_SIZE;
			dRight.xwidth = dRight.xwidth/2-MAX_BOUNDED_SIZE;
			
			return improperRombergIntegral(function,dLeft,epsilon,finalError,MAXN,MAXR,1,-1)+
					improperRombergIntegral(function,dMid,epsilon,finalError,MAXN,MAXR,1,0)+
					improperRombergIntegral(function,dRight,epsilon,finalError,MAXN,MAXR,1,1);
		}else if(domain.xwidth>=COORDINATE_INFINITY){
			// check if the width is infinity(but less than 2*inf), to see if it's unbounded
			// on only one side. Which side is unbounded must be understood.

			// if it's unbounded on the right the vertex is above -inf 
			if(domain.vertex.x > -COORDINATE_INFINITY){
				Parallelepiped dMid(domain),dRight(domain);

				dMid.xwidth = MAX_BOUNDED_SIZE;

				dRight.vertex.x = dRight.vertex.x+MAX_BOUNDED_SIZE;
				dRight.xwidth = dRight.xwidth-MAX_BOUNDED_SIZE;
				
				return improperRombergIntegral(function,dMid,epsilon,finalError,MAXN,MAXR,1,0)+
						improperRombergIntegral(function,dRight,epsilon,finalError,MAXN,MAXR,1,1);
			}else{
				Parallelepiped dLeft(domain),dMid(domain);

				dMid.vertex.x = (dMid.vertex.x+dMid.xwidth)-MAX_BOUNDED_SIZE;
				dMid.xwidth = MAX_BOUNDED_SIZE;

				dLeft.xwidth = dLeft.xwidth-MAX_BOUNDED_SIZE;
				
				return improperRombergIntegral(function,dMid,epsilon,finalError,MAXN,MAXR,1,xSubstitution,0)+
						improperRombergIntegral(function,dLeft,epsilon,finalError,MAXN,MAXR,1,xSubstitution,-1);
			}
		}else{
			// if domain is "finite"(aka the width is below the maximum allowed)
			return improperRombergIntegral(function,domain,epsilon,finalError,MAXN,MAXR,1,0);
		}
	}else if(axis==1){
		// same process for "y"

		if(domain.ywidth>=2*COORDINATE_INFINITY){
			// double unbounded
			Parallelepiped dLeft(domain),dMid(domain),dRight(domain);
			dLeft.ywidth = dLeft.ywidth/2-MAX_BOUNDED_SIZE;

			dMid.vertex.y = (dMid.vertex.y+dMid.ywidth/2)-MAX_BOUNDED_SIZE;
			dMid.ywidth = 2*MAX_BOUNDED_SIZE;

			dRight.vertex.y = MAX_BOUNDED_SIZE;
			dRight.ywidth = dRight.ywidth/2-MAX_BOUNDED_SIZE;
			
			return improperRombergIntegral(function,dLeft,epsilon,finalError,MAXN,MAXR,2,xSubstitution,-1)+
					improperRombergIntegral(function,dMid,epsilon,finalError,MAXN,MAXR,2,xSubstitution,0)+
					improperRombergIntegral(function,dRight,epsilon,finalError,MAXN,MAXR,2,xSubstitution,1);
		}else if(domain.ywidth>=COORDINATE_INFINITY){
			// single unbounded

			if(domain.vertex.y > -COORDINATE_INFINITY){
				Parallelepiped dMid(domain),dRight(domain);

				dMid.ywidth = MAX_BOUNDED_SIZE;

				dRight.vertex.y = dRight.vertex.y+MAX_BOUNDED_SIZE;
				dRight.ywidth = dRight.ywidth-MAX_BOUNDED_SIZE;
				
				return improperRombergIntegral(function,dMid,epsilon,finalError,MAXN,MAXR,2,xSubstitution,0)+
						improperRombergIntegral(function,dRight,epsilon,finalError,MAXN,MAXR,2,xSubstitution,1);
			}else{
				Parallelepiped dLeft(domain),dMid(domain);

				dMid.vertex.y = (dMid.vertex.y+dMid.ywidth)-MAX_BOUNDED_SIZE;
				dMid.ywidth = MAX_BOUNDED_SIZE;

				dLeft.ywidth = dLeft.ywidth-MAX_BOUNDED_SIZE;

				return improperRombergIntegral(function,dMid,epsilon,finalError,MAXN,MAXR,2,xSubstitution,0)+
						improperRombergIntegral(function,dLeft,epsilon,finalError,MAXN,MAXR,2,xSubstitution,-1);
			}
		}else{
			return improperRombergIntegral(function,domain,epsilon,finalError,MAXN,MAXR,2,xSubstitution,0);
		}
	}else if(axis==2){
		// if it's z axis instead of doing recursion we call functions to actually
		// compute the integral

		// check if the width is 2*infinity, to see if it's unbounded
		// on both sides
		if(domain.zwidth>=2*COORDINATE_INFINITY){
			std::cout << "::"<<xSubstitution<<","<<ySubstitution<<","<<domain.zwidth<<"-"<<2*COORDINATE_INFINITY<<"\n";
			Parallelepiped dLeft(domain),dMid(domain),dRight(domain);
			dLeft.zwidth = dLeft.zwidth/2-MAX_BOUNDED_SIZE;

			dMid.vertex.z = (dMid.vertex.z+dMid.zwidth/2)-MAX_BOUNDED_SIZE;
			dMid.zwidth = 2*MAX_BOUNDED_SIZE;

			dRight.vertex.z = MAX_BOUNDED_SIZE;
			dRight.zwidth = dRight.zwidth/2-MAX_BOUNDED_SIZE;

			return evaluateImproperRombergIntegral(function,dLeft,epsilon,finalError,MAXN,MAXR,xSubstitution,ySubstitution,-1)+
					evaluateImproperRombergIntegral(function,dMid,epsilon,finalError,MAXN,MAXR,xSubstitution,ySubstitution,0)+
					evaluateImproperRombergIntegral(function,dRight,epsilon,finalError,MAXN,MAXR,xSubstitution,ySubstitution,1);
		}else if(domain.zwidth>=COORDINATE_INFINITY){
			// single unbounded
			
			// if it's unbounded on the right the vertex is above -inf 
			if(domain.vertex.z > -COORDINATE_INFINITY){			
				Parallelepiped dMid(domain),dRight(domain);

				dMid.zwidth = MAX_BOUNDED_SIZE;

				dRight.vertex.z = dRight.vertex.z+MAX_BOUNDED_SIZE;
				dRight.zwidth = dRight.zwidth-MAX_BOUNDED_SIZE;
				
				return evaluateImproperRombergIntegral(function,dMid,epsilon,finalError,MAXN,MAXR,xSubstitution,ySubstitution,0)+
						evaluateImproperRombergIntegral(function,dRight,epsilon,finalError,MAXN,MAXR,xSubstitution,ySubstitution,1);
			}else{
				Parallelepiped dLeft(domain),dMid(domain);

				dMid.vertex.z = (dMid.vertex.z+dMid.zwidth)-MAX_BOUNDED_SIZE;
				dMid.zwidth = MAX_BOUNDED_SIZE;

				dLeft.zwidth = dLeft.zwidth-MAX_BOUNDED_SIZE;
				
				return evaluateImproperRombergIntegral(function,dMid,epsilon,finalError,MAXN,MAXR,xSubstitution,ySubstitution,0)+
						evaluateImproperRombergIntegral(function,dLeft,epsilon,finalError,MAXN,MAXR,xSubstitution,ySubstitution,-1);
			}
		}else{
			return evaluateImproperRombergIntegral(function,domain,epsilon,finalError,MAXN,MAXR,xSubstitution,ySubstitution,0);
		}
	}
	return 0;
}

// This function receives a domain that may be unbounded, but it's already been processed by improperRombergIntegral().
// Even if it's called evaluateImproperRombergIntegral it can also evaluate standard integrals. In fact what it does
// it taking in arbitrary domains, converting them into bounded domains, and them evaluate the standard integral.
double Integral3D::evaluateImproperRombergIntegral(const Function3D &function, const Parallelepiped &domain,
													const double &epsilon, double &finalError, const int &MAXN,
													const int &MAXR, const int &xSubstitution, const int &ySubstitution,
													const int &zSubstitution){
	// Set Integral3D "substitution" attributes to corresponding values. This is important
	// because rombergIntegral relies on those attributes to calculate the actual
	// integral, and not the integral of f(x,y,z) on the modified domain(which would be wrong).
	setChangeOfVariable(xSubstitution,ySubstitution,zSubstitution);
	Parallelepiped newDomain(domain);
	// transform domain into a bounded one
	int sign = changeDomainOfIntegration(newDomain,xSubstitution,ySubstitution,zSubstitution);
	return sign*rombergIntegral(function,newDomain,epsilon,finalError,MAXN,MAXR);
}

// This function is responsable to actually calculate the integral using Romberg's algorithm, with adaptive
// quadrature. MAXN is the maximum depth of Romberg's algorithm, whilst MAXR is the maximum recursion depth.
// "recursion" keeps track of the recursion depth the function is in, by default it's 0.
double Integral3D::rombergIntegral(const Function3D &function, const Parallelepiped &domain,
									const double &epsilon, double &finalError, const int &MAXN, const int &MAXR,
									const int &recursion){
	// If received domain has depth 0 on any dimension, the integral is 0.
	// std::cout << domain.vertex.x << " "<<domain.vertex.y<<" "<<domain.vertex.z<<"\n";
	// std::cout << domain.xwidth << " "<<domain.ywidth<<" "<<domain.zwidth<<"\n";
	std::cout << domain.vertex.x << " " << domain.vertex.y << " " << domain.vertex.z << std::endl;
	std::cout << domain.xwidth << " " << domain.ywidth << " " << domain.zwidth << std::endl;
	std::cout << isXSubstituted << " " << isYSubstituted << " " << isZSubstituted << "\n\n";
	if(domain.xwidth==0 or domain.ywidth==0 or domain.zwidth==0){
		return 0;
	}
	if(MAXN==0){
		return 0;
	}
	// Romberg's algorithm
	double R[MAXN][MAXN]; // Romberg's table
	double temp;
	int i,j;
	R[0][0] = directionedTrapezoidIntegral(function, domain, 0); // trapezoidal integral
	for(i=1;i<MAXN;++i){
		// trapezoidal integral using successing refinements
		R[i][0] = R[i-1][0]/8+directionedTrapezoidIntegral(function, domain, pow(2,i)-1);
		// std::cout << R[i][0]<<"\n";
		for(j=1;j<=i;++j){
			temp = pow(4,j);
			R[i][j] = ( temp*R[i][j-1]-R[i-1][j-1] )/( temp-1 ); // Richardson's extrapolation
		}
		// checking for early stop if error tolerance is met
		if(std::fabs(R[i-1][i-1]-R[i][i])<epsilon and i-1>=0 and R[i][i]!=0 and R[i-1][i-1]!=0){
			finalError += std::fabs(R[i-1][i-1]-R[i][i]); 
			return R[i][i];
		}
	}

	for(int k=0;k<MAXN;++k){
		for(int p=0;p<=k;++p){
			std::cout << R[k][p] << " ";
		}
		std::cout << std::endl;
	}

	// adaptive integration implementation
	if(recursion<MAXR){
		int split_number = 8;
		double sum = 0;
		std::vector<Parallelepiped> newDomains;
		newDomains.resize(split_number);
		splitDomain(domain,newDomains);
		for(i=0;i<split_number;++i){
			sum += rombergIntegral(function,newDomains[i],epsilon/split_number,
									finalError,MAXN,MAXR,recursion+1);
		}
		return sum;
	}

	// if both MAXN and MAXR are reached the "best" value obtained is returned
	approximationFlag = ERROR_INTEGRATION_FLAG_TRIGGERED_STATE;
	if(MAXN==1){
		finalError += R[0][0];
	}else{
		finalError += std::fabs(R[MAXN-2][MAXN-2]-R[MAXN-1][MAXN-1]); 
	}
	return R[MAXN-1][MAXN-1];
}

// This function compute the trapezoidal rule on the given domain, considering
// the refinement at n points. Being in 3 dimension an extended 3D form is used.
// Standard trapezoidal rule is applied on "x" axis, but for every point it's applied the
// trapezoidal rule over the "y" axis, and for every point of the "y" axis it's applied the
// trapezoidal rule over the "z" axis, over which a standard trapezoidal rule is used.
// "axis" represent the axis over which the trapezoidal rule is being applied on,
// 0="x", 1="y" and 2="z". "fixedX" and "fixedY" are the constant values over which
// the trapezoidal rule is applied. Either "y"(with fixedX constant), or "z"(with both fixedX and
// fixedY constant).



double Integral3D::directionedTrapezoidIntegral(const Function3D &function, const Parallelepiped &domain,
													const int &n, const int &axis, const double &fixedX,
													const double &fixedY) const{
	int i,j,k;
	double hx,hy,hz,sumx,sumy,sumz,temp;
	hx = domain.xwidth/(n+1);
	hy = domain.ywidth/(n+1);
	hz = domain.zwidth/(n+1);
	sumx = 0;
	for(i=0;i<=n+1;++i){
		sumy = 0;
		for(j=0;j<=n+1;++j){
			sumz = 0;
			// if either on x, or y I end up on a new line
			// every point is a new one
			if(i%2==1 or j%2==1 or n==0){
				for(k=0;k<=n+1;++k){
					temp = function(standardInverseChangeOfVariableFunction(domain.vertex.x+i*hx,"x"),
								standardInverseChangeOfVariableFunction(domain.vertex.y+j*hy,"y"),
								standardInverseChangeOfVariableFunction(domain.vertex.z+k*hz,"z"))*
								changeOfVariableDifferential(domain.vertex.x+i*hx,domain.vertex.y+j*hy,
																domain.vertex.z+k*hz);
					if(k==0 or k==n+1){
						temp *= 0.5;
					}
					sumz += temp;
				}
			}else{
				// case when some of the values has already been accounted for
				for(k=1;k<=n;k+=2){
					sumz += function(standardInverseChangeOfVariableFunction(domain.vertex.x+i*hx,"x"),
								standardInverseChangeOfVariableFunction(domain.vertex.y+j*hy,"y"),
								standardInverseChangeOfVariableFunction(domain.vertex.z+k*hz,"z"))*
								changeOfVariableDifferential(domain.vertex.x+i*hx,domain.vertex.y+j*hy,
																domain.vertex.z+k*hz);
				}
			}
			if(j==0 or j==n+1){
				sumz *= 0.5;
			}
			sumy += sumz;
		}
		if(i==0 or i==n+1){
			sumy *= 0.5;
		}
		sumx += sumy;
	}
	return hx*hy*hz*sumx;
}

// functions related to the change of variable:

// this is the change of variable function used for the improper integrals
double Integral3D::standardChangeOfVariableFunction(const double &value) const{
	// e^x/(1+e^x) = 1-1/(1+e^x)
	return 1-1/(1+pow(M_E,value));
}

// this is the inverse change of variable function used for the improper integrals
double Integral3D::standardInverseChangeOfVariableFunction(const double &value,
															const std::string &coordinate) const{
	if((isXSubstituted!=0 and coordinate=="x")
		or (isYSubstituted!=0 and coordinate=="y")
		or (isZSubstituted!=0 and coordinate=="z")){
		// e^x/(1+e^x) = 1-1/(1+e^x)
		return -log(-1+1/value);
	}
	return value;
}

// function that takes in input the variables x,y or z, and returns the changed
// variable(if it doesn't need a transformation the "transformation" is the identity)
double Integral3D::changeVariable(const double &coordinate, const double &axis) const{
	if(axis==0 and isXSubstituted){
		return standardChangeOfVariableFunction(coordinate);
	}else if(axis==1 and isYSubstituted){
		return standardChangeOfVariableFunction(coordinate);
	}else if(axis==2 and isZSubstituted){
		return standardChangeOfVariableFunction(coordinate);
	}
	return coordinate;
}

// Function that update the state of the necessary substitution to make.
// Default values are 0,0,0, so an empty call reset the state.
void Integral3D::setChangeOfVariable(const int &xSubstitution, const int &ySubstitution,
										const int &zSubstitution){
	isXSubstituted = xSubstitution;
	isYSubstituted = ySubstitution;
	isZSubstituted = zSubstitution;
}

// function that calculates the updated differential, due to a possible change of variables,
// if there are none 1 is returned
double Integral3D::changeOfVariableDifferential(const double &u, const double &v, const double &w) const{
	double r = 1;
	if(isXSubstituted){
		if(u==1 or u==0){
			// this values gives infinite contribute,
			// to prevent infinities 0 is returned.
			// removing a point from the integration doesn't affect
			// the integral
			return 0;
		}
		// since the input is already transformed, and since the
		// differential is expressed in terms of x, the inverse
		// function is called
		double x = standardInverseChangeOfVariableFunction(u,"x");
		if(x<10){
			// standard differential contribution
			r *= pow(1+pow(M_E,x),2)/pow(M_E,x);
		}else{
			// approximation for large "x"s
			r *= pow(M_E,x);
		}
	}
	// same logic for the other variables
	if(isYSubstituted){
		if(v==1 or v==0){
			return 0;
		}
		double y = standardInverseChangeOfVariableFunction(v,"y");
		if(y<10){
			r *= pow(1+pow(M_E,y),2)/pow(M_E,y);
		}else{
			r *= pow(M_E,y);
		}
	}
	if(isZSubstituted){
		if(w==1 or w==0){
			return 0;
		}
		double z = standardInverseChangeOfVariableFunction(w,"z");
		if(z<10){
			r *= pow(1+pow(M_E,z),2)/pow(M_E,z);
		}else{
			r *= pow(M_E,z);
		}
	}
	return r;
}

// functions related to the domain management:

// function that given a Functinon3D, extrapolates from the two inequalities
// the minimum rectangular domain that contains the intersection of the
// two inequalities
Parallelepiped Integral3D::rectanglifyDomain(const Function3D &function) const{
	double xMin,yMin,zMin,xMax,yMax,zMax;
	xMin = yMin = zMin = -COORDINATE_INFINITY;
	xMax = yMax = zMax = COORDINATE_INFINITY;

	// analyzing first inequality
	applyInequality(function.getInequality(1),xMin,xMax,yMin,yMax,zMin,zMax);
	// analyzing second inequality
	applyInequality(function.getInequality(2),xMin,xMax,yMin,yMax,zMin,zMax);
	return Parallelepiped(Point3D(xMin,yMin,zMin),xMax-xMin,yMax-yMin,zMax-zMin);
}

// function that extrapolate the ranges of an inequality for each coordinate, and intersects
// it with the (xMin,xMax),... ranges given
void Integral3D::applyInequality(const Inequality &inequality, double &xMin, double &xMax,
									double &yMin, double &yMax, double &zMin, double &zMax) const{
	// ranges are temporarily stored in the "min","max" variables
	double min, max;
	getRange(inequality, "x", min, max);
	if(min>xMin){
		xMin = min;
	}
	if(max<xMax){
		xMax = max;
	}
	getRange(inequality, "y", min, max);
	if(min>yMin){
		yMin = min;
	}
	if(max<yMax){
		yMax = max;
	}
	getRange(inequality, "z", min, max);
	if(min>zMin){
		zMin = min;
	}
	if(max<zMax){
		zMax = max;
	}
}

// Function that takes in input an inequality, and update the "min" and "max" variables
// with the min and max values assumed by the "axis" coordinate.
// For more details visit https://github.com/Sonodaart/Triple-Integral-Calculator/blob/main/README.md#numerical-solution
void Integral3D::getRange(const Inequality &_inequality, const std::string &axis,
							double &min, double &max) const{
	double k,discriminant;
	min = -COORDINATE_INFINITY;
	max = COORDINATE_INFINITY;
	std::string first,second;
	if(axis == "x"){
		first = "y";
		second = "z";
	}
	if(axis == "y"){
		first = "x";
		second = "z";
	}
	if(axis == "z"){
		first = "x";
		second = "y";
	}
	Inequality inequality(_inequality);
	inequality.standardize(); // disequality is of type ">"

	// comments will assume fixed="x", first="y" and second="z"
	if(inequality[axis+"^2"]>0){
		// Case A>0
		return;
	}else if(inequality[axis+"^2"]<0){
		// Case A<0
		if(inequality[first+"^2"]>0 or inequality[second+"^2"]>0){
			// Case B>0 or C>0
			return;
		}else if(inequality[first+"^2"]<0 and inequality[second+"^2"]<0){
			// Case B<0 and C<0
			// change of notation for A,a,B,b,C,c
			k = -inequality["r"]+pow(inequality[first],2)/(4*inequality[first+"^2"])+pow(inequality[second],2)/(4*inequality[second+"^2"]);
			discriminant = pow(inequality[axis],2)+4*inequality[axis+"^2"]*k;
			if(discriminant>0){
				min = -(inequality[axis]-sqrt(discriminant))/(2*inequality[axis+"^2"]);
				max = -(inequality[axis]+sqrt(discriminant))/(2*inequality[axis+"^2"]);
				return;
			}else if(discriminant<=0){
				// 0 included since it means it has no width
				return;
			}
		}else if(inequality[first+"^2"]<0 and inequality[second+"^2"]==0){
			// Case B<0 and C=0
			if(inequality[second]!=0){
				// Case c!=0
				return;
			}else if(inequality[second]==0){
				// Case c=0
				// change of notation for A,a,B,b
				k = -inequality["r"]+pow(inequality[first],2)/(4*inequality[first+"^2"]);
				discriminant = pow(inequality[axis],2)+4*inequality[axis+"^2"]*k;
				if(discriminant>0){
					min = -(inequality[axis]-sqrt(discriminant))/(2*inequality[axis+"^2"]);
					max = -(inequality[axis]+sqrt(discriminant))/(2*inequality[axis+"^2"]);
					return;
				}else if(discriminant<=0){
					// 0 included since it means it has no width
					return;
				}
			}
		}else if(inequality[first+"^2"]==0 and inequality[second+"^2"]<0){
			// Case B=0 and C<0
			if(inequality[first]!=0){
				// Case b!=0
				return;
			}else if(inequality[first]==0){
				// Case b=0
				// change of notation for A,a,C,c
				k = -inequality["r"]+pow(inequality[second],2)/(4*inequality[second+"^2"]);
				discriminant = pow(inequality[axis],2)+4*inequality[axis+"^2"]*k;
				if(discriminant>0){
					min = -(inequality[axis]-sqrt(discriminant))/(2*inequality[axis+"^2"]);
					max = -(inequality[axis]+sqrt(discriminant))/(2*inequality[axis+"^2"]);
					return;
				}else if(discriminant<=0){
					// 0 included since it means it has no width
					return;
				}
			}
		}else if(inequality[first+"^2"]==0 and inequality[second+"^2"]==0){
			// Case B=0 and C=0
			if(inequality[first]!=0 or inequality[second]!=0){
				// Case b!=0 or c!=0
				return;
			}else if(inequality[first]==0 and inequality[second]==0){
				// Case b==0 and c==0
				// change of notation for A,a
				discriminant = pow(inequality[axis],2)-4*inequality[axis+"^2"]*inequality["r"];
				if(discriminant>0){
					min = -(inequality[axis]-sqrt(discriminant))/(2*inequality[axis+"^2"]);
					max = -(inequality[axis]+sqrt(discriminant))/(2*inequality[axis+"^2"]);
					return;
				}else if(discriminant<=0){
					// 0 included since it means it has no width
					return;
				}
			}
		}
	}else if(inequality[axis+"^2"]==0){
		// Case A=0
		if(inequality[first+"^2"]>0 or inequality[second+"^2"]>0){
			// Case B>0 or C>0
			return;
		}else if(inequality[first+"^2"]<0 and inequality[second+"^2"]<0){
			// Case B<0 and C<0
			// change of notation for a,B,b,C,c
			k = inequality["r"]-pow(inequality[first],2)/(4*inequality[first+"^2"])-pow(inequality[second],2)/(4*inequality[second+"^2"]);
			if(inequality[axis]>0){
				// Case a>0
				min = -k/inequality[axis];
				return;
			}else if(inequality[axis]<0){
				// Case a<0
				max = -k/inequality[axis];
				return;
			}else if(inequality[axis]==0){
				// Case a=0
				if((k>=0 and inequality.getDisequality()==">=") or (k>0 and inequality.getDisequality()==">")){
					return;
				}else if(k<0){
					min = max = 0;
					return;
				}
			}
		}else if(inequality[first+"^2"]==0 or inequality[second+"^2"]==0){
			// Case B=0 or C=0
			if(inequality[first+"^2"]==0 and inequality[first]!=0){
				// Case B=0 and b!=0
				return;
			}else if(inequality[second+"^2"]==0 and inequality[second]!=0){
				// Case C=0 and c!=0
				return;
			}else if(inequality[first+"^2"]==0 and inequality[second+"^2"]==0){
				// Case B=0 and C=0
				if(inequality[first]!=0 or inequality[second]!=0){
					// Case b!=0 or c!=0
					return;
				}else if(inequality[first]==0 and inequality[second]==0){
					// Case b=0 and c=0
					if(inequality[axis]>0){
						// Case a>0
						min = -inequality["r"]/inequality[axis];
						return;
					}else if(inequality[axis]<0){
						// Case a<0
						max = -inequality["r"]/inequality[axis];
						return;
					}else if(inequality[axis]==0){
						// Case a=0
						if(inequality(0,0,0)){
							return;
						}else{
							min = max = 0;
							return;
						}
					}
				}
			}
		}
	}
}

// function that given a domain, it transform it accordingly to the substitution passed in input
int Integral3D::changeDomainOfIntegration(Parallelepiped &domain, const double &xSubstitution,
											const double &ySubstitution, const double &zSubstitution) const{
	double bound1,bound2;
	int sign_inversion = 1;
	if(isXSubstituted){
		bound1 = standardChangeOfVariableFunction(domain.vertex.x);
		bound2 = standardChangeOfVariableFunction(domain.vertex.x+domain.xwidth);
		if(bound1>bound2){
			sign_inversion *= -1;
		}
		domain.vertex.x = std::min(bound1,bound2);
		domain.xwidth = std::fabs(bound2-bound1);
	}
	if(isYSubstituted){
		bound1 = standardChangeOfVariableFunction(domain.vertex.y);
		bound2 = standardChangeOfVariableFunction(domain.vertex.y+domain.ywidth);
		if(bound1>bound2){
			sign_inversion *= -1;
		}
		domain.vertex.y = std::min(bound1,bound2);
		domain.ywidth = std::fabs(bound2-bound1);
	}
	if(isZSubstituted){
		bound1 = standardChangeOfVariableFunction(domain.vertex.z);
		bound2 = standardChangeOfVariableFunction(domain.vertex.z+domain.zwidth);
		if(bound1>bound2){
			sign_inversion *= -1;
		}
		domain.vertex.z = std::min(bound1,bound2);
		domain.zwidth = std::fabs(bound2-bound1);
	}
	return sign_inversion;
}

// function used for the adaptive strategy, it split the domain in 8 equal-sized subdomains
void Integral3D::splitDomain(const Parallelepiped &domain, std::vector<Parallelepiped> &newD) const{
	unsigned int i;
	double halfx, halfy, halfz;
	halfx = domain.xwidth/2;
	halfy = domain.ywidth/2;
	halfz = domain.zwidth/2;

	for(i=0;i<newD.size();++i){
		newD[i].xwidth = halfx;
		newD[i].ywidth = halfy;
		newD[i].zwidth = halfz;
	}

	// leftmost
	i = 0;
	newD[i].vertex = domain.vertex;
	
	// rightmost
	i = 1;
	newD[i].vertex.x = domain.vertex.x+halfx;
	newD[i].vertex.y = domain.vertex.y+halfy;
	newD[i].vertex.z = domain.vertex.z+halfz;
	
	// one displacement
	i = 2;
	newD[i].vertex.x = domain.vertex.x+halfx;
	newD[i].vertex.y = domain.vertex.y;
	newD[i].vertex.z = domain.vertex.z;

	i = 3;
	newD[i].vertex.x = domain.vertex.x;
	newD[i].vertex.y = domain.vertex.y+halfy;
	newD[i].vertex.z = domain.vertex.z;

	i = 4;
	newD[i].vertex.x = domain.vertex.x;
	newD[i].vertex.y = domain.vertex.y;
	newD[i].vertex.z = domain.vertex.z+halfz;

	// two displacements
	i = 5;
	newD[i].vertex.x = domain.vertex.x+halfx;
	newD[i].vertex.y = domain.vertex.y+halfy;
	newD[i].vertex.z = domain.vertex.z;
	
	i = 6;
	newD[i].vertex.x = domain.vertex.x+halfx;
	newD[i].vertex.y = domain.vertex.y;
	newD[i].vertex.z = domain.vertex.z+halfz;
	
	i = 7;
	newD[i].vertex.x = domain.vertex.x;
	newD[i].vertex.y = domain.vertex.y+halfy;
	newD[i].vertex.z = domain.vertex.z+halfz;
}











// double Integral3D::directionedTrapezoidIntegral(const Function3D &function, const Parallelepiped &domain,
// 													const int &n, const int &axis, const double &fixedX,
// 													const double &fixedY) const{
// 	int i;
// 	double step,sum;
// 	sum = 0;
// 	if(axis == 0){
// 		step = domain.xwidth/pow(2,n);
// 		sum = 0.5*(directionedTrapezoidIntegral(function,domain,n,1,domain.vertex.x)+
// 					directionedTrapezoidIntegral(function,domain,n,1,domain.vertex.x+domain.xwidth));
// 	}else if(axis == 1){
// 		step = domain.ywidth/pow(2,n);
// 		sum = 0.5*(directionedTrapezoidIntegral(function,domain,n,2,fixedX,domain.vertex.y)+
// 					directionedTrapezoidIntegral(function,domain,n,2,fixedX,domain.vertex.y+domain.ywidth));
// 	}else if(axis == 2){
// 		step = domain.zwidth/pow(2,n-1);
// 		if(n==0){
// 			sum = 0.5*(function(fixedX,fixedY,domain.vertex.z)+function(fixedX,fixedY,domain.vertex.z+domain.zwidth));
// 		}
// 	}
	
// 	if(axis==0 or axis==1){
// 		for(i=1;i<pow(2,n);++i){
// 			if(axis == 0){
// 				sum += directionedTrapezoidIntegral(function,domain,n,1,domain.vertex.x+i*step);
// 			}else if(axis == 1){
// 				sum += directionedTrapezoidIntegral(function,domain,n,2,fixedX,domain.vertex.y+i*step);
// 			}
// 		}
// 	}else if(axis==2){
// 		for(i=1;i<=pow(2,n-1);++i){
// 			sum += function(fixedX,fixedY,domain.vertex.z+i*step-step/2)*changeOfVariableDifferential(fixedX,fixedY,domain.vertex.z+i*step-step/2);
// 		}
// 	}

// 	return step*sum;
// }