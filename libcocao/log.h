#ifndef __LIBCOCAO_LOG_H
#define __LIBCOCAO_LOG_H

#include <iostream>
#include <string>
#include <stdint.h>
#include <memory>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <list>
#include <map>
#include <time.h>
#include "singleton.h"
#include "utils.h"
#include <stdio.h>

#define LIBCOCAO_LOG_LEVEL(logger, level) \
    if (logger->getLevel() <= level)     \
        libcocao::LogEventWarp(logger, libcocao::LogEvent::ptr(new libcocao::LogEvent(level, __FILE__, __LINE__, 0, libcocao::GetThreadId(), \
                0, time(0), "system"))).getSS()

#define LIBCOCAO_LOG_NAME(name) libcocao::LoggerMgr::GetInstance()->getLogger(name)
#define LIBCOCAO_LOG_ROOT() libcocao::LoggerMgr::GetInstance()->getRoot()

#define LIBCOCAO_LOG_DEBUG(logger) LIBCOCAO_LOG_LEVEL(logger, libcocao::LogLevel::DEBUG)
#define LIBCOCAO_LOG_INFO(logger)  LIBCOCAO_LOG_LEVEL(logger, libcocao::LogLevel::INFO)
#define LIBCOCAO_LOG_WARN(logger)  LIBCOCAO_LOG_LEVEL(logger, libcocao::LogLevel::WARN)
#define LIBCOCAO_LOG_ERROR(logger) LIBCOCAO_LOG_LEVEL(logger, libcocao::LogLevel::ERROR)
#define LIBCOCAO_LOG_FATAL(logger) LIBCOCAO_LOG_LEVEL(logger, libcocao::LogLevel::FATAL)



namespace libcocao {

class Logger;
class LoggerManager;

class LogLevel {
public:
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* ToString(LogLevel::Level level);
    static LogLevel::Level FromString(const std::string& str);
};

class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(LogLevel::Level level
            , const char* file, int32_t line
            , uint32_t elapse, uint32_t thread_id
            , uint32_t fiber_id, uint64_t time
            , const std::string& threadname);

    const char* getFile() const { return m_file; } //文件名
    int32_t getLine() const {return m_line;	}	//行号
    uint32_t getElapse() const { return m_elapse; }	//程序启动开始到现在的毫秒数
    int32_t getThreadId() const { return m_threadId; } //线程id
    uint32_t getFiberId() const { return m_fiberId; } //协程号
    uint64_t getTime() const { return m_time; } //时间戳
    std::string getContent() const { return m_ss.str(); } //内容
    LogLevel::Level getLevel() const { return m_level; }
    std::string getThreadName() const { return m_threadName; }

    std::stringstream& getSS() {return m_ss; }
    void format(const char* fmt, va_list al);
    void format(const char* fmt, ...);

private:
    const char* m_file = nullptr; 	//文件名
    int32_t m_line = 0;				//行号
    uint32_t m_elapse = 0;			//程序启动开始到现在的毫秒数
    int32_t m_threadId = 0;			//线程id
    uint32_t m_fiberId = 0; 		//协程号
    uint64_t m_time = 0;			//时间戳
    std::stringstream m_ss;			//内容
    LogLevel::Level m_level;
    std::string m_threadName;

};

// 日志格式器
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern = "dTmTtTlTFTf:rTC");
    std::string format(LogLevel::Level level, LogEvent::ptr event);
public:
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) = 0;
        virtual ~FormatItem(){};
    };

    void init();
    bool isError() const {return m_error;}
    const std::string getPattern() const {return m_pattern;}

private:
    std::string m_pattern;
    bool m_error = false;
    std::vector<FormatItem::ptr> m_items;
};


//日志输出地
class LoggerAppender: public std::enable_shared_from_this<LoggerAppender> {
    friend class Logger;
public:
    typedef std::shared_ptr<LoggerAppender> ptr;
    virtual ~LoggerAppender() {}
    virtual void log(LogLevel::Level, LogEvent::ptr event) = 0;

    void setLevel (LogLevel::Level level) { m_level = level; }
    LogLevel::Level getLevel () const { return m_level; }
    void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
    LogFormatter::ptr getFormatter() { return m_formatter; }

protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;

};

//日志器
class Logger: public std::enable_shared_from_this<Logger> {
    friend class LoggerManager;
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, const LogEvent::ptr& event) ;

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    const std::string& getName() const { return m_name; }
    LogLevel::Level getLevel() const { return m_level; }
    LogFormatter::ptr getFormatter() {return m_formatter; }
    void addAppender (LoggerAppender::ptr der);
    void delAppender (LoggerAppender::ptr appender);
    void clearAppenders ();
private:
    std::string m_name; //日志名称
    LogLevel::Level m_level; //日志级别
    LogFormatter::ptr m_formatter;
    std::list<LoggerAppender::ptr> m_appenders;
    Logger::ptr m_root;
};

class StdoutLogAppender:public LoggerAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(LogLevel::Level level, LogEvent::ptr event) override;
};

class FileLogAppender: public LoggerAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(LogLevel::Level level, LogEvent::ptr event) override;
    bool reopen();

private:
    std::string m_filename;
    std::ofstream m_filestream;
    uint64_t m_lastTime;
};

class LogEventWarp {
public:
    LogEventWarp(Logger::ptr logger, LogEvent::ptr event);
    ~LogEventWarp();

    std::stringstream& getSS() { return m_event->getSS(); }

private:
    LogEvent::ptr m_event;
    Logger::ptr m_logger;
};

class LoggerManager {
public:
    LoggerManager();
    Logger::ptr getLogger(const std::string& name);

//    void init();
    Logger::ptr getRoot() const { return m_root; }

private:
    std::map<std::string, Logger::ptr> m_loggers;
    Logger::ptr m_root;
};

typedef libcocao::Singleton<LoggerManager> LoggerMgr;

}


#endif
