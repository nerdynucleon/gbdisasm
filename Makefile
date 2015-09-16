all: gbdisasm.cpp
	g++ gbdisasm.cpp -o gbdisasm
debug: gbdisasm.cpp
	g++ -g -o gbdisasm gbdisasm.cpp
clean:
	rm gbdisasm
	rm -r gbdisasm.dSYM
