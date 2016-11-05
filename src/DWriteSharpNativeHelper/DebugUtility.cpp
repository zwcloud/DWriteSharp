#include "DebugUtility.h"

#if PRINT_TO_FILE
std::ofstream out("debug.txt", std::ios_base::app);
#endif