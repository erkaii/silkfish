// eval.h - Position evaluation
#ifndef EVAL_H
#define EVAL_H

#include "types.h"
#include "position.h"
#include "bitboard.h"

namespace Eval {
    constexpr Score PieceValue[PIECE_TYPE_NB] = {
        100,  // Pawn
        320,  // Knight
        330,  // Bishop
        500,  // Rook
        900,  // Queen
        20000 // King
    };
    
    inline Score evaluate(const Position& pos) {
        Score score = 0;
        
        // Material evaluation
        for (PieceType pt = PAWN; pt < PIECE_TYPE_NB; pt = PieceType(pt + 1)) {
            int white_count = Bitboard::pop_count(pos.pieces(WHITE, pt));
            int black_count = Bitboard::pop_count(pos.pieces(BLACK, pt));
            score += (white_count - black_count) * PieceValue[pt];
        }
        
        // TODO: Add positional evaluation
        // - Piece-square tables
        // - Pawn structure
        // - King safety
        // - Piece mobility
        
        return pos.side_to_move() == WHITE ? score : -score;
    }
}

#endif // EVAL_H
