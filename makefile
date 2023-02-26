all:
	clang++ -std=c++20 draft_simulator.cpp -o draft_executable
	./draft_executable
	python3 plot_results.py
	rm draft_executable
clean:
	rm ./output/* || echo "output dir clean."