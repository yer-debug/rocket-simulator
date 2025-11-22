#include "Logger.h"

Logger::Logger(const std::string &path) {
    ofs.open(path, std::ios::out);
    if (ofs.fail()) {
        throw std::runtime_error("Failed to open log file");
    }
    ofs << "time(s),altitude(m),velocity(m/s),mass(kg),stage\n";
}

Logger::~Logger() {
    if (ofs.is_open()) ofs.close();
}

void Logger::log_line(const std::string &line) {
    std::lock_guard<std::mutex> lk(m);
    ofs << line << "\n";
}
