// Author: Gabriele Fazio 994848
// Contact: gabriele.fazio@studenti.unimi.it
// Date: 30/01/2023
// Functionality: Program that calculates triple integrals over a domain
// specified. For more info refer to
// https://github.com/Sonodaart/Triple-Integral-Calculator/blob/main/README.md

#include <iostream>

#include "../include/error.h"
#include "../include/linker.h"
#include "../include/math3D.h"

// function to check if a string is a double or integer
int isDouble(const char *array);
int isInteger(const char *array);

// functions to load values into variables
void loadDouble(const char*, double&, const std::string&);
void loadInteger(const char*, int&, const std::string&);