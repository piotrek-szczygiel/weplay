#pragma once
#include <inih.h>

class Config {
public:
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    static Config& instance()
    {
        static Config instance;
        return instance;
    }

    bool load() { return m_reader.ParseError() == 0; }

    static std::string string(std::string section, std::string name, std::string default = {})
    {
        return instance().m_reader.Get(section, name, default);
    }

    static int integer(std::string section, std::string name, int default = 0)
    {
        return instance().m_reader.GetInteger(section, name, default);
    }

    static int boolean(std::string section, std::string name, bool default = false)
    {
        return instance().m_reader.GetBoolean(section, name, default);
    }

private:
    Config() { }

    INIReader m_reader { "config.ini" };
};
