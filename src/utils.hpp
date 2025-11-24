#pragma once

#define NotImplemented() throw NotImplementedError(__PRETTY_FUNCTION__)

class NotImplementedError : public std::logic_error {
   public:
    NotImplementedError()
        : NotImplementedError("Not implemented", __FUNCTION__) {}
    NotImplementedError(const char* function)
        : NotImplementedError("Function Not implemented =>", function) {}

    virtual const char* what() const throw() { return __text.c_str(); }

   private:
    NotImplementedError(const char* message, const char* function)
        : std::logic_error("Not Implemented") {
        __text = message;
        __text += " : ";
        __text += function;
    }

    std::string __text;
};

