
headers = Group/Group.h Group/Comparer.h Events/Date/format_error.h   Events/Date/Period.h   Events/Date/Date.h   Events/Event.h   Events/REvent.h   Events/ComparerEvent.h   Group/Group-Event-.h   Budget.h   BudgetString.h
cpps = main.cpp                          Events/Date/format_error.cpp Events/Date/Period.cpp Events/Date/Date.cpp Events/Event.cpp Events/REvent.cpp Events/ComparerEvent.cpp Group/Group-Event-.cpp Budget.cpp BudgetString.cpp

all: budget_runnable

budget_runnable: ./bin/budgetApp

./bin/budgetApp: $(headers) $(cpps)
	ln -s -f ../Events Group/Events
	g++ $(cpps) -o ./bin/budgetApp 
	rm Group/Events

clean:
	rm -f bin/budgetApp
