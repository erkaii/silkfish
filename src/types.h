// types.h - Basic types and constants
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>

using U64 = uint64_t;
using Score = int16_t;
using Depth = int8_t;

constexpr int MAX_MOVES = 256;
constexpr int MAX_PLY = 128;
constexpr Score SCORE_INFINITE = 32000;
constexpr Score SCORE_MATE = 31000;
constexpr Score SCORE_DRAW = 0;

enum Color { WHITE, BLACK, COLOR_NB };
enum PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, PIECE_TYPE_NB };
enum Piece { 
    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
    PIECE_NB, NO_PIECE
};

enum Square {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    SQUARE_NB, NO_SQUARE
};

enum CastlingRight {
    WHITE_OO = 1, WHITE_OOO = 2,
    BLACK_OO = 4, BLACK_OOO = 8,
    CASTLING_NB = 16
};

struct Move {
    Square from;
    Square to;
    PieceType promotion;
    
    Move() : from(NO_SQUARE), to(NO_SQUARE), promotion(PIECE_TYPE_NB) {}
    Move(Square f, Square t, PieceType p = PIECE_TYPE_NB) 
        : from(f), to(t), promotion(p) {}
    
    bool operator==(const Move& other) const {
        return from == other.from && to == other.to && promotion == other.promotion;
    }
    
    std::string to_string() const {
        if (from == NO_SQUARE || to == NO_SQUARE) return "0000";
        
        std::string str;
        str += char('a' + (from % 8));
        str += char('1' + (from / 8));
        str += char('a' + (to % 8));
        str += char('1' + (to / 8));
        
        if (promotion != PIECE_TYPE_NB) {
            switch (promotion) {
                case KNIGHT: str += 'n'; break;
                case BISHOP: str += 'b'; break;
                case ROOK: str += 'r'; break;
                case QUEEN: str += 'q'; break;
                default: break;
            }
        }
        
        return str;
    }
};

 Color operator~(Color c) { return Color(c ^ 1); }

#endif // TYPES_H
