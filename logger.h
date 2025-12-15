
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <string>
#include <memory> 

class Logger {
private:
    static std::unique_ptr<Logger> instance;
    static std::mutex init_mtx;
    static std::mutex write_mtx;
    std::ofstream logFile;

    Logger(const std::string& filename); 


public:
    ~Logger(); 

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    static Logger* getInstance(const std::string& filename="Test/output2.txt"); //

    void log(const std::string& message);

};

std::unique_ptr<Logger> Logger::instance;
std::mutex Logger::init_mtx;
std::mutex Logger::write_mtx;

Logger::Logger(const std::string& filename){ 
    logFile.open(filename, std::ios::trunc);
    if(logFile.is_open()){
        logFile << "[Init] Logger started." << std::endl;
    }
}

Logger:: ~Logger(){ 
    if(logFile.is_open()){
        logFile<<"[Shutdown] Logger closed." << std::endl;
        logFile.close();
    }
}

Logger* Logger::getInstance(const std::string& filename){ 
    if(!instance){
        std::lock_guard<std::mutex>lock(init_mtx);
        if(!instance){
            instance.reset(new Logger(filename));
        }
    }
    return instance.get();
}

void Logger::log(const std::string& message){
    std::lock_guard<std::mutex> lock(write_mtx);
    if(logFile.is_open()){
        logFile << message << std::endl;
    }
}