#include <exception>
#include <string>

class TokenException : public std::exception {
public:
    TokenException(const std::string& message) : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

