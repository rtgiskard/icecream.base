#include <argparse/argparse.hpp>
#include <fmt/ranges.h>

#define CHOICE_LOG_LEVELS \
	{ "trace", "debug", "info", "warning", "error", "critical", "off" }
#define CHOICE_OPERATIONS \
	{ "dump", "run", "test" }

template <typename T = std::string>
T verify_choice(const T & value, const std::vector<T> & choices = {},
                const std::string msg = "invalid choice") {
	if (std::find(choices.begin(), choices.end(), value) == choices.end())
		throw std::runtime_error(fmt::format("* {}: \"{}\" not in {}", msg, value, choices));

	return value;
}

argparse::ArgumentParser parse_args(int argc, char ** argv) {
	argparse::ArgumentParser parser("icecream", "0.1");

	using namespace std::string_literals;

	parser.add_argument("-c", "--config")
		.default_value("data/config.toml"s)
		.metavar("CONFIGFILE")
		.help("load config file");

	parser.add_argument("--log-dir")
		.default_value("./logs"s)
		.metavar("DIR")
		.help("directory to hold all logs");

	parser.add_argument("--log-level")
		.default_value("info"s)
		.metavar("LEVEL")
		.help(fmt::format("log level for stdout: {}", std::vector(CHOICE_LOG_LEVELS)))
		.action([](const std::string & value) {
			return verify_choice(value, CHOICE_LOG_LEVELS, "--log-level");
		});

	parser.add_argument("-o", "--output")
		.default_value("-"s)
		.metavar("OUTPUT")
		.help("dump config to the output, write stdout if '-'");

	parser.add_argument("op")
		.help(fmt::format("operations: {}", std::vector(CHOICE_OPERATIONS)))
		.action([](const std::string & value) {
			return verify_choice(value, CHOICE_OPERATIONS, "op");
		});

	try {
		parser.parse_args(argc, argv);
	} catch (const std::runtime_error & err) {
		if (!parser.is_used("op"))
			std::cerr << "* missing args: 'op'" << std::endl;
		else
			std::cerr << err.what() << std::endl;
		std::exit(1);
	}

	return parser;
}

// 笔记
// 1. 参数类型由 parser.get<T>("--arg") 显式指明
// 2. 没有 default_value 属性时，get() 会导致异常退出
// 3. get<T>(..) 与 nargs 不匹配时，仍以 T 为准
