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

#define TIME_PER_MOVE
#define INIT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"


int main (int argc, char *argv[]) {
	handle_uci_command();
	return 0;
}
