#include "logstream.h"
#include "logger.h"
#include <thread>


logstream::logstream(int line, const char pFile[], const char pFunc[], unafrog::log::LogLevel level) :
    mFile(pFile),
    mFunc(pFunc),
    mLevel(level),
    mLine(line)
{}

logstream::~logstream()
{
    std::stringstream arguments;
    std::thread::id tid = std::this_thread::get_id();

    arguments << mFile.substr(mFile.find_last_of('/') + 1) << ":"
              << mLine << " (" << mFunc << ") [" << tid << "] " << mOutputStream.str();

    logger::instance().propagate_log(mLevel, arguments.str());
}

logstream &logstream::appendSpace()
{
    mOutputStream << ' ';
    mOutputStream.flush();
    return *this;
}

