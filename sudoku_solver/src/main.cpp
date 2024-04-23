
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#ifdef _DEBUG
#include <bitset>
#endif
#include <cassert>
static constexpr int BASE_CELL_SIZE = 3;
static constexpr int BOARD_SIZE = BASE_CELL_SIZE * BASE_CELL_SIZE;
struct Sudoku {
	std::vector<int> board;
	// possible values for each cell each bit represent possible values from 1-9
	// a value of 1 1111 1111 = all values from 1-9 is available
	std::vector<int> matrix;
};

void display_board(const std::vector<int>& board)
{
	int i, j;
	int index = 0;
	for (i = 0; i < BOARD_SIZE; ++i) {
		for (j = 0; j < BOARD_SIZE; ++j) {
			std::cout << board[index] << ' ';
			++index;
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void display_possible_value(std::vector<int> matrix)
{
	int i, j;
	int index = 0;
	for (i = 0; i < BOARD_SIZE; ++i) {
		for (j = 0; j < BOARD_SIZE; ++j) {
			std::cout << std::bitset<9>(matrix[index]) << ' ';
			++index;
		}
		std::cout << '\n';
	}

}

void read_puzzle_files(const char* path, std::vector<Sudoku>& p)
{
	std::ifstream ifs(path);

	if (!ifs) {
		std::cerr << "Error: failed to open file";
		return;
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
}

bool is_valid(const std::vector<int>& board, int i, int value)
{
	int r = i / BOARD_SIZE;
	int c = i % BOARD_SIZE;
	int j, k;
	int base = (r - r % BASE_CELL_SIZE) * BOARD_SIZE + (c - c % BASE_CELL_SIZE);
	//check for dups in 3x3 cell
	for (j = 0; j < BASE_CELL_SIZE; ++j) {
		for (k = 0; k < BASE_CELL_SIZE; ++k) {
			if (board[base + j * BOARD_SIZE + k] == value)
				return false;
		}
	}
	// dup values in col
	for (j = 0; j < BOARD_SIZE; ++j) {
		if (board[c + BOARD_SIZE * j] == value)
			return false;
	}
	// dup values in row
	for (j = 0; j < BOARD_SIZE; ++j) {
		if (board[r * BOARD_SIZE + j] == value) 
			return false;
	}
	return true;
}

void reduce_puzzle(Sudoku& p)
{

	int i;
	int j, k;
	for (i = 0; i < p.board.size(); ++i) {
		int cellValue = p.board[i];
		if (cellValue == 0) continue;
		//eliminate all dup values in cell
		int r = i / BOARD_SIZE;
		int c = i % BOARD_SIZE;
		int base = (r - r % BASE_CELL_SIZE) * BOARD_SIZE + (c - c % BASE_CELL_SIZE);
		for (j = 0; j < BASE_CELL_SIZE; ++j) {
			for (k = 0; k < BASE_CELL_SIZE; ++k) {
				p.matrix[base + j * BOARD_SIZE + k] &= ~(1 << (cellValue - 1));
			}
		}
		//eliminate all dup values in col
		for (j = 0; j < BOARD_SIZE; ++j) {
			p.matrix[c + BOARD_SIZE * j] &= ~(1 << (cellValue - 1));
		}
		//eliminate all dup values in row
		for (j = 0; j < BOARD_SIZE; ++j) {
			p.matrix[r * BOARD_SIZE + j] &= ~(1 << (cellValue - 1));
		}
	}
}

int get_next_empty_cell(const std::vector<int>& board)
{
	int i;
	for (i = 0; i < board.size(); ++i) {
		if (board[i] == 0)
			return i;
	}
	return -1; 
}

bool search(std::vector<int>& board, const std::vector<int>& matrix)
{
	//get next empty cell
	int i = get_next_empty_cell(board);
	// solved
	if (i == -1 ) return true; 
	//display_board(board);

	for (int k = 0; k < BOARD_SIZE; ++k) {
		int bitFlag = 1 << k;
		int value = k + 1;
		if (matrix[i] & bitFlag && is_valid(board,i,value)) {
			board[i] = value;
			bool success = search(board, matrix);
			if (success) 
				return true;
			//else backtrack
			board[i] = 0;
		}
	}
	return false;
}



bool solve(Sudoku& p)
{
	return search(p.board, p.matrix);
}

int main(int argc, const char** argv)
{
	if (argc != 2) {
		std::cerr << "Error no path provided\n";
	}
	static std::vector<Sudoku> puzzle;
	//second arg is filepath
	read_puzzle_files(argv[1], puzzle);

	/*for (const auto& p : puzzle) {
		display_board(p.board);
		std::cout << '\n';
	}*/

	display_board(puzzle[1].board);
	reduce_puzzle(puzzle[1]);
	display_possible_value(puzzle[1].matrix);
	solve(puzzle[1]);

	std::cout << "\nAfter:\n";
	display_board(puzzle[1].board);

	return 0;
}