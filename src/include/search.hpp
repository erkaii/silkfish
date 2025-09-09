#pragma once
#include "chess.hpp"
#include "thread_pool.hpp"
#include <algorithm>
std::pair<int, std::string> quiescence_search(int q_depth, int alpha, int beta, chess::Color color, chess::Board board);
std::pair<int, std::string> minimax(int mm_depth, int alpha, int beta, chess::Color color, chess::Board board);
chess::Move findBestMove(chess::Board& board, int depth, int max_threads);