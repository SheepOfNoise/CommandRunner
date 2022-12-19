#ifndef C_LOGGER_H
#define C_LOGGER_H

#include <string>
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace std;

class CLogger
{
public:
	CLogger(const CLogger&) = delete;
	CLogger& operator=(const CLogger&) = delete;
	static CLogger* GetInstance()
	{
		if (instance_ == 0)
		{
			instance_ = new CLogger();
		}
		return instance_;
	}
	static void SetLogConfig(string log_file_prefix, int max_files = 30, int max_file_size = 1048576) { log_file_prefix_ = log_file_prefix; max_files_ = max_files; max_file_size_ = max_file_size; }
	std::shared_ptr<spdlog::logger> CreateLogger(string name);

private:
	std::vector<spdlog::sink_ptr> sinks_;
	static CLogger* instance_;
	static string log_file_prefix_;
	static int max_files_;
	static int max_file_size_;
	CLogger();
};

#endif
