// movegen.h - Move generation
#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "types.h"
#include "position.h"

class MoveList
{
public:
    void add(Move m) { moves[count++] = m; }
    Move &operator[](int i) { return moves[i]; }
    const Move &operator[](int i) const { return moves[i]; }
    int size() const { return count; }

private:
    Move moves[MAX_MOVES];
    int count = 0;
};

namespace MoveGen
{
    // Forward declarations
    void generate_captures(const Position &pos, MoveList &list);
    void generate_quiet(const Position &pos, MoveList &list);

    void generate_all(const Position &pos, MoveList &list)
    {
        generate_captures(pos, list);
        generate_quiet(pos, list);
    }

    void generate_captures(const Position &pos, MoveList &list)
    {
        // TODO: Implement capture generation
        // Generate pawn captures, piece captures
        (void)pos;
        (void)list;
    }

    void generate_quiet(const Position &pos, MoveList &list)
    {
        // TODO: Implement quiet move generation
        // Generate pawn pushes, piece moves, castling
        (void)pos;
        (void)list;
    }
}

#endif // MOVEGEN_H
