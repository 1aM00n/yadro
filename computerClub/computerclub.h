#ifndef WORKPLACE_SYSTEM_H
#define WORKPLACE_SYSTEM_H


#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <queue>
#include <vector>
#include <set>

#include "time.h"

struct Client {
    std::string name;
    Time starTime;
};

struct LogEntry {
    Time timestamp;
    int id;
    std::string message;
};

class ComputerClub {
    private:
        int totalPlaces,
            price;
        Time start, 
             end;
        std::vector<std::pair<Time, std::string>> workplaces; // {занято, client_id}
        std::vector<std::pair<int, int>> wpProfitability; // {дохооод}
        std::queue<std::string> waitingQueue;
        std::set<std::string> registeredClients;
        std::queue<std::string> eventLog;
    
        void logEvent(const Time time, const int id, const std::string& message);
        void logEvent(const Time time, int id, const std::string& message, int placeID);
    
    public:
        ComputerClub(int n, Time& start, Time& end, int price);
        
        void arrivalClient(std::string name, const Time& arrival_time); //пришел
        bool occupyPlace(std::string name, int placeID, const Time& occupation_time); //занял место
        void waitingPlace(std::string name, const Time& release_time); //ждет
        void deleteClient(std::string name, const Time& arrival_time); //ушел
        void dayEnds();
    };



#endif // WORKPLACE_SYSTEM_H