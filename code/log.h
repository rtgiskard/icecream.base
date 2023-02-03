#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H

#include <string>
#include <spdlog/spdlog.h>

// turn on/off logging statements at compile time
#define SPGLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

struct LogSettings {
	std::string name       = "main";
	std::string dir        = "./logs";
	std::string level_term = "info";
	std::string level_file = "trace";
	std::string pattern    = "[%Y.%m.%d %H:%M:%S.%e] [%^%L%$] %v";

	uint rotate_size    = 5 * 1024 * 1024;
	uint rotate_count   = 4;
	uint flush_interval = 1;
	bool sync           = true;
};

void init_log(const LogSettings & setting);

#endif
