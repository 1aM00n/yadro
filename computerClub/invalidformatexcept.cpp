#include <stdexcept>

class InvalidFormatException : public std::runtime_error {
    public:
        // Конструктор с сообщением об ошибке
        explicit InvalidFormatException(const std::string& message, const int strNum)
            : std::runtime_error("Error in line number " + 
                            std::to_string(strNum) + 
                            ": " + message) {}
};