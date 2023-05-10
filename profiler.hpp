#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>                      // for current PC time/date
#include "include/nlohmann/json.hpp"  // lib for handling JSON
#include <cstdio>                     // lib for deleting files
#include <sstream>                    // lib for handling time output
#include <cmath>
#include "include/logger.hpp"
#include "include/constants.hpp"
#include "include/color.hpp"

typedef long long unsigned int ll;

std::string getGracefulDoubleString(double str) {
    int precisionVal = 3;
    std::string trimmedString =
        std::to_string(str).substr(0, std::to_string(str).find(".") + precisionVal + 1);
    return trimmedString;
}

std::string STR_NO_DUPLICATION;

/**************************************************/
/******************** PROFILER ********************/
/**************************************************/
class Profiler {
public:
    Logger logger;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime, stopTime;
    std::chrono::duration<ll, std::nano> duration_time;
};

Profiler profiler;
double RES_NO_DUPLICATION;

/*************** Beginning of time measurement ***************/
#define START profiler.startTime = std::chrono::high_resolution_clock::now();
#define STARTP                                                      \
    profiler.startTime = std::chrono::high_resolution_clock::now(); \
    STR_NO_DUPLICATION = "Started...\n";                            \
    std::cout << changeColor(STR_NO_DUPLICATION);
#define START_PRINT_LINE STARTP
#define START_PRINT STARTP

/*************** End of time measurement ***************/
// Standart:
#define END                                                                        \
    profiler.stopTime = std::chrono::high_resolution_clock::now();                 \
    profiler.duration_time = std::chrono::duration_cast<std::chrono::nanoseconds>( \
        profiler.stopTime - profiler.startTime);                                   \
    RES_NO_DUPLICATION = (double)profiler.duration_time.count();                   \
    profiler.logger.log(RES_NO_DUPLICATION);
#define ENDP                                                                               \
    profiler.stopTime = std::chrono::high_resolution_clock::now();                         \
    profiler.duration_time = std::chrono::duration_cast<std::chrono::nanoseconds>(         \
        profiler.stopTime - profiler.startTime);                                           \
    RES_NO_DUPLICATION = (double)profiler.duration_time.count();                           \
    STR_NO_DUPLICATION =                                                                   \
        getGracefulDoubleString(RES_NO_DUPLICATION / NANOSECS_IN_MILISEC) + " milisecs\n"; \
    std::cout << changeColor(STR_NO_DUPLICATION);                                          \
    profiler.logger.log(RES_NO_DUPLICATION);                                               
    // std::cout << "Testing result has been written to a logger file" << '\n';
#define END_PRINT ENDP

// With comments:
#define END_WITH_COMMENT(str)                                                          \
    profiler.stopTime = std::chrono::high_resolution_clock::now();                     \
    profiler.duration_time = std::chrono::duration_cast<std::chrono::nanoseconds>(     \
        profiler.stopTime - profiler.startTime);                                       \
    RES_NO_DUPLICATION = (double)profiler.duration_time.count() / NANOSECS_IN_MILISEC; \
    \ profiler.logger.log(RES_NO_DUPLICATION, str);                                    \
    // std::cout << "Testing result has been written to a logger file" << '\n';
#define END_COMMENT(str) END_WITH_COMMENT(str)

#define ENDP_WITH_COMMENT(str)                                                         \
    profiler.stopTime = std::chrono::high_resolution_clock::now();                     \
    profiler.duration_time = std::chrono::duration_cast<std::chrono::nanoseconds>(     \
        profiler.stopTime - profiler.startTime);                                       \
    RES_NO_DUPLICATION = (double)profiler.duration_time.count() / NANOSECS_IN_MILISEC; \
    STR_NO_DUPLICATION = getGracefulDoubleString(RES_NO_DUPLICATION) + " milisecs\n";  \
    std::cout << changeColor(STR_NO_DUPLICATION);                                      \
    profiler.logger.log(RES_NO_DUPLICATION, str);                                      \
    // std::cout << "Testing result has been written to a logger file" << '\n';
#define END_PRINT_WITH_COMMENT(str) ENDP_WITH_COMMENT(str)
#define ENDP_COMMENT(str) ENDP_WITH_COMMENT(str)
#define END_PRINT_COMMENT(str) ENDP_WITH_COMMENT(str)

/*************** Printing result of time measurement ***************/
// Standart:
#define PRINT_TIME                                                                              \
    std::string ANS_NO_DUPLICATION_1 =                                                          \
        getGracefulDoubleString((double)profiler.duration_time.count() / NANOSECS_IN_MILISEC) + \
        " milisecs" + " | " + profiler.logger.comments.back() + '\n';                           \
    std::cout << changeColor(ANS_NO_DUPLICATION_1);
#define PRINT_TIME_IN_NANOSECS                                                                  \
    std::string ANS_NO_DUPLICATION_2 =                                                          \
        getGracefulDoubleString((double)profiler.duration_time.count()) + " nanosecs" + " | " + \
        profiler.logger.comments.back() + '\n';                                                 \
    std::cout << changeColor(ANS_NO_DUPLICATION_2);
#define PRINT_TIME_IN_MICROSECS                                                                  \
    STR_NO_DUPLICATION =                                                                         \
        getGracefulDoubleString((double)profiler.duration_time.count() / NANOSECS_IN_MICROSEC) + \
        " microsecs" + " | " + profiler.logger.comments.back() + '\n';                           \
    std::cout << changeColor(STR_NO_DUPLICATION);
#define PRINT_TIME_IN_SECS                                                                  \
    STR_NO_DUPLICATION =                                                                    \
        getGracefulDoubleString((double)profiler.duration_time.count() / NANOSECS_IN_SEC) + \
        " secs" + " | " + profiler.logger.comments.back() + '\n';                           \
    std::cout << changeColor(STR_NO_DUPLICATION);
#define PRINT_TIME_IN_MILISECS PRINT_TIME

// With comments (these are temporary comments):
#define PRINT_TIME_WITH_COMMENT(str)                                                            \
    STR_NO_DUPLICATION = "Comment: " + str + ":\n";                                             \
    std::cout << changeColor(STR_NO_DUPLICATION);                                               \
    STR_NO_DUPLICATION =                                                                        \
        getGracefulDoubleString((double)profiler.duration_time.count() / NANOSECS_IN_MILISEC) + \
        " milisecs\n";                                                                          \
    std::cout << changeColor(STR_NO_DUPLICATION);
#define PRINT_TIME_IN_NANOSECS_WITH_COMMENT(str)                                         \
    STR_NO_DUPLICATION = "Comment: " + str + ":\n";                                      \
    std::cout << changeColor(STR_NO_DUPLICATION);                                        \
    STR_NO_DUPLICATION =                                                                 \
        getGracefulDoubleString((double)profiler.duration_time.count()) + " nanosecs\n"; \
    std::cout << changeColor(STR_NO_DUPLICATION);
#define PRINT_TIME_IN_MICROSECS_WITH_COMMENT(str)                                                \
    STR_NO_DUPLICATION = "Comment: " + str + ":\n";                                              \
    std::cout << changeColor(STR_NO_DUPLICATION);                                                \
    STR_NO_DUPLICATION =                                                                         \
        getGracefulDoubleString((double)profiler.duration_time.count() / NANOSECS_IN_MICROSEC) + \
        " microsecs\n";                                                                          \
    std::cout << changeColor(STR_NO_DUPLICATION);
#define PRINT_TIME_IN_SECS_WITH_COMMENT(str)                                              \
    STR_NO_DUPLICATION = "Comment: " + str + ":\n";                                       \
    std::cout << changeColor(STR_NO_DUPLICATION);                                         \
    STR_NO_DUPLICATION =                                                                  \
        getGracefulDoubleString((double)profiler.duration_time.count() / NANOSECS_IN_SEC) \
        << " secs\n";                                                                     \
    std::cout << changeColor(STR_NO_DUPLICATION);
#define COMMENT(str) PRINT_TIME_WITH_COMMENT(str)

/*************** Profiler info ***************/
#define INFO                                                                          \
    STR_NO_DUPLICATION = "------- Simple fortov profiler -------\n";                  \
    std::cout << changeColor(STR_NO_DUPLICATION);                                     \
    STR_NO_DUPLICATION =                                                              \
        "Simple profiler for measuring execution time in .cpp programms;\nCommands: " \
        "START, END, ENDP(=ENDR=ENDF=ENDf), PRINT_TIME, PRINT_TIME_IN_MICROSECS, "    \
        "PRINT_TIME_IN_SECS\n";                                                       \
    std::cout << changeColor(STR_NO_DUPLICATION);                                     \
    STR_NO_DUPLICATION = "dev's email: ekfortov@edu.hse.ru";                          \
    std::cout << changeColor(STR_NO_DUPLICATION);

// STOPPED HERE
/*************** Profiler help ********* ******/
#define HELP                                                           \
    STR_NO_DUPLICATION = "Short overview of all commands:\n";          \
    std::cout << changeColor(STR_NO_DUPLICATION);                      \
    std::string LINE_NO_DUPLICATION;                                   \
    std::ifstream in_help_NO_DUPLICATION;                              \
    in_help_NO_DUPLICATION.open("include/help.txt");                   \
    if (in_help_NO_DUPLICATION.is_open()) {                            \
        while (getline(in_help_NO_DUPLICATION, LINE_NO_DUPLICATION)) { \
            std::cout << changeColor(LINE_NO_DUPLICATION) << '\n';     \
        }                                                              \
        in_help_NO_DUPLICATION.close();                                \
    } else {                                                           \
        STR_NO_DUPLICATION = "Failed to open help file";               \
        std::cerr << changeColor(STR_NO_DUPLICATION);                  \
    }

/*************** Profiler history ***************/
#define SHOW_ALL_LOGS profiler.logger.show_all_statistics();

/************ Profiler clear history ************/
#define CLEAR_LOGS profiler.logger.clear_logs();
#define CLEAR_ALL_FILE_LOGS profiler.logger.clear_log_files();
#define CLEAR_ALL_LOGS            \
    profiler.logger.clear_logs(); \
    profiler.logger.clear_log_files();

/* ANALYZER */
#define ANALYZE                                                       \
    STR_NO_DUPLICATION = "Use log files to analyze your code speed!"; \
    std::cout << changeColor(STR_NO_DUPLICATION);

#define CHANGE_DIMENSION(str)                   \
    if (strcmp(str, "NANOSECS") == 0) {         \
        profiler.logger.changeDimension(0);     \
    } else if (strcmp(str, "MICROSECS") == 0) { \
        profiler.logger.changeDimension(1);     \
    } else if (strcmp(str, "MILISECS") == 0) {  \
        profiler.logger.changeDimension(2);     \
    } else if (strcmp(str, "SECS") == 0) {      \
        profiler.logger.changeDimension(3);     \
    }
