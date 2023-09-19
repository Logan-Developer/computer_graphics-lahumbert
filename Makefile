EXECUTABLES = sim

main: main.cpp
	g++ -Wall -o $(EXECUTABLES) $^

run: $(EXECUTABLES)
	./$^

clean:
	rm -f $(EXECUTABLES)