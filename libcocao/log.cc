#include "log.h"

namespace libcocao {

const char* LogLevel::ToString(LogLevel::Level level) {
    switch (level) {

    #define XX(a, name) \
        case LogLevel::a: \
            return name;\
            break;

        XX(DEBUG, "DEBUG")
        XX(INFO, "INFO")
        XX(WARN, "WARN")
        XX(ERROR, "ERROR")
        XX(FATAL, "FATAL")
    #undef XX

        default:
            return "UNKNOW";
    }
}

LogLevel::Level LogLevel::FromString(const std::string &str) {
#define XX(level, s) \
    if(s == str){  \
        return LogLevel::level;  \
    }

    XX(DEBUG, "DEBUG")
    XX(INFO, "INFO")
    XX(WARN, "WARN")
    XX(ERROR, "ERROR")
    XX(FATAL, "FATAL")

#undef XX

    return LogLevel::UNKNOW;
}

class StringFormatItem: public LogFormatter::FormatItem {
public:
    StringFormatItem(const char str)
        : m_string(str){
    }
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) {
        os << m_string;
    }
private:
    char m_string;
};

class DateTimeSormatItem: public LogFormatter::FormatItem {
public:
    DateTimeSormatItem(const std::string& format = "%Y:%M:%d %H:%M:%S")
        : m_format(format){
        if (m_format.empty())
            m_format = "%Y-%M-%d %H:%M:%S";
    }
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof buf, m_format.c_str(), &tm);
        os << buf;
    }
private:
    std::string m_format;
};

class TabFormatItem:public LogFormatter::FormatItem {
public:
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) override {
        os << "\t";
    }
};

class NewLineFormatItem:public LogFormatter::FormatItem {
public:
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) override {
        os << std::endl;
    }
};

class EalpseFormatItem:public LogFormatter::FormatItem {
public:
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) {
        os << event->getElapse();
    }
};

class ThreadIdFormatItem:public LogFormatter::FormatItem {
public:
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) {
        os << event->getThreadId();
    }
};

class LevelFormatItem:public LogFormatter::FormatItem {
public:
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) {
        os << LogLevel::ToString(level);
    }
};

class FiberIdFormatItem:public LogFormatter::FormatItem {
public:
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) {
        os << event->getFiberId();
    }
};

class FileFormatItem:public LogFormatter::FormatItem {
public:
    void format(std::ostream& os, LogLevel::Level level, LogEvent::ptr event) {
        os << event->getFile();
    }
};

class LineFormatItem:public LogFormatter::FormatItem {
public:
    void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event) {
        os << event->getLine();
    }
};

class MessageFormatItem:public LogFormatter::FormatItem {
public:
    void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event) {
        os << event->getContent();
    }
};


LogEvent::LogEvent(LogLevel::Level level, const char *file, int32_t line, uint32_t elapse, uint32_t thread_id,
                   uint32_t fiber_id, uint64_t time, const std::string &threadname):
                     m_file(file)
                   , m_line(line)
                   , m_elapse(elapse)
                   , m_threadId(thread_id)
                   , m_fiberId(fiber_id)
                   , m_time(time)
                   , m_level(level)
                   , m_threadName(threadname){
}

void LogEvent::format(const char *fmt, va_list al) {
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, al);
    if (len != -1) {
        m_ss << std::string(buf, len);
        free(buf);
    }
}

void LogEvent::format(const char *fmt, ...) {
    va_list al;
    va_start(al, fmt);
    format(fmt, al);
    va_end(al);
}

LogFormatter::LogFormatter(const std::string &pattern)
    : m_pattern(pattern){
    init();
}
//    dTmTtTlTFTf:r
void LogFormatter::init() {
    for (size_t i = 0; i < m_pattern.size(); ++ i) {
        if (m_pattern[i] == 'T') {
            m_items.push_back(FormatItem::ptr(new TabFormatItem()));
        } else if (m_pattern[i] == 'd') {
            m_items.push_back(FormatItem::ptr(new DateTimeSormatItem()));
        } else if (m_pattern[i] == 'm') {
            m_items.push_back(FormatItem::ptr(new EalpseFormatItem()));
        } else if (m_pattern[i] == 't') {
            m_items.push_back(FormatItem::ptr(new ThreadIdFormatItem()));
        } else if (m_pattern[i] == 'l') {
            m_items.push_back(FormatItem::ptr(new LevelFormatItem()));
        } else if (m_pattern[i] == 'F') {
            m_items.push_back(FormatItem::ptr(new FiberIdFormatItem()));
        } else if (m_pattern[i] == 'f') {
            m_items.push_back(FormatItem::ptr(new FileFormatItem()));
        } else if (m_pattern[i] == 'r') {
            m_items.push_back(FormatItem::ptr(new LineFormatItem()));
        } else if (m_pattern[i] == 'C'){
            m_items.push_back(FormatItem::ptr(new MessageFormatItem()));
        } else {
            m_items.push_back(FormatItem::ptr(new StringFormatItem(m_pattern[i])));
        }
    }
    m_items.push_back(FormatItem::ptr(new NewLineFormatItem()));
}

std::string LogFormatter::format(LogLevel::Level level, LogEvent::ptr event) {
    std::stringstream ss;
    for (auto& it: m_items) {
        it->format(ss, level, event);
    }
    return ss.str();
}

Logger::Logger(const std::string &name)
    : m_name(name), m_level(LogLevel::DEBUG) {
    m_formatter.reset(new LogFormatter);
}

void Logger::addAppender (LoggerAppender::ptr appender) {
    if (!appender->getFormatter()) {
        appender->m_formatter = m_formatter;
    }
    m_appenders.push_back(appender);
}

void Logger::delAppender (LoggerAppender::ptr appender) {
    for (auto it = m_appenders.begin(); it != m_appenders.end(); ++ it)
        if (*it == appender) {
            m_appenders.erase(it);
            break;
        }
}

void Logger::clearAppenders () {
    m_appenders.clear();
}

void Logger::log(LogLevel::Level level, const LogEvent::ptr &event) {
    if (level >= m_level) {
        auto self = shared_from_this();
        if (!m_appenders.empty()) {
            for (auto & it: m_appenders) {
                it->log(level, event);
            }
        } else if (m_root) {
            m_root->log(level, event);
        }
    }
}

void Logger::debug(LogEvent::ptr event) {
    log(LogLevel::DEBUG, event);
}

void Logger::info(LogEvent::ptr event) {
    log(LogLevel::INFO, event);
}

void Logger::warn(LogEvent::ptr event) {
    log(LogLevel::WARN, event);
}

void Logger::error(LogEvent::ptr event) {
    log(LogLevel::ERROR, event);
}

void Logger::fatal(LogEvent::ptr event) {
    log(LogLevel::FATAL, event);
}


void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        std::cout << m_formatter->format(level, event);
    }
}

FileLogAppender::FileLogAppender(const std::string &filename)
    : m_filename(filename) {
    reopen();
}

bool FileLogAppender::reopen() {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename, std::ios::app);
    return !!m_filestream;
}

void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        uint64_t now = time(0);
        if (now != m_lastTime) {
            reopen();
            m_lastTime = now;
        }
        m_filestream << m_formatter->format(level, event);
    }
}

LogEventWarp::LogEventWarp(Logger::ptr logger, LogEvent::ptr event)
    : m_event(event)
    , m_logger(logger){
}

LogEventWarp::~LogEventWarp() {
    m_logger->log(m_event->getLevel(), m_event);
}

LoggerManager::LoggerManager() {
    m_root.reset(new Logger);
    m_root->addAppender(LoggerAppender::ptr(new StdoutLogAppender));
    m_loggers[m_root->m_name] = m_root;
//    init();
}

Logger::ptr LoggerManager::getLogger(const std::string &name) {
    auto it = m_loggers.find(name);
    if (it != m_loggers.end())
        return it->second;

    Logger::ptr logger(new Logger(name)) ;
    logger->m_root = m_root;
    m_loggers[name] = logger;
    return logger;
}




}