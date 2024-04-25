#pragma once
#include <vector>



namespace Solver {
	struct Sudoku {

		static constexpr int BASE_CELL_SIZE = 3;
		static constexpr int BOARD_SIZE = BASE_CELL_SIZE * BASE_CELL_SIZE;
		std::vector<int> board;
		// possible values for each cell each bit represent possible values from 1-9
		// a value of 1 1111 1111 = all values from 1-9 is available
		std::vector<int> matrix;

	public:
		void display_board() const noexcept;
		void display_possible_value() const noexcept;

		void reduce_puzzle(std::vector<int> m) noexcept;
		bool solve() noexcept;
	private:
		int get_next_empty_cell() const noexcept;
		bool is_valid_entry(int i, int value) const noexcept;
		bool search(std::vector<int> m) noexcept;

	};
}
