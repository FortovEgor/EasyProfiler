# EasyProfiler
A light-weight, easy-to-import &amp; -use profiler for C++ programs.

### API
Just download `profiler.hpp` file & folder `include` and write the following line: 
```
#include "profiler.hpp"
```

### Instructions
All instructions are written in `include\help.txt` file. Please, read them carefully!

### Test code:
```
#include <iostream>
#include <pthread.h>  // c lib for posix threads
#include <cstdio>
#include "profiler.hpp"  // handwritten simple profiler

#define CYCLES 1'000'000'000

void* func(void* arg1) {
    int temp = 0;
    for (int i = 0; i < CYCLES; ++i) {
        temp++;
    }
    std::cout << temp << "\n";
    return nullptr;
}

void* func2(void* arg1) {
    int temp = 0;
    for (int i = 0; i < CYCLES; ++i) {
        temp++;
    }
    std::cout << temp << "\n";
    return nullptr;
}


int main(void) {
    pthread_t th1, th2;
    
    START;
    pthread_create(&th1, NULL, &func, NULL);
    pthread_create(&th2, NULL, &func2, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    ENDP_WITH_COMMENT("hahahjdhhd");

    PRINT_TIME
    PRINT_TIME_IN_NANOSECS

    CLEAR_ALL_LOGS;
    CLEAR_ALL_FILE_LOGS;
    HELP;
    START;
    func(nullptr);
    func2(nullptr);
    ENDP;


    return 0;
}
```

NOTE: A new version of thread-safe profiler (based on current) with custom logs is now a Sber's property (NOT open-source anymore).
