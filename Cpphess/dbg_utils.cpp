//
//  dbg_utils.cpp
//  Cpphess
//
//  Created by Francesco Maio on 26/03/23.
//

#include "dbg_utils.hpp"

void LOG(int level,const char *fmt, ...) {
    if( level <= LOG_THRESHOLD ) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}
