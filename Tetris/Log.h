#pragma once
#include <iostream>
#include <sstream>



class EnLogger
{
public:
	enum  LogLevel { LevelError, LevelWarning, LevelInfo };
	
private:	

	LogLevel m_LogLevel;
	
public:
	EnLogger(LogLevel lvl = LevelInfo): m_LogLevel(lvl) {}
	~EnLogger() {};
	
	

	void setLevel(LogLevel level) {
		m_LogLevel = level;
	};
	void error(std::stringstream msg)
	{
		std::cout << "<<ERROR>> :" << msg.str() << std::endl;
	};
	static void warn(const std::stringstream& stream)
	{
		std::cout << "<<WARNING>> :" << stream.str() << std::endl;
	};
	static void info(const std::stringstream& stream)
	{
		std::cout << "<<INFO>> :" << stream.str() << std::endl;
	};
	/*
	cLog& operator << (std::string data);
	cLog& operator << (uint32_t data);
	cLog& operator << (uint16_t data);
	cLog& operator << (uint8_t data);
	cLog& operator << (int32_t data);
	cLog& operator << (int16_t data);
	cLog& operator << (int8_t data);

	cLog& operator >> (std::string& data);
	cLog& operator >> (uint32_t& data);
	cLog& operator >> (uint16_t& data);
	cLog& operator >> (uint8_t& data);
	cLog& operator >> (int32_t& data);
	cLog& operator >> (int16_t& data);
	cLog& operator >> (int8_t& data);
	*/
};

