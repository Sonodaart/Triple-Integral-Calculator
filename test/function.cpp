#ifdef __cplusplus
#define EXPORT_SYMBOL extern "C" __attribute__((visibility("default")))
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif
#include <map>
#include <cmath>
#include <string>

EXPORT_SYMBOL double f(double x, double y, double z){
	// if(0<=x and x<=3 and 0<=y and y<z*z and 0<=z and z<=1)return y*cos(pow(z,5));
	return y*z;
}

EXPORT_SYMBOL std::map<std::string,double> first = {
	{"x",1},
	{"y^2",-2},
	{"z^2",-2},
	{"r",5},
	{"<=",1}
};

EXPORT_SYMBOL std::map<std::string,double> second = {
	{"x",1},
	{"r",-1},
	{"<=",1}
};