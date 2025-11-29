// search.h - Search algorithm
#ifndef SEARCH_H
#define SEARCH_H

#include "types.h"
#include "position.h"
#include "movegen.h"
#include "eval.h"
#include "tt.h"
#include <atomic>
#include <algorithm>
#include <iostream>

namespace Search
{
    struct SearchInfo
    {
        std::atomic<bool> stopped{false};
        int max_depth = MAX_PLY;
        int64_t max_nodes = 0;
        int64_t max_time = 0;

        void reset()
        {
            stopped = false;
            max_depth = MAX_PLY;
            max_nodes = 0;
            max_time = 0;
        }
    };

    SearchInfo info;

    Score search(Position &pos, Depth depth, Score alpha, Score beta);
    Score quiescence(Position &pos, Score alpha, Score beta);

    Move best_move(Position &pos)
    {
        Move best;
        Score best_score = -SCORE_INFINITE;

        // Iterative deepening
        for (Depth depth = 1; depth <= info.max_depth; depth++)
        {
            if (info.stopped)
                break;

            Score score = search(pos, depth, -SCORE_INFINITE, SCORE_INFINITE);

            // Get best move from TT
            TTEntry entry;
            if (TranspositionTable::instance().probe(0, entry))
            {
                best = entry.best_move;
                best_score = entry.score;
            }

            std::cout << "info depth " << int(depth)
                      << " score cp " << score << std::endl;
        }

        return best;
    }

    Score search(Position &pos, Depth depth, Score alpha, Score beta)
    {
        if (info.stopped)
            return 0;

        // Check TT
        TTEntry tt_entry;
        bool tt_hit = TranspositionTable::instance().probe(0, tt_entry);

        if (tt_hit && tt_entry.depth >= depth)
        {
            if (tt_entry.bound == BOUND_EXACT)
                return tt_entry.score;
            if (tt_entry.bound == BOUND_LOWER)
                alpha = std::max(alpha, tt_entry.score);
            if (tt_entry.bound == BOUND_UPPER)
                beta = std::min(beta, tt_entry.score);
            if (alpha >= beta)
                return tt_entry.score;
        }

        // Leaf node - quiescence search
        if (depth <= 0)
        {
            return quiescence(pos, alpha, beta);
        }

        // Generate moves
        MoveList moves;
        MoveGen::generate_all(pos, moves);

        if (moves.size() == 0)
        {
            return pos.in_check() ? -SCORE_MATE : SCORE_DRAW;
        }

        Move best_move;
        Score best_score = -SCORE_INFINITE;
        Bound bound = BOUND_UPPER;

        // Search all moves
        for (int i = 0; i < moves.size(); i++)
        {
            pos.do_move(moves[i]);
            Score score = -search(pos, depth - 1, -beta, -alpha);
            pos.undo_move();

            if (score > best_score)
            {
                best_score = score;
                best_move = moves[i];

                if (score > alpha)
                {
                    alpha = score;
                    bound = BOUND_EXACT;

                    if (score >= beta)
                    {
                        bound = BOUND_LOWER;
                        break; // Beta cutoff
                    }
                }
            }
        }

        // Store in TT
        TranspositionTable::instance().store(0, best_move, best_score, depth, bound);

        return best_score;
    }

    Score quiescence(Position &pos, Score alpha, Score beta)
    {
        Score stand_pat = Eval::evaluate(pos);

        if (stand_pat >= beta)
            return beta;
        if (stand_pat > alpha)
            alpha = stand_pat;

        // Generate and search captures
        MoveList moves;
        MoveGen::generate_captures(pos, moves);

        for (int i = 0; i < moves.size(); i++)
        {
            pos.do_move(moves[i]);
            Score score = -quiescence(pos, -beta, -alpha);
            pos.undo_move();

            if (score >= beta)
                return beta;
            if (score > alpha)
                alpha = score;
        }

        return alpha;
    }
}

#endif // SEARCH_H
