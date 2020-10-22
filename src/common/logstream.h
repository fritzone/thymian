#ifndef _LOGSTREAM_H_
#define _LOGSTREAM_H_

#ifdef QT_VERSION
#include <QString>
#endif

#include "logtypes.h"

#include <common.h>

#include <sstream>
#include <string>
#include <iomanip>
#include <type_traits>

class logstream
{
public:

    logstream() = delete;
    logstream(int line, const char pFile[], const char pFunc[], unafrog::log::LogLevel level);

    virtual ~logstream();

    template<class T>
    logstream &operator<<(T t)
    {
        const bool is_bool = std::is_same<T,bool>::value;
        std::string out = is_bool?(t ? "true" : "false") : unafrog::utils::to_string(t);
        mOutputStream << out;
        return appendSpace();
    }

    logstream &operator<<(void* t)
    {
        mOutputStream << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << t;
        return appendSpace();
    }

    logstream &operator<<(const char* t)
    {
        mOutputStream << t;
        return appendSpace();
    }

    logstream &operator<<(const std::string& t)
    {
        return operator<<(t.c_str());
    }

#ifdef QT_VERSION
    logstream &operator<<(const QString& t)
    {
        return operator<<(t.toUtf8().constData());
    }
#endif

private:
    logstream &operator=(const logstream &rOther);
    logstream(const logstream &rOther);

    logstream &appendSpace();

private:
    std::stringstream mOutputStream;
    std::string mFile;
    std::string mFunc;
    unafrog::log::LogLevel mLevel;
    int mLine;
};

#endif  // logstream_H
