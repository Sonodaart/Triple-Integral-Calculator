#ifdef __cplusplus
#define EXPORT_SYMBOL extern "C" __attribute__((visibility("default")))
#else
#define EXPORT_SYMBOL __attribute__((visibility("default")))
#endif
#include <map>
#include <string>

EXPORT_SYMBOL double f(double x, double y, double z);
EXPORT_SYMBOL std::map<std::string,double> first,second;

double f(double x, double y, double z){
	return 5*x*x+y;
}

std::map<std::string,double> first = {
	{"x^2",1},
	{"y^2",2},
	{"z^2",1},
	{"r",-5},
	{"<",1}
};

std::map<std::string,double> second = {
	{"y",1},
	{">",1}
};