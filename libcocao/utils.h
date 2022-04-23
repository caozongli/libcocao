#ifndef __LIBCOCAO_UTILS_H_
#define __LIBCOCAO_UTILS_H_

#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace libcocao {
pid_t GetThreadId();


}

#endif