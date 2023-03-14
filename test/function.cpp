#ifdef __cplusplus
#define EXPORT_SYMBOL extern "C" __attribute__((visibility("default")))
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif
#include <map>
#include <cmath>
#include <string>

EXPORT_SYMBOL double f(double x, double y, double z) {
	if(0<=x and x<=3 and 0<=y and y<z*z and 0<=z and z<=1)return y*cos(pow(z,5));
	// if(2<=x and x<=3 and -1<=y and y<4 and 0<=z and z<=1)return pow(z,3)-4*x*x*y;
	return 0;
}

EXPORT_SYMBOL std::map<std::string,double> first = {
	{"x^2",1},
	{"y^2",1},
	{"z^2",1},
	{"r",-30},
	{"<",1}
};

EXPORT_SYMBOL std::map<std::string,double> second = first;