#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include <doctest.h>

#include <fmt/core.h>
#include <iostream>
#include <string>

#include <toml++/toml.h>
#include <toml++/impl/json_formatter.h>

using namespace std::string_view_literals;

TEST_CASE("toml") {
	toml::table config;

	try {
		config = toml::parse_file("data/test.toml");
	} catch (const toml::parse_error & err) {
		std::cout << err << std::endl;
		return;
	}

	// get key-value pairs
	std::string_view library_name           = config["library"]["name"].value_or(""sv);
	std::string_view library_author         = config["library"]["authors"][0].value_or(""sv);
	int64_t          depends_on_cpp_version = config["dependencies"]["cpp"].value_or(0);

	fmt::print("name: {}, author: {}, ver: {}",
	           library_name,
	           library_author,
	           depends_on_cpp_version);

	// modify the data
	config.insert_or_assign("alternatives", toml::array{"cpptoml", "toml11", "Boost.TOML"});

	// use a visitor to iterate over heterogenous data
	config.for_each([](auto & key, auto & value) {
		std::cout << value << std::endl;
		if constexpr (toml::is_string<decltype(value)>) {
			std::cout << value << std::endl;
		}
	});

	// you can also iterate more 'traditionally' using a ranged-for
	// for (auto &&[k, v] : config) {
	//	// ...
	//}

	std::cout << "\n==== re-serialize as TOML:" << std::endl;
	std::cout << config << std::endl;

	std::cout << "\n==== re-serialize as YAML:" << std::endl;
	std::cout << toml::yaml_formatter{config} << std::endl;

	// re-serialize as JSON
	std::cout << "\n==== re-serialize as JSON:" << std::endl;
	std::cout << toml::json_formatter{config} << std::endl;
}
