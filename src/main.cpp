// main.cpp - Entry point for Silkfish Chess Engine
#include "uci.h"
#include "bitboard.h"
#include "search.h"
#include "tt.h"
#include <iostream>

int main()
{
    std::cout << "Silkfish Chess Engine v1.0" << std::endl;

    // Initialize engine components
    Bitboard::init();
    TranspositionTable::instance().resize(16); // 16 MB default

    // Start UCI loop
    UCI::loop();

    return 0;
}
