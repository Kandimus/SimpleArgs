
#include "simpleargs.h"

const std::string ARG_FIRST  = "first";
const std::string ARG_SECOND = "second";
const std::string ARG_THRID  = "thrid";

void printOption(const std::string& name)
{
	printf("'%s' = '%s' (%u)\n", name.c_str(), rSimpleArgs::instance().getOption(name).c_str(), rSimpleArgs::instance().isSet(name));
}

int main(int argc, const char **argv)
{
	rSimpleArgs::instance()
			.addSwitch(ARG_FIRST, 'a')
			.addSwitch(ARG_SECOND, 'b')
			.addOption(ARG_THRID, 'c', "123");

	unsigned int result = rSimpleArgs::instance().parse(argc, argv);

	if (result) {
		printf("error arg num %u\n", result);
		return result;
	}

	printOption(ARG_FIRST);
	printOption(ARG_SECOND);
	printOption(ARG_THRID);

	std::string  arg = "";
	unsigned int num = 0;
	printf("argumets:\n");
	while ((arg = rSimpleArgs::instance().getArgument(num)) != "") {
		printf("  %u '%s'\n", num++, arg.c_str());
	}

	return 0;
}
