//
// Created by Petru on 16.04.2019.
//

#ifndef NEWTON_RAPHSON_MATLAB_MEX_LOGGER_H
#define NEWTON_RAPHSON_MATLAB_MEX_LOGGER_H


#include <vector>
#include <fstream>
#include <string>

namespace Logger {

    class Logger {
        std::ofstream logFile;
    public:
        Logger() : logFile{"logger.txt", std::ios::app} {
            for(int i = 0 ;i<96;i++){
                logFile<<'_';
            }
            logFile << "\n";
        }

        explicit Logger(const char *name) : logFile{name} {
            logFile << "Logging File Created!\n\n\n";
        }

        void Write(const char *text) {
            logFile << text << "\n";
        };

        void Write(const std::string &text) {
            logFile << text << "\n";
        }
    };
}


#endif //NEWTON_RAPHSON_MATLAB_MEX_LOGGER_H
