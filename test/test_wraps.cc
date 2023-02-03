#define DOCTEST_CONFIG_IMPLEMENTATION_IN_DLL
#include <doctest.h>

#include <argparse/argparse.hpp>

TEST_SUITE("argparse") {
	TEST_CASE("args_default") {
		argparse::ArgumentParser parser("test");
		auto                     help_msg{parser.help().str()};
		REQUIRE(help_msg.find("shows help message") != std::string::npos);
		REQUIRE(help_msg.find("prints version information") != std::string::npos);
	}

	TEST_CASE("args_no_default") {
		argparse::ArgumentParser parser("test", "1.0", argparse::default_arguments::none);
		parser.parse_args({"test"});
		REQUIRE_THROWS_AS(parser.get("--help"), std::logic_error);
		REQUIRE_THROWS_AS(parser.get("--version"), std::logic_error);
	}
}

#include <thread>
#include <iostream>
#include <indicators/progress_spinner.hpp>

TEST_CASE("indicators") {

	using namespace indicators;

	indicators::ProgressSpinner spinner{
		option::PostfixText{"Checking credentials"},
		option::ForegroundColor{Color::yellow},
		option::SpinnerStates{
			std::vector<std::string>{"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"}},
		option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};

	// Update spinner state
	auto job = [&spinner]() {
		while (true) {
			if (spinner.is_completed()) {
				spinner.set_option(option::ForegroundColor{Color::green});
				spinner.set_option(option::PrefixText{"✔"});
				spinner.set_option(option::ShowSpinner{false});
				spinner.set_option(option::ShowPercentage{false});
				spinner.set_option(option::PostfixText{"Authenticated!"});
				spinner.mark_as_completed();
				break;
			} else
				spinner.tick();
			std::this_thread::sleep_for(std::chrono::milliseconds(40));
		}
	};
	std::thread thread(job);
	thread.join();
}

#include <tabulate/table.hpp>

TEST_CASE("tabulate") {
	using namespace tabulate;

	Table styled_table;
	styled_table.add_row({"Bold", "Italic", "Bold & Italic", "Blinking"});
	styled_table.add_row({"Underline", "Crossed", "Dark", "Bold, Italic & Underlined"});

	styled_table[0][0].format().font_style({FontStyle::bold});
	styled_table[0][1].format().font_style({FontStyle::italic});
	styled_table[0][2].format().font_style({FontStyle::bold, FontStyle::italic});
	styled_table[0][3].format().font_style({FontStyle::blink});
	styled_table[1][0].format().font_style({FontStyle::underline});
	styled_table[1][1].format().font_style({FontStyle::crossed});
	styled_table[1][2].format().font_style({FontStyle::dark});
	styled_table[1][3].format().font_style(
		{FontStyle::bold, FontStyle::italic, FontStyle::underline});

	std::cout << styled_table << std::endl;
}
