#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

#include <string>
#include <map>
#include <argparse/argparse.hpp>

#include "log.h"

enum Operation {
	OP_RUN,
	OP_TEST,
	OP_DUMP_CONFIG,
};

struct Config {

	// from args
	std::string    path_load;
	std::string    path_dump;
	enum Operation op;

	// from config
	std::string name;
	std::string version;

	LogSettings log;

	bool load(const std::string & path);
	void dump(const std::string & path = "");
};

Config get_config_with_args(argparse::ArgumentParser & parser);

#endif
