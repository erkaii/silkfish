// tt.h - Transposition table
#ifndef TT_H
#define TT_H

#include "types.h"
#include <vector>

enum Bound { BOUND_NONE, BOUND_UPPER, BOUND_LOWER, BOUND_EXACT };

struct TTEntry {
    U64 key;
    Move best_move;
    Score score;
    Depth depth;
    Bound bound;
    
    TTEntry() : key(0), best_move(), score(0), depth(0), bound(BOUND_NONE) {}
};

class TranspositionTable {
public:
    static TranspositionTable& instance() {
        static TranspositionTable tt;
        return tt;
    }
    
    void resize(size_t mb) {
        size_t entries = (mb * 1024 * 1024) / sizeof(TTEntry);
        // Round down to power of 2
        size_t size = 1;
        while (size * 2 <= entries) size *= 2;
        
        table.resize(size);
        size_mask = size - 1;
        clear();
    }
    
    void clear() {
        for (auto& entry : table) {
            entry = TTEntry();
        }
    }
    
    bool probe(U64 key, TTEntry& entry) const {
        const TTEntry& stored = table[key & size_mask];
        if (stored.key == key) {
            entry = stored;
            return true;
        }
        return false;
    }
    
    void store(U64 key, Move best_move, Score score, Depth depth, Bound bound) {
        TTEntry& entry = table[key & size_mask];
        
        // Replace if depth is higher or same position
        if (entry.key != key || depth >= entry.depth) {
            entry.key = key;
            entry.best_move = best_move;
            entry.score = score;
            entry.depth = depth;
            entry.bound = bound;
        }
    }
    
private:
    TranspositionTable() {}
    std::vector<TTEntry> table;
    size_t size_mask;
};

#endif // TT_H
