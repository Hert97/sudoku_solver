
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <chrono>
#include "Sudoku.h"
std::vector<Solver::Sudoku> read_puzzle_files(const char* path)
{
	std::ifstream ifs(path);
	std::vector<Solver::Sudoku> p;
	if (!ifs) {
		std::cerr << "Error: failed to open file";
		return p;
	}

	std::string s;
	while (ifs) {
		p.emplace_back();
		ifs >> s; // each puzzle is one line in data

		for (char c : s) { //parse each puzzle data
			p.back().matrix.emplace_back(0);

			if (c == '.' || c == '0') { // empty cell
				p.back().matrix.back() = 0b111111111;
				p.back().board.emplace_back(0);
				continue;
			}
			p.back().board.emplace_back(c - '0');
		}
	}

	assert(p.front().board.size() == p.front().matrix.size());
	return p;
}



int main(int argc, const char** argv)
{
	if (argc != 2) {
		std::cerr << "Error no path provided\n";
	}
	//second argv is filepath
	std::vector<Solver::Sudoku> puzzle = read_puzzle_files(argv[1]);

	int count = 1;
	
	for (auto& p : puzzle) {
		using namespace std::chrono;
		std::cout << "\nPuzzle " << count++ << '\n';
		p.display_board();
		//puzzle[0].display_possible_value();
		high_resolution_clock::time_point t1 = high_resolution_clock::now();
		p.solve();
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
		std::cout << "Solved in: " << std::fixed << std::setprecision(4) << time_span.count() << 's' << '\n';
		p.display_board();
	}

	return 0;
}