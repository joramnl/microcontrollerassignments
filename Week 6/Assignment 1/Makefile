all: cleanup firstname lastname firstname_objectfiles lastname_objectfiles compile

firstname:
	@echo -n  "Generating firstname_array.h... "
	@echo "#include <vector>" > firstname_array.h
	@echo "#include <string>" >> firstname_array.h
	@echo "std::vector<std::string> firstname = {" >> firstname_array.h
	@sed 's/.*/"&",/' firstname.txt >> firstname_array.h
	@echo "}; " >> firstname_array.h
	@echo "Done"

lastname:
	@echo -n  "Generating lastname_array.h... "
	@echo "#include <vector>" > lastname_array.h
	@echo "#include <string>" >> lastname_array.h
	@echo "std::vector<std::string> lastname = {" >> lastname_array.h
	@sed 's/.*/"&",/' lastname.txt >> lastname_array.h
	@echo "}; " >> lastname_array.h
	@echo "Done"

firstname_objectfiles:
	@echo -n "Generating object files for firstname... "
	@g++ -Wall -g -c firstname.cpp -o firstname.o
	@echo "Done"

lastname_objectfiles:
	@echo -n "Generating object files for lastname... "
	@g++ -Wall -g -c lastname.cpp -o lastname.o
	@echo "Done"

compile:
	@echo -n "Compiling into executable... "
	@g++ -std=c++11 -Wall -o generate_name firstname.o lastname.o main.cpp
	@echo "Done"

cleanup:
	@echo -n "Cleaning up... "
	@rm -f *_array.h
	@rm -f *.o
	@rm -f generate_name
	@echo "Done"