#pragma once

#include <exception>
#include <string>

class SdlError : public std::exception {
public:
    SdlError(const std::string& sdlFunction);

    const char* what() const noexcept override;

private:
    std::string _message;
};

class ImgError : public std::exception {
public:
    ImgError(const std::string& sdlImageFunction);

    const char* what() const noexcept override;

private:
    std::string _message;
};

class TtfError : public std::exception {
public:
    TtfError(const std::string& sdlTtfFunction);

    const char* what() const noexcept override;

private:
    std::string _message;
};
