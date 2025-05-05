#include "computerclub.h"
#include <string>
#include <iomanip>
#include <sstream>


ComputerClub::ComputerClub(int n, Time& start, Time& end, int price) :
 totalPlaces(n),
 start(start),
 end(end),
 price(price),
 wpProfitability(n, {0, 0}) {
    workplaces.resize(n, {Time{00,00}, ""});
}

void ComputerClub::logEvent(const Time time, int id, const std::string& message) {
    std::ostringstream sid;
    sid << id;
    eventLog.push(time.toString() + " " + sid.str()+ " " + message);
}

void ComputerClub::logEvent(const Time time, int id, const std::string& message, int placeID) {
    std::ostringstream sid, splaceID;
    sid << id;
    splaceID << placeID + 1;
    eventLog.push(time.toString()+ " " + sid.str()+ " " + message+ " " + splaceID.str());
}

void ComputerClub::arrivalClient(const std::string name, const Time& arrivalTime) {
    if (arrivalTime > this->start && arrivalTime < this->end){
        auto check = registeredClients.find(name); //смотрим пришел ли человечек
        if (check != registeredClients.end()) {
            logEvent(arrivalTime, 1, name);
            logEvent(arrivalTime, 13, "YouShallNotPass");
            return;
        }
        registeredClients.insert(name);
        logEvent(arrivalTime, 1, name);
    } else {
        logEvent(arrivalTime, 1, name);
        logEvent(arrivalTime, 13, "NotOpenYet");
    }
}

bool ComputerClub::occupyPlace(const std::string name, const int placeID, const Time& occupationTime) {
    if (occupationTime > this->start && occupationTime < this->end){
        auto check = registeredClients.find(name); //смотрим пришел ли человечек
        if (check == registeredClients.end()) {
            logEvent(occupationTime, 2, name, placeID);
            logEvent(occupationTime, 13, "ClientUnknown");
            return false;
        }

         // Проверка, не пытается ли клиент сесть на своё же место
        if (workplaces[placeID].second != "") {
            logEvent(occupationTime, 2, name, placeID);
            logEvent(occupationTime, 13, "PlaceIsBusy");
            return false;
        }

        // Если клиент уже сидел где-то - освобождаем предыдущее место и придется считать прибыль
        for (int i = 0; i<workplaces.size(); i++) {
            if (workplaces[i].second == name) {
                wpProfitability[i].first += (occupationTime - workplaces[i].first + 59) / 60; // Округление вверх
                wpProfitability[i].second += occupationTime - workplaces[i].first;
                workplaces[i].second = "";
                break;
            }
        }
        
        workplaces[placeID].second = name;
        workplaces[placeID].first = occupationTime;
        logEvent(occupationTime, 2, name, placeID);
       
    } else {
        logEvent(occupationTime, 2, name, placeID);
        logEvent(occupationTime, 13, "NotOpenYet");
        return false;
    }

    return true;
}

void ComputerClub::waitingPlace(std::string name, const Time& arrivalTime) {
    if (arrivalTime > this->start && arrivalTime < this->end){

        auto check = registeredClients.find(name); //смотрим пришел ли человечек
        if (check == registeredClients.end()) {
            logEvent(arrivalTime, 3, name);
            logEvent(arrivalTime, 13, "ClientUnknown");
            return;
        } // в тз про это не сказано но наличие этой проверки логично

        for (int i = 0; i<workplaces.size(); i++) {
            if (workplaces[i].second == "") {
                logEvent(arrivalTime, 3, name);
                logEvent(arrivalTime, 13, "ICanWaitNoLonger!");
                return;
            }
        }

        if (waitingQueue.size() < totalPlaces){
            waitingQueue.push(name);
            logEvent(arrivalTime, 3, name);
            return;
        } else {
            logEvent(arrivalTime, 3, name);
            logEvent(arrivalTime, 11, name);
            return;
        }

    } else {
        logEvent(arrivalTime, 3, name);
        logEvent(arrivalTime, 13, "NotOpenYet");
        return;
    }
}

void ComputerClub::deleteClient(std::string name, const Time& arrivalTime){
    if (arrivalTime > this->start && arrivalTime < this->end){

        auto check = registeredClients.find(name); //смотрим пришел ли человечек
        if (check == registeredClients.end()) {
            logEvent(arrivalTime, 4, name);
            logEvent(arrivalTime, 13, "ClientUnknown");
            return;
        }

        for (int i = 0; i<workplaces.size(); i++) {
            if (workplaces[i].second == name) {
                wpProfitability[i].first += (arrivalTime - workplaces[i].first + 59) / 60; // Округление вверх
                wpProfitability[i].second += arrivalTime - workplaces[i].first;
                workplaces[i].second = "";
                logEvent(arrivalTime, 4, name);
                registeredClients.erase(name);
                
                if (!waitingQueue.empty()){
                    std::string frQueue = waitingQueue.front();
                    waitingQueue.pop();

                    workplaces[i].first = arrivalTime;
                    workplaces[i].second = frQueue;
                    logEvent(arrivalTime, 12, frQueue, i+1);
                }     
                break;
            }
        }
    } else {
        logEvent(arrivalTime, 4, name);
        logEvent(arrivalTime, 13, "NotOpenYet");
        return;
    }
}

void ComputerClub::dayEnds(){
    std::cout << start.toString()<< "\n";

    for (const auto& name : registeredClients) {
        
        for (int i = 0; i<workplaces.size(); i++) {
            if (workplaces[i].second == name) {
                wpProfitability[i].first += (this->end - workplaces[i].first + 59) / 60; // Округление вверх
                wpProfitability[i].second += this->end - workplaces[i].first;
                break;
            }
        }  
        logEvent(this->end, 11, name);
    }

    
    std::queue<std::string> temp_log = eventLog; //чтобы логи не портить, вдруг куда еще
    while (!temp_log.empty()) {
        std::cout << temp_log.front() << "\n";
        temp_log.pop();
    }

    std::cout << end.toString()<< "\n";

    for (int i = 0; i<wpProfitability.size(); i++){
        Time frMin;
        frMin.hours = wpProfitability[i].second/60;
        frMin.minutes = wpProfitability[i].second%60;
        std::cout <<i+1 << " "<< wpProfitability[i].first*price << " "<<frMin.toString() << '\n';
    }
}