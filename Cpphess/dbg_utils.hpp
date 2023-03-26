//
//  dbg_utils.hpp
//  Cpphess
//
//  Created by Francesco Maio on 26/03/23.
//

#ifndef dbg_utils_hpp
#define dbg_utils_hpp

#include <stdio.h>
#include <stdarg.h>

#define ERROR     0
#define INFO      1
#define DBG       2
#define DBGX      3

#define LOG_THRESHOLD INFO

// LOG(DBG_LVL) silent unless
void LOG(int level,const char *fmt, ...);

#endif /* dbg_utils_hpp */
