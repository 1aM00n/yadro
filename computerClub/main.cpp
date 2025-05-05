#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <regex>

#include "computerclub.h"
#include "time.h"
#include "invalidformatexcept.cpp"


bool checkHorsMinutesRange(const std::string& timePart) { //проверка что минуты минуты и часы часы)
    int hours = std::stoi(timePart.substr(0, 2));
    int min = std::stoi(timePart.substr(3, 2));
    
    return (hours >= 0 && hours <= 23) && (min >= 0 && min <= 59);
}

void checkTimeForm(const std::string& str, int strNum) { //смотрим что строка 2 подходит
    static const std::regex timeRegex(R"(^\d{2}:\d{2} \d{2}:\d{2}$)");
    if (!std::regex_match(str, timeRegex) ||
        !checkHorsMinutesRange(str.substr(0, 5)) || // Первое время
        !checkHorsMinutesRange(str.substr(6, 5)) // Второе время
        ) {
        throw InvalidFormatException(str + " (expected format: \'hours:minutes hours:minutes\')\n", strNum);
    }
}

void eventTimeForm(const std::string& str, int strNum){
    static const std::regex timeRegex(R"(^\d{2}:\d{2}$)");
    if(!std::regex_match(str, timeRegex) ||
       !checkHorsMinutesRange(str)){
        throw InvalidFormatException(str + " (expected format: \'hours:minutes\')\n", strNum);
    }
}

int main(int argc, char* argv[]){
    
    unsigned int sCounter = 0, //подсчет строк
                 compNum, //кол-во столов
                 perHour; 
    Time start, 
         end; 
    
    std::string s; // сюда будем класть считанные строки
    std::ifstream file(argv[1]); // файл из которого читаем 

    if(!file.is_open()){
        std::cout << "Error opening file!" << '\n';
        return 1;
    }

    try{

        getline(file, s); //сначала количество столов
        sCounter++;
        for(char c: s){
            if (!(std::isdigit(c))){
                throw InvalidFormatException(s + " (unsigned int needed)", sCounter);
            }
        }
        compNum = std::stoi(s);

        getline(file, s); // теперь начало и конец рабочего дня
        sCounter++;
        checkTimeForm(s, sCounter); // проверяем строку
        start.hours = std::stoi(s.substr(0, 2));
        start.minutes = std::stoi(s.substr(3, 2));
        end.hours = std::stoi(s.substr(6, 2));
        end.minutes = std::stoi(s.substr(9, 2));

        
        getline(file, s); //стоимость стола в час
        sCounter++;
        for(char c: s){
            if (!(std::isdigit(c))){
                throw InvalidFormatException(s + " (unsigned int needed)", sCounter);
            }
        }
        perHour = std::stoi(s);
        
        ComputerClub club(compNum, start, end, perHour);
        bool firstTime = true;
        Time when, 
             after;
        std::string str,
                    who;
        int id,
            where;

        while(getline(file, s)){ //дальше только события
            sCounter++;
            std::stringstream ss(s);
            
            //берем время ивента
            getline(ss, str, ' '); 
            eventTimeForm(str, sCounter); //смотрим что там точно время
            when.hours = std::stoi(str.substr(0, 2));
            when.minutes = std::stoi(str.substr(3, 2));
            if (firstTime){
                after = when;
            } else {
                if(after>when){
                    throw InvalidFormatException(s + " (time sequence error) " + after.toString() + " > " + when.toString(), sCounter);
                } else {
                    after = when;
                }
            }

            //берем id случившегося
            getline(ss, str, ' ');
            if (str.length() == 1 && std::isdigit(str[0])){
                id = std::stoi(str);
            } else {
                throw InvalidFormatException(s + " (event id was expected)", sCounter);
            }

            //берем имя
            getline(ss, str, ' ');
            std::regex nameRegex("^[a-z0-9_-]+$");
            if(!std::regex_match(str, nameRegex)){
                throw InvalidFormatException(s + " (body can contain only a..z, 0..9, _, -)", sCounter);
            }
            who = str;

            switch(id){

                case 1:
                    club.arrivalClient(who, when);
                    break;

                case 2:
                    getline(ss, str, ' ');
                    for(char c: str){
                        if (!(std::isdigit(c))){
                            throw InvalidFormatException(s + " (unsigned int needed)", sCounter);
                        }
                    }
                    where = std::stoi(str);
                    if (where > compNum || where==0){
                        throw InvalidFormatException(s + " (The club doesn't have that many computers)", sCounter);
                    }
                    club.occupyPlace(who, where-1, when);
                    break;

                case 3:
                    club.waitingPlace(who, when);
                    break;

                case 4:
                    club.deleteClient(who, when);
                    break;

                default:
                    throw InvalidFormatException(s + " (unknown event id)", sCounter);
                    break;
            }
            firstTime = false;
        }

        club.dayEnds();
    }
    catch(const InvalidFormatException& e){
        std::cerr << e.what() << '\n';
        return 1;
    }

    file.close(); // обязательно закрываем файл что бы не повредить его
    return 0;
}