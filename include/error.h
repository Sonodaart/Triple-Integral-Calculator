#ifndef _ERROR_LIB
#define _ERROR_LIB

/*========== Program info ==========*/
#define PROGRAM_NAME "Integral calculator"
#define PROGRAM_OUTPUT_NAME ("[" PROGRAM_NAME "]")

/*========== ANSII colouring ==========*/
#define RESET_COLOUR "\033[0m"
// killing (processes) colour
#define RED "\033[31m"
// error colour
#define BRIGHT_RED "\033[1;31m"
// warnings colour
#define BRIGHT_PURPLE "\033[1;35m"
// usage colour
#define YELLOW "\033[33m"

/*========== Standard errors ==========*/
#define FATAL_ERROR_LOG PROGRAM_OUTPUT_NAME << RED << " Fatal error:" << RESET_COLOUR
#define ERROR_LOG PROGRAM_OUTPUT_NAME << BRIGHT_RED << " Error:" << RESET_COLOUR
#define USAGE_LOG PROGRAM_OUTPUT_NAME << YELLOW << " Usage:" << RESET_COLOUR
#define WARNING_LOG PROGRAM_OUTPUT_NAME << BRIGHT_PURPLE << " Warning:" << RESET_COLOUR

#endif // end of library guardian