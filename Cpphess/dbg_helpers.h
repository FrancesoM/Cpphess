//
//  dbg_helpers.h
//  chess_engine
//
//  Created by Francesco Maio on 25/03/23.
//

#ifndef dbg_helpers_h
#define dbg_helpers_h


#define ERROR     0
#define INFO      1
#define DBG       2
#define DBGX      3

#define LOG_THRESHOLD INFO

// LOG(DBG_LVL) silent unless
void LOG(int level,const char *fmt, ...) {
    if( level <= LOG_THRESHOLD ) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

#endif /* dbg_helpers_h */
