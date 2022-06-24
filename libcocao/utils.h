#ifndef __LIBCOCAO_UTILS_H_
#define __LIBCOCAO_UTILS_H_

#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <vector>
#include <string>

namespace libcocao {
pid_t GetThreadId();
uint32_t GetFiberId();

//时间ms
uint64_t GetCurrentMS();
uint64_t GetCurrentUS();


}

#endif