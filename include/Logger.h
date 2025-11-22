#pragma once
#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    Logger(const std::string &path);
    ~Logger();
    void log_line(const std::string &line);
private:
    std::ofstream ofs;
    std::mutex m;
};
