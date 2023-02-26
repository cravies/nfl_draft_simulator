all:
	clang++ -std=c++20 draft_simulator.cpp -o draft
	./draft
	python3 plot_results.py
clean:
	rm *.png || echo "dir clean."
