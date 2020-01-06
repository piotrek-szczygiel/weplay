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

    static std::string string(std::string section, std::string name, std::string default_value = {})
    {
        return instance().m_reader.Get(section, name, default_value);
    }

    static int integer(std::string section, std::string name, int default_value = 0)
    {
        return instance().m_reader.GetInteger(section, name, default_value);
    }

    static float real(std::string section, std::string name, float default_value = 0.0F)
    {
        return instance().m_reader.GetFloat(section, name, default_value);
    }

    static int boolean(std::string section, std::string name, bool default_value = false)
    {
        return instance().m_reader.GetBoolean(section, name, default_value);
    }

private:
    Config() { }

    INIReader m_reader { "config.ini" };
};
