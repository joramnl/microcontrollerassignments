all: clean compile run

compile:
	@echo -n "Compiling..."
	@gcc -Wall -o assignment assignment.c -lwiringPi
	@echo "Done"

run:
	@echo "Running..."
	@./assignment

clean:
	@echo -n "Cleaning up... "
	@rm -f assignment
	@echo "Done"

