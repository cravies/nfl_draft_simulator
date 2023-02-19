all:
	clang++ -std=c++20 helloworld.cpp -o hello
	./hello
	python3 plot_results.py
