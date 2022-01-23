all: cleanup firstname_array.h lastname_array.h firstname.o lastname.o generate_name

firstname_array.h: firstname.txt
	@echo -n  "Generating firstname_array.h... "
	@echo "#include <vector>" > firstname_array.h
	@echo "#include <string>" >> firstname_array.h
	@echo "std::vector<std::string> firstname = {" >> firstname_array.h
	@sed 's/.*/"&",/' firstname.txt >> firstname_array.h
	@echo "}; " >> firstname_array.h
	@echo "Done"

lastname_array.h: lastname.txt
	@echo -n  "Generating lastname_array.h... "
	@echo "#include <vector>" > lastname_array.h
	@echo "#include <string>" >> lastname_array.h
	@echo "std::vector<std::string> lastname = {" >> lastname_array.h
	@sed 's/.*/"&",/' lastname.txt >> lastname_array.h
	@echo "}; " >> lastname_array.h
	@echo "Done"

firstname.o: firstname_array.h firstname.cpp
	@echo -n "Generating object files for firstname... "
	@g++ -Wall -g -c firstname.cpp -o firstname.o
	@echo "Done"

lastname.o: lastname_array.h lastname.cpp
	@echo -n "Generating object files for lastname... "
	@g++ -Wall -g -c lastname.cpp -o lastname.o
	@echo "Done"

generate_name: firstname.o lastname.o main.cpp
	@echo -n "Compiling into executable... "
	@g++ -std=c++11 -Wall -o generate_name firstname.o lastname.o main.cpp
	@echo "Done"

compile: generate_name

cleanup:
	@echo -n "Cleaning up... "
	@rm -f *_array.h
	@rm -f *.o
	@rm -f generate_name
	@echo "Done"