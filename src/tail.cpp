#include <iostream>
#include <string>
#include <fstream>

 void tail () {    
    std::ifstream ifs("/var/log/khomp/k3l_intf.log");    
    if (ifs.is_open()) {
        std::string line;
        while (true)  {
            while (std::getline(ifs, line)) {
                std::cout << line << "\n";
            }
            if (!ifs.eof()) {
                break;
            }
            ifs.clear();        
        }
    }
}