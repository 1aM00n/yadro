#ifndef TIME_H
#define TIME_H

#include <string>
#include <cstdio> // Для snprintf

struct Time {
    int hours;
    int minutes;

    std::string toString() const;
    int operator-(const Time& other) const;
    bool operator<(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator>=(const Time& other) const;
    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;
};

#endif // TIME_H