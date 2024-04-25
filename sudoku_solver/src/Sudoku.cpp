#include "Sudoku.h"
#include <iostream>
#include <bitset>
#ifdef _DEBUG
#include <cassert>
#endif

namespace Solver {
	void Sudoku::display_board() const noexcept
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

	void Sudoku::display_possible_value() const noexcept
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
		std::cout << '\n';
	}


	bool Sudoku::is_valid_entry(int i, int value) const noexcept
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

	void Sudoku::reduce_puzzle(std::vector<int> m) noexcept
	{
		//eliminate values
		int i;
		int j, k;
		for (i = 0; i < board.size(); ++i) {
			int cellValue = board[i];
			if (cellValue == 0) continue;
			//eliminate all dup values in cell
			int r = i / BOARD_SIZE;
			int c = i % BOARD_SIZE;
			int base = (r - r % BASE_CELL_SIZE) * BOARD_SIZE + (c - c % BASE_CELL_SIZE);
			for (j = 0; j < BASE_CELL_SIZE; ++j) {
				for (k = 0; k < BASE_CELL_SIZE; ++k) {
					m[base + j * BOARD_SIZE + k] &= ~(1 << (cellValue - 1));
				}
			}
			//eliminate all dup values in col
			for (j = 0; j < BOARD_SIZE; ++j) {
				m[c + BOARD_SIZE * j] &= ~(1 << (cellValue - 1));
			}
			//eliminate all dup values in row
			for (j = 0; j < BOARD_SIZE; ++j) {
				m[r * BOARD_SIZE + j] &= ~(1 << (cellValue - 1));
			}
		}

		//naked twin strategy @todo

	}

	int Sudoku::get_next_empty_cell() const noexcept
	{
		int i;
		for (i = 0; i < board.size(); ++i) {
			if (board[i] == 0)
				return i;
		}
		return -1;
	}

	bool Sudoku::search(std::vector<int> m) noexcept
	{
		reduce_puzzle(m);

		int i = get_next_empty_cell();
		// solved
		if (i == -1) 
			return true;
		//display_board(board);

		for (int k = 0; k < BOARD_SIZE; ++k) {
			int bitFlag = 1 << k;
			int value = k + 1;
			if (m[i] & bitFlag && is_valid_entry( i, value)) {
				board[i] = value;
				bool success = search(m);
				if (success)
					return true;
				//else backtrack
				board[i] = 0;
				
			}
		}
		return false;
	}

	bool Sudoku::solve() noexcept
	{
		return search(matrix);
	}
}