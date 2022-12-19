#include "CLogger.h"
#include "spdlog/sinks/daily_file_sink.h"

CLogger* CLogger::instance_ = NULL;
string CLogger::log_file_prefix_ = "DefaultLogFile";
int CLogger::max_file_size_ = 1048576;
int CLogger::max_files_ = 30;

CLogger::CLogger() {
	sinks_.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	sinks_.push_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_file_prefix_, 0, 0, false, 14));
	spdlog::flush_every(std::chrono::seconds(10));
}

std::shared_ptr<spdlog::logger> CLogger::CreateLogger(std::string name)
{
	std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(name, begin(sinks_), end(sinks_));
	logger->set_level(spdlog::level::debug);

	return logger;
}
