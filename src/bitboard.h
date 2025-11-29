// bitboard.h - Bitboard representation and utilities
#ifndef BITBOARD_H
#define BITBOARD_H

#include "types.h"

namespace Bitboard
{
    U64 SquareBB[SQUARE_NB];
    U64 FileBB[8];
    U64 RankBB[8];

    void init()
    {
        // Initialize square bitboards
        for (int s = 0; s < SQUARE_NB; s++)
        {
            SquareBB[s] = 1ULL << s;
        }

        // Initialize file bitboards
        for (int f = 0; f < 8; f++)
        {
            FileBB[f] = 0x0101010101010101ULL << f;
        }

        // Initialize rank bitboards
        for (int r = 0; r < 8; r++)
        {
            RankBB[r] = 0xFFULL << (r * 8);
        }
    }

    U64 square_bb(Square s) { return SquareBB[s]; }
    int pop_count(U64 b) { return __builtin_popcountll(b); }
    Square lsb(U64 b) { return Square(__builtin_ctzll(b)); }
    Square pop_lsb(U64 &b)
    {
        Square s = lsb(b);
        b &= b - 1;
        return s;
    }

    U64 attacks_bb(PieceType pt, Square s, U64 occupied)
    {
        // Simplified attack generation - expand with magic bitboards
        U64 attacks = 0ULL;

        switch (pt)
        {
        case KNIGHT:
        {
            int rank = s / 8, file = s % 8;
            int offsets[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
            for (auto &off : offsets)
            {
                int r = rank + off[0], f = file + off[1];
                if (r >= 0 && r < 8 && f >= 0 && f < 8)
                    attacks |= square_bb(Square(r * 8 + f));
            }
            break;
        }
        case KING:
        {
            int rank = s / 8, file = s % 8;
            for (int dr = -1; dr <= 1; dr++)
            {
                for (int df = -1; df <= 1; df++)
                {
                    if (dr == 0 && df == 0)
                        continue;
                    int r = rank + dr, f = file + df;
                    if (r >= 0 && r < 8 && f >= 0 && f < 8)
                        attacks |= square_bb(Square(r * 8 + f));
                }
            }
            break;
        }
        default:
            // Implement sliding piece attacks (rook, bishop, queen)
            (void)occupied; // Suppress unused warning
            break;
        }

        return attacks;
    }
}

#endif // BITBOARD_H
