#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>              // for current PC time/date
#include "nlohmann/json.hpp"  // lib for handling JSON
#include <cstdio>             // lib for deleting files
#include <sstream>            // lib for handling time output
#include <cmath>
#include "constants.hpp"
#include "color.hpp"

std::string STR_NO_DUPLICATION_Q;

struct Data {
    std::string start_time = "start_time";
    std::string end_time = "end_time";
    std::string logs = "logs";
    std::string comments = "comments";
};

bool is_empty(std::ifstream& pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

std::string getTimeString(time_t timestamp) {
    struct tm* timeinfo = localtime(&timestamp);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

/**************************************************/
/********************* LOGGER *********************/
/**************************************************/
class Logger {
public:
    std::vector<int64_t>
        logs;  // measurement results are stored here  // stored in nanosecs always!
    std::vector<std::string> comments;
    int8_t format = 0;  // 0 => nanosecs; 1 => microsecs; 2 => milisecs; 3 => secs
    std::time_t start_time, end_time;

    Logger() {
        auto current_time = std::chrono::system_clock::now();
        start_time = std::chrono::system_clock::to_time_t(current_time);
    }

    void changeDimension(int8_t format) {
        this->format = format;
    }

    void log(const int64_t& nanoseconds) {
        logs.push_back(nanoseconds);
        comments.push_back("-");
    }

    void log(const int64_t& nanoseconds, const std::string& comment) {
        logs.push_back(nanoseconds);
        comments.push_back(comment);
    }

    int64_t getMax() {
        return *max_element(std::begin(logs), std::end(logs));
    }

    int64_t getMin() {
        return *min_element(std::begin(logs), std::end(logs));
    }

    void clear_logs() {
        this->logs.clear();
    }

    void clear_log_files() {
        std::remove("log.txt");
        std::remove("log.csv");
        std::remove("log.json");
    }

    void show_all_statistics() {
        STR_NO_DUPLICATION_Q = "ALL LOGS: \n";
        std::cout << changeColor(STR_NO_DUPLICATION_Q);
        for (int64_t& elem : logs) {
            STR_NO_DUPLICATION_Q = std::to_string(elem);
            std::cout << changeColor(STR_NO_DUPLICATION_Q) << '\n';
        }
        STR_NO_DUPLICATION_Q = "Those are all logs!\n";
        std::cout << changeColor(STR_NO_DUPLICATION_Q);
    }

    ~Logger() {
        if (logs.empty()) {
            return;
        }
        auto current_time = std::chrono::system_clock::now();
        end_time = std::chrono::system_clock::to_time_t(current_time);

        std::ofstream out_txt, out_json, out_csv;
        const std::string file_txt = "log.txt";
        const std::string file_csv = "log.csv";
        out_txt.open(file_txt, std::ios::app);  // open for additional writing to a file (previous
                                                // data in file remains)
        out_csv.open(file_csv, std::ios::app);  // -||-

        switch (format) {
            case 1:  // microsecs
                for (int i = 0; i < logs.size(); ++i) {
                    logs[i] /= NANOSECS_IN_MICROSEC;
                }
                break;
            case 2:  // milisecs
                for (int i = 0; i < logs.size(); ++i) {
                    logs[i] /= NANOSECS_IN_MILISEC;
                }
                break;
            case 3:  // secs
                for (int i = 0; i < logs.size(); ++i) {
                    logs[i] /= NANOSECS_IN_SEC;
                }
                break;
        }

        // TXT output (supports SEVERAL launches of program)
        // output WITH comments
        if (out_txt.is_open()) {
            out_txt << "--------------------------------------------------" << '\n';
            out_txt << "Profiler started at: " << std::ctime(&start_time);
            out_txt << "Profiler ended at: " << std::ctime(&end_time);
            out_txt << "----- LOGS -----\n";
            switch (format) {
                case 0:
                    out_txt << "DIMENSION: nanosecs\n";
                    break;
                case 1:
                    out_txt << "DIMENSION: microsecs\n";
                    break;
                case 2:
                    out_txt << "DIMENSION: milisecs\n";
                    break;
                case 3:
                    out_txt << "DIMENSION: secs\n";
                    break;
                default:
                    out_txt << "DIMENSION: unknown\n";
            }
            for (size_t i = 0; i < logs.size(); ++i) {
                out_txt << logs[i] << ' ' << comments[i] << '\n';
            }
            out_txt << "----- STATS -----\n";
            out_txt << "Min value: " << getMin() << " ; Max value: " << getMax() << '\n';
            out_txt << "--------------------------------------------------\n\n";
            out_txt.close();
        } else {
            STR_NO_DUPLICATION_Q = "FAILED to open " + file_txt + " file\n";
            std::cerr << changeColor(STR_NO_DUPLICATION_Q);
        }

        // CSV output (supports SEVERAL launches of program)
        // output WITHOUT comments
        if (out_csv.is_open()) {
            for (int64_t& elem : logs) {
                out_csv << elem << ';';
            }
            out_csv << '\n';
            out_csv.close();
        } else {
            STR_NO_DUPLICATION_Q = "FAILED to open " + file_csv + " file\n";
            std::cerr << changeColor(STR_NO_DUPLICATION_Q);
        }

        // JSON output (supports SEVERAL launch of program)
        // output WITH comments
        const std::string file_json = "log.json";
        std::string str_logs, str_comments;
        for (int i = 0; i < logs.size(); ++i) {
            str_logs += std::to_string(logs[i]) + ", ";
            str_comments += comments[i] + ", ";
        }

        Data new_data = {getTimeString(start_time), getTimeString(end_time), str_logs,
                         str_comments};
        std::ifstream file_in(file_json);
        if (is_empty(file_in)) {
            std::ofstream out;
            out.open(file_json);
            if (out.is_open()) {
                out << "[]" << std::endl;
            }
            out.close();
        }
        file_in.close();
        file_in.open(file_json);
        nlohmann::json j_in;
        file_in >> j_in;
        for (size_t i = 0; i < logs.size(); ++i) {
            j_in.push_back({// Add new object to JSON array
                            {"start_time", new_data.start_time},
                            {"end_time", new_data.end_time},
                            {"logs", new_data.logs},
                            {"comments", new_data.comments}});
        }

        file_in.close();

        // New json is writed to file
        std::ofstream file_out2(file_json);
        file_out2 << j_in.dump(4);
        file_out2.close();
    }
};
// LOGGER ended
