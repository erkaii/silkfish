#include "chess.hpp"
#include "constants.hpp"
#include "evaluation.hpp"
#include "search.hpp"
#include "uci.hpp"

#include <chrono> // For time measurements
#include <thread>
#include <vector>
#include <semaphore>

using namespace chess;
using namespace std;

int quiescence_depth = DEFAULT_DEPTH_Q;
int mm_depth = DEFAULT_DEPTH_MM;
int evals[1000];

#define DEBUG
#define TIME_PER_MOVE
#define INIT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"


int main (int argc, char *argv[]) {
#ifdef DEBUG
	string fen_string = string(INIT_FEN);

	Board board = Board(fen_string);
	Movelist moves;

	Color turn = board.sideToMove();

	string game_pgn = "";
	int round = 1;
	while (board.isGameOver().first == GameResultReason::NONE) {
		movegen::legalmoves(moves, board);
		Move picked_move;
		auto start = std::chrono::high_resolution_clock::now();
		picked_move = findBestMove(board, mm_depth, MAX_THREAD);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		std::cout << "Execution time: " << duration.count() << " seconds\n";
		string move_s = uci::moveToSan(board, picked_move);
		if (turn == Color::WHITE) {
			game_pgn = game_pgn + " " + to_string(round) + ". " + move_s;
		} else {
			game_pgn = game_pgn + " " + move_s;
			round++;
		}
		board.makeMove(picked_move);
		std::cout << move_s << endl << endl;
		turn = 1 - turn;
	}

	std::cout << game_pgn << endl;
	return 0;
#endif
	handle_uci_command();
	return 0;
}
