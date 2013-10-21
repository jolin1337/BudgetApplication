#include "Group/Group-Event-.h"
#include "Events/Event.h"
#include "BudgetString.h"

#include <iostream>
#include <sstream>

#include <getopt.h> // includes the getopt command for input flags


void help() {
	std::cout << "This program is a simple budget manager that can be used as follows:\n\n";
	std::cout << "   (-v|--version)                          prints the version of this program\n"
			  << "   (-h|--help)                             prints this help message\n"
			  << "   (-p|--period) period                    lists the events active under the specified period\n"
			  << "                                           example: -p \"2000-01-02 2000-03-30\"\n"
			  << "   (-g|--group) [group]                    lists all groups if no parameter given, prints the\n"
			  << "                                           members of the specified group otherwise\n"
			  << "   (-l|--ls) [event|revent]                lists all events. If parameter events specified it\n"
			  << "                                           will print only events that happens once, if revents\n"
			  << "                                           specified the it will print only the repeating events\n"
			  << "   (-a|--add) money,date(s),[period]       adds a event to the budget with money and date\n"
			  << "   (-r|--rm) index                         removes event(s) with specified index/indicies\n"
			  << "                                           if the index is negative it will remove the events, start\n"
			  << "                                           counting backwards\n"
			  << "   (-m|--mv) index,[money|date],[period]   modifies a event specified with index(otherwise it's the\n"
			  << "                                           same syntax as adding an event)\n"
			  << "   (-A|--link) 'groupname',index           links a event with index specified with a specific group\n"
			  << "                                           if the index is negative it will link the events, start\n"
			  << "                                           counting backwards\n"
			  << "   (-R|--unlink) groupname,index           unlinks the events with index from the specified group\n"
			  << "                                           if the index is negative it will link the events, start\n"
			  << "                                           counting backwards\n"
			  << "   (-M|--modify) groupname,newgroupname    unlinks the events with index from the specified group\n"
			  << "                                           if the index is negative it will link the events, start\n"
			  << "                                           counting backwards\n\n"
			  << "Note: There are not allowed any spaces in group names(you can use _ for a nice output)\n"
			  << "      Adding spaces to a group may cause some unnexpected errors but the most of the \n"
			  << "      time nothing will happend.\n";
}
void version(){
	std::cout << "Version nummber: 1.0\nThis program was made by Johannes Lindén and are free to use\n";
}

int main(int argc, char *argv[]) {
	option options[] = {
		{"version", no_argument, 0, 'v' },
		{"help", no_argument, 0, 'h' },
		{"period", required_argument, 0, 'p' },
		{"group", optional_argument, 0, 'g' },
		{"ls", optional_argument, 0, 'l'},
		{"add", required_argument, 0, 'a'},
		{"rm", required_argument, 0, 'r'},
		{"mv", required_argument, 0, 'm'},
		{"link", required_argument, 0, 'A'},
		{"unlink", required_argument, 0, 'R'},
		{"modify", required_argument, 0, 'M'},
		{0, 0, 0, 0 }
	};
	int this_option_optind = optind ? optind : 1;
	int option_index = 0;
	char c;

	int status = 0;
	BudgetString budget("MyBudget.bas");
	try {
		if(argc == 1) {
			help();
			return 0;
		}
		while((c = getopt_long(argc, argv, "vhp:g::l::a:r:m:A:R:M:", options, &option_index)) != -1) {
			std::string str;
			switch(c) {
				case 'v':
					version();
					break;
				case 'h':
					help();
					break;
				case 'p':
					budget.printPeriodStr(optarg);
					break;
				case 'g':
					budget.load("MyBudget.bas");
					if(optarg == 0)
						budget.printGroups();
					else
						budget.printGroup(optarg);
					break;
				case 'l':
					if(optarg == 0) {
						budget.load("MyBudget.bas");
						budget.printEventList();
						budget.printREventList();
					}
					else {
						str = optarg;
						if(str == "events" || str == "event")
							budget.printEventList();
						else if(str == "revents" || str == "revent")
							budget.printREventList();
					}
					break;
				case 'a':
					status |= budget.addEventStr(optarg);
					break;
				case 'r':
					status |= budget.removeEventStr(optarg);
					break;
				case 'm':
					status |= budget.editEventStr(optarg);
					break;
				case 'A':
					status |= budget.linkEventGroupStr(optarg);
					break;
				case 'R':
					status |= budget.unlinkEventGroupStr(optarg);
					break;
				case 'M':
					status |= budget.editGroupStr(optarg);
					break;
				case '?':
					status = 1;
			}
			if(status) {
				help();
				return status;
			}
		}
	}
	catch(std::exception &e) {
		std::cout << e.what() << "\n";
		help();
	}
	return 0;
}