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
	finalError = 0; // reset error
	// if domain has at least one coordinate that doesn't have width, it's
	// 2 dimensional, and 3D integrals on 2D surfaces are 0
	if(domain.xwidth==0 or domain.ywidth==0 or domain.zwidth==0){
		return 0;
	}
	approximationFlag = ERROR_INTEGRATION_FLAG_BASE_STATE;
	double r = rombergIntegral(function,domain,epsilon,finalError,MAXN,MAXR);
	if(approximationFlag == ERROR_INTEGRATION_FLAG_TRIGGERED_STATE){
		std::cerr << WARNING_LOG << "at least one recursion reached maximum depth."
					<< " Error may be greater than the one required." << std::endl;
	}
	return r;
}

//========= private functions =========//

// functions related to the evaluation of the integral:

// This function is responsable to calculate the integral using Romberg's algorithm, with adaptive
// quadrature. MAXN is the maximum depth of Romberg's algorithm, whilst MAXR is the maximum recursion depth.
// "recursion" keeps track of the recursion depth the function is in, by default it's 0.
double Integral3D::rombergIntegral(const Function3D &function, const Parallelepiped &domain,
									const double &epsilon, double &finalError, const int &MAXN, const int &MAXR,
									const int &recursion){
	// If received domain has depth 0 on any dimension, the integral is 0.
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
		// 0s are excluded cause it may be not enough refined to find
		// points inisde the domain
		if(std::fabs(R[i][i-1]-R[i][i])<epsilon and i-1>=0 and R[i][i]!=0 and R[i][i-1]!=0){
			finalError += std::fabs(R[i][i-1]-R[i][i]); 
			return R[i][i];
		}
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
	if(R[MAXN-1][MAXN-1]!=0){
		std::cout << R[MAXN-1][MAXN-1] << " " << R[MAXN-2][MAXN-2] << "\n";
		approximationFlag = ERROR_INTEGRATION_FLAG_TRIGGERED_STATE;
	}
	if(MAXN==1){
		finalError += std::fabs(R[0][0]);
	}else{
		finalError += std::fabs(R[MAXN-1][MAXN-2]-R[MAXN-1][MAXN-1]); 
	}
	return R[MAXN-1][MAXN-1];
}

// This function compute the trapezoidal rule on the given domain, considering
// the refinement at n points. Being in 3 dimension an extended 3D form is used.
// Standard trapezoidal rule is applied on "x" axis, but for every point it's applied the
// trapezoidal rule over the "y" axis, and for every point of the "y" axis it's applied the
// trapezoidal rule over the "z" axis, over which a standard trapezoidal rule is used.
double Integral3D::directionedTrapezoidIntegral(const Function3D &function, const Parallelepiped &domain,
													const int &n) const{
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
					temp = function(domain.vertex.x+i*hx,domain.vertex.y+j*hy,domain.vertex.z+k*hz);
					if(k==0 or k==n+1){
						temp *= 0.5;
					}
					sumz += temp;
				}
			}else{
				// case when some of the values has already been accounted for
				for(k=1;k<=n;k+=2){
					sumz += function(domain.vertex.x+i*hx,domain.vertex.y+j*hy,domain.vertex.z+k*hz);
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
	makeDomainFinite(xMin,xMax,yMin,yMax,zMin,zMax);
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

void Integral3D::makeDomainFinite(double &xMin, double &xMax, double &yMin, double &yMax, double &zMin, double &zMax) const{
	if(xMax-xMin>=2*COORDINATE_INFINITY){
		xMin = -MAX_BOUNDED_SIZE/2;
		xMax = MAX_BOUNDED_SIZE/2;
	}else if(xMax-xMin>=COORDINATE_INFINITY){
		if(xMin==-COORDINATE_INFINITY){
			xMin = xMax-MAX_BOUNDED_SIZE;
		}else if(xMax==COORDINATE_INFINITY){
			xMax = xMin+MAX_BOUNDED_SIZE;
		}
	}
	if(yMax-yMin>=2*COORDINATE_INFINITY){
		yMin = -MAX_BOUNDED_SIZE/2;
		yMax = MAX_BOUNDED_SIZE/2;
	}else if(yMax-yMin>=COORDINATE_INFINITY){
		if(yMin==-COORDINATE_INFINITY){
			yMin = yMax-MAX_BOUNDED_SIZE;
		}else if(yMax==COORDINATE_INFINITY){
			yMax = yMin+MAX_BOUNDED_SIZE;
		}
	}
	if(zMax-zMin>=2*COORDINATE_INFINITY){
		zMin = -MAX_BOUNDED_SIZE/2;
		zMax = MAX_BOUNDED_SIZE/2;
	}else if(zMax-zMin>=COORDINATE_INFINITY){
		if(zMin==-COORDINATE_INFINITY){
			zMin = zMax-MAX_BOUNDED_SIZE;
		}else if(zMax==COORDINATE_INFINITY){
			zMax = zMin+MAX_BOUNDED_SIZE;
		}
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