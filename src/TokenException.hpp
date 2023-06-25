#include <exception>
#include <string>

class TokenException : public std::exception
{
	private:
	    std::string m_message;
	public:
	    TokenException(const std::string& message) : m_message(message) {}

	    const char* what() const noexcept override { return message_.c_str(); }
};
