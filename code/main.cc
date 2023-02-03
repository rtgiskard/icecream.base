#include <spdlog/spdlog.h>

#include "main.h"

int main(int argc, char ** argv) {

	auto parser = parse_args(argc, argv);
	auto config = get_config_with_args(parser);

	init_log(config.log);
	auto logger = spdlog::default_logger();

	logger->info("========");

	switch (config.op) {
	case OP_RUN:
		logger->info("mode: OP_RUN");
		break;
	case OP_TEST:
		logger->info("mode: OP_TEST");
		break;
	case OP_DUMP_CONFIG:
		config.dump();
		break;
	default:
		logger->info("nothing to do");
	}

	return 0;
}
