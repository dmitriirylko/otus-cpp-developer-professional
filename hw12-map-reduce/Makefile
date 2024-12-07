.PHONY: all clean

all: bin/mapper_mean bin/reducer_mean bin/mapper_var bin/reducer_var

bin/mapper_mean:
	mkdir -p bin
	g++ -o $@ -g mapper_mean.cpp

bin/reducer_mean:
	mkdir -p bin
	g++ -o $@ -g reducer_mean.cpp

bin/mapper_var:
	mkdir -p bin
	g++ -o $@ -g mapper_var.cpp

bin/reducer_var:
	mkdir -p bin
	g++ -o $@ -g reducer_var.cpp

clean:
	rm -rf bin output_mean output_var