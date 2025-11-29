// position.h - Board position representation
#ifndef POSITION_H
#define POSITION_H

#include "types.h"
#include "bitboard.h"
#include <string>
#include <sstream>
#include <cctype>

class Position
{
public:
    Position()
    {
        for (int i = 0; i < SQUARE_NB; i++)
            board[i] = NO_PIECE;
        for (int i = 0; i < COLOR_NB; i++)
            byColor[i] = 0ULL;
        for (int i = 0; i < PIECE_TYPE_NB; i++)
            byType[i] = 0ULL;

        sideToMove = WHITE;
        castlingRights = 0;
        epSquare = NO_SQUARE;
        halfmoveClock = 0;
        fullmoveNumber = 1;
        hash = 0;
    }

    void set_fen(const std::string &fen)
    {
        // Clear position
        *this = Position();

        std::istringstream ss(fen);
        std::string token;

        // Piece placement
        ss >> token;
        int rank = 7, file = 0;
        for (char c : token)
        {
            if (c == '/')
            {
                rank--;
                file = 0;
            }
            else if (isdigit(c))
            {
                file += c - '0';
            }
            else
            {
                Color color = isupper(c) ? WHITE : BLACK;
                PieceType pt;
                switch (tolower(c))
                {
                case 'p':
                    pt = PAWN;
                    break;
                case 'n':
                    pt = KNIGHT;
                    break;
                case 'b':
                    pt = BISHOP;
                    break;
                case 'r':
                    pt = ROOK;
                    break;
                case 'q':
                    pt = QUEEN;
                    break;
                case 'k':
                    pt = KING;
                    break;
                default:
                    continue;
                }

                Square sq = Square(rank * 8 + file);
                Piece piece = Piece(color * 6 + pt);
                board[sq] = piece;
                byColor[color] |= Bitboard::square_bb(sq);
                byType[pt] |= Bitboard::square_bb(sq);
                file++;
            }
        }

        // Side to move
        ss >> token;
        sideToMove = (token == "w") ? WHITE : BLACK;

        // Castling rights
        ss >> token;
        castlingRights = 0;
        if (token.find('K') != std::string::npos)
            castlingRights |= WHITE_OO;
        if (token.find('Q') != std::string::npos)
            castlingRights |= WHITE_OOO;
        if (token.find('k') != std::string::npos)
            castlingRights |= BLACK_OO;
        if (token.find('q') != std::string::npos)
            castlingRights |= BLACK_OOO;

        // En passant
        ss >> token;
        if (token != "-")
        {
            int file = token[0] - 'a';
            int rank = token[1] - '1';
            epSquare = Square(rank * 8 + file);
        }

        // Half-move clock
        if (ss >> token)
            halfmoveClock = std::stoi(token);

        // Full-move number
        if (ss >> token)
            fullmoveNumber = std::stoi(token);
    }

    std::string get_fen() const
    {
        // TODO: Implement FEN generation
        return "";
    }

    Piece piece_on(Square s) const { return board[s]; }
    U64 pieces(Color c) const { return byColor[c]; }
    U64 pieces(PieceType pt) const { return byType[pt]; }
    U64 pieces(Color c, PieceType pt) const { return byColor[c] & byType[pt]; }

    Color side_to_move() const { return sideToMove; }
    int castling_rights() const { return castlingRights; }
    Square ep_square() const { return epSquare; }

    bool is_legal(Move m) const
    {
        // TODO: Implement legality check
        (void)m;
        return true;
    }

    void do_move(Move m)
    {
        // TODO: Implement move execution
        // Update board, bitboards, castling rights, ep square, etc.
        (void)m;
    }

    void undo_move()
    {
        // TODO: Implement move undo
        // Requires maintaining move history
    }

    bool in_check() const
    {
        // TODO: Check if current side is in check
        return false;
    }

    bool is_draw() const
    {
        // Check for insufficient material, 50-move rule, repetition
        return halfmoveClock >= 100;
    }

private:
    Piece board[SQUARE_NB];
    U64 byColor[COLOR_NB];
    U64 byType[PIECE_TYPE_NB];

    Color sideToMove;
    int castlingRights;
    Square epSquare;
    int halfmoveClock;
    int fullmoveNumber;

    U64 hash;
};

#endif // POSITION_H
