#include <fstream>
#include <filesystem>
#include <map>
#include <toml++/toml.h>

#include "config.h"

bool Config::load(const std::string & path) {

	using namespace std::string_view_literals;

	toml::table tbl;

	if (std::filesystem::is_regular_file(path))
		try {
			tbl = toml::parse_file(path);
		} catch (const toml::parse_error & err) {
			std::cout << err << std::endl;
			return false;
		}
	else
		std::cout << "* config not find, use default config" << std::endl;

	name    = tbl["name"].value_or("icecream"sv);
	version = tbl["version"].value_or(""sv);

	path_load = path;
	log.name  = name;

	// set with hard code default value
	log.dir        = tbl["log"]["dir"].value_or("./logs"sv);
	log.pattern    = tbl["log"]["pattern"].value_or("[%Y.%m.%d %H:%M:%S.%e] [%^%L%$] %v"sv);
	log.level_term = tbl["log"]["level_term"].value_or("info"sv);
	log.level_file = tbl["log"]["level_file"].value_or("trace"sv);

	log.rotate_size    = tbl["log"]["rotate_size"].value_or(10 * 1024 * 1024);
	log.rotate_count   = tbl["log"]["rotate_count"].value_or(1);
	log.flush_interval = tbl["log"]["flush_interval"].value_or(1);
	log.sync           = tbl["log"]["sync"].value_or(true);

	return true;
}

void Config::dump(const std::string & path) {
	auto tbl_log = toml::table{
		{"dir",            log.dir           },
		{"level_term",     log.level_term    },
		{"level_file",     log.level_file    },
		{"rotate_size",    log.rotate_size   },
		{"rotate_count",   log.rotate_count  },
		{"flush_interval", log.flush_interval},
		{"sync",           log.sync          },
		{"pattern",        log.pattern       },
	};

	auto tbl = toml::table{
		{"name",    name   },
		{"version", version},
		{"log",     tbl_log},
	};

	const auto path_out = (path == "" ? path_dump : path);

	auto logger = spdlog::default_logger();
	logger->info("dump config: {}", path_out);

	if (path_out == "-") {
		std::cout << "--------\n"
				  << "# config: " << path_load << "\n"
				  << tbl << "\n--------" << std::endl;
	} else {
		std::fstream s(path_out, s.binary | s.trunc | s.out);
		if (!s.is_open()) {
			std::cerr << "* failed to open: " << path_out << '\n';
		} else {
			s << tbl;
			s.close();
		}
	}
}

Config get_config_with_args(argparse::ArgumentParser & parser) {
	Config config;

	// op is verified on arg parse
	config.op = std::map<std::string, Operation>{
		{"dump", OP_DUMP_CONFIG},
		{"run",  OP_RUN        },
		{"test", OP_TEST       },
	}[parser.get<std::string>("op")];

	// load config from path given by args
	config.load(parser.get<std::string>("--config"));
	config.path_dump = parser.get<std::string>("-o");

	// override config with args
	if (parser.is_used("--log-dir"))
		config.log.dir = parser.get<std::string>("--log-dir");

	if (parser.is_used("--log-level"))
		config.log.level_term = parser.get<std::string>("--log-level");

	return config;
}

//	== 配置文件与命令行参数的处理逻辑
//
//	- 目标优先级：
// 	命令行参数 > 配置文件 > 硬编码
//
//	- 实际处理逻辑：
//	1. 读取配置文件: as_T() 检查配置信息是否存在（部分参数不检查，提供默认值）
//		. 存在则赋值（覆盖硬编码的值）
//		. 不存在则跳过（使用硬编码的初始值）
//
//	2. 参数解析：is_used() 检查参数是否声明（部分仅从命令行获取的选项除外）
// 		. 有声明则使用命令行参数覆盖
// 		. 无声明则跳过（命令行默认值被忽略）
//
//	- 理想处理逻辑：
// 	1. 命令行参数有声明：使用声明值
//
// 	2. 命令行参数未声明（有默认值）
// 		. 配置文件中存在：使用配置文件的值
//		. 配置文件中不存在：使用参数默认值
//
// 	3. 命令行参数未声明（无默认值）
// 		. 配置文件中存在：使用配置文件的值
//		. 配置文件中不存在：使用硬编码的值
//
//	- 说明：为简化处理，避免多次检查：
//	1. 默认值在初始化或读配置文件时通过硬编码给出
//	2. 命令行参数的默认值与硬编码的默认值保持一致，只作提示
