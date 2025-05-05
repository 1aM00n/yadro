#include "time.h"
#include <cstdio>
#include <string>

std::string Time::toString() const {
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, minutes);
    return buffer;
}

int Time::operator-(const Time& other) const {
    return (hours * 60 + minutes) - (other.hours * 60 + other.minutes);
}

bool Time::operator<(const Time& other) const {
    return (hours != other.hours) ? (hours < other.hours) : (minutes < other.minutes);
}

bool Time::operator>(const Time& other) const {
    return other < *this;
}

bool Time::operator<=(const Time& other) const {
    return !(other < *this);
}

bool Time::operator>=(const Time& other) const {
    return !(*this < other);
}

bool Time::operator==(const Time& other) const {
    return !(*this < other) && !(other < *this);
}

bool Time::operator!=(const Time& other) const {
    return (*this < other) || (other < *this);
}