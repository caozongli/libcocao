#include "utils.h"

namespace libcocao {

pid_t GetThreadId() {
    return syscall(SYS_gettid);
}

}