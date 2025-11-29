// uci.h - UCI protocol interface
#ifndef UCI_H
#define UCI_H

#include "types.h"
#include "position.h"
#include "search.h"
#include "tt.h"
#include "movegen.h"
#include <string>
#include <iostream>
#include <sstream>
#include <thread>

namespace UCI {
     Position position;
     std::thread search_thread;
    
     Move parse_move(const Position& pos, const std::string& str) {
        // Parse UCI move string (e.g., "e2e4", "e7e8q")
        if (str.length() < 4) return Move();
        
        Square from = Square((str[0] - 'a') + (str[1] - '1') * 8);
        Square to = Square((str[2] - 'a') + (str[3] - '1') * 8);
        
        PieceType promotion = PIECE_TYPE_NB;
        if (str.length() == 5) {
            switch (str[4]) {
                case 'n': promotion = KNIGHT; break;
                case 'b': promotion = BISHOP; break;
                case 'r': promotion = ROOK; break;
                case 'q': promotion = QUEEN; break;
            }
        }
        
        (void)pos; // Suppress unused warning
        return Move(from, to, promotion);
    }
    
     void cmd_uci() {
        std::cout << "id name Silkfish 1.0" << std::endl;
        std::cout << "id author Erkai Yu" << std::endl;
        std::cout << "option name Hash type spin default 16 min 1 max 1024" << std::endl;
        std::cout << "uciok" << std::endl;
    }
    
     void cmd_isready() {
        std::cout << "readyok" << std::endl;
    }
    
     void cmd_ucinewgame() {
        TranspositionTable::instance().clear();
        position.set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    
     void cmd_position(const std::string& line) {
        std::istringstream iss(line);
        std::string token;
        iss >> token; // "position"
        iss >> token; // "startpos" or "fen"
        
        if (token == "startpos") {
            position.set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            iss >> token; // "moves" or end
        } else if (token == "fen") {
            std::string fen;
            while (iss >> token && token != "moves") {
                fen += token + " ";
            }
            position.set_fen(fen);
        }
        
        // Apply moves
        if (token == "moves") {
            while (iss >> token) {
                Move m = parse_move(position, token);
                position.do_move(m);
            }
        }
    }
    
     void cmd_go(const std::string& line) {
        std::istringstream iss(line);
        std::string token;
        iss >> token; // "go"
        
        Search::info.reset();
        
        while (iss >> token) {
            if (token == "depth") {
                iss >> Search::info.max_depth;
            } else if (token == "movetime") {
                iss >> Search::info.max_time;
            } else if (token == "infinite") {
                Search::info.max_depth = MAX_PLY;
            }
        }
        
        // Start search in separate thread
        if (search_thread.joinable()) {
            search_thread.join();
        }
        
        search_thread = std::thread([]() {
            Position pos_copy = position;
            Move best = Search::best_move(pos_copy);
            std::cout << "bestmove " << best.to_string() << std::endl;
        });
    }
    
     void cmd_stop() {
        Search::info.stopped = true;
        if (search_thread.joinable()) {
            search_thread.join();
        }
    }
    
     void cmd_setoption(const std::string& line) {
        std::istringstream iss(line);
        std::string token;
        iss >> token; // "setoption"
        iss >> token; // "name"
        
        std::string name;
        while (iss >> token && token != "value") {
            name += token;
        }
        
        if (name == "Hash") {
            int mb;
            iss >> mb;
            TranspositionTable::instance().resize(mb);
        }
    }
    
     void loop() {
        std::string line;
        position.set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        
        while (std::getline(std::cin, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> token;
            
            if (token == "uci") {
                cmd_uci();
            } else if (token == "isready") {
                cmd_isready();
            } else if (token == "ucinewgame") {
                cmd_ucinewgame();
            } else if (token == "position") {
                cmd_position(line);
            } else if (token == "go") {
                cmd_go(line);
            } else if (token == "stop") {
                cmd_stop();
            } else if (token == "setoption") {
                cmd_setoption(line);
            } else if (token == "quit") {
                cmd_stop();
                break;
            }
        }
    }
}

#endif // UCI_H
