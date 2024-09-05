import random
import chess

from enum import Enum, auto
from configure import Configure
from evaluation import Evaluation

class PlayerType(Enum):
    HUMAN = auto()
    ENGINE = auto()

class Player:
    """
    A class for the player in a game, it stores all the configuration for the 
    player, including settings for an engine, the color it is using, etc.
    """

    def __init__ (self, type, conf_file=None):
        self.type = type
        self.color = None
        self.nodes_searched = 0
    
        # Load configuration    
        if self.type == PlayerType.ENGINE:
            if conf_file == None:
                raise ValueError("Error: Engine player missing configuration file.")
            self.conf = Configure(conf_file)
            self.evaluation = Evaluation(self.conf)
        elif self.type == PlayerType.HUMAN:
            self.conf = None
            self.evaluation = None
        else:
            raise ValueError("Error: Player type invalid.")
    
    def assign_color(self, color):
        """
        Assuming the given color input is valid.
        """
        self.color = color
    
    def get_move(self, board):
        """
        Returns:
            chess.Move: A move to be made given the board.
        """
        if self.type == PlayerType.HUMAN:
            while True:
                user_input = input("Enter your move in SAN format (e.g. Nf3): ")
                
                # Try to parse the input move
                try:
                    move = board.parse_san(user_input)  # Parse Standard Algebraic Notation (SAN)
                except ValueError:
                    print("Invalid move format. Please try again.")
                    continue
                
                # Check if the move is legal
                if move in board.legal_moves:
                    return move
                else:
                    print("Illegal move. Please try again.")

        elif self.type == PlayerType.ENGINE:
            self.minimax_nodes_searched = 0
            self.capture_nodes_searched = 0
            best_move = None
            max_eval = float('-inf')
            min_eval = float('inf')
            alpha = float('-inf')
            beta = float('inf')
            depth = self.conf.config["minimax_depth"]

            legal_moves = list(board.legal_moves)
            random.shuffle(legal_moves)
            for move in legal_moves:
                board.push(move)
                if self.color == chess.WHITE:
                    eval = self.minimax(board, depth - 1, alpha, beta, chess.BLACK)
                else:
                    eval = self.minimax(board, depth - 1, alpha, beta, chess.WHITE)
                board.pop()
                if self.color == chess.WHITE and eval > max_eval:
                    max_eval = eval
                    best_move = move
                if self.color == chess.BLACK and eval < min_eval:
                    min_eval = eval
                    best_move = move
            if best_move == None:
                best_move = legal_moves[0]
            
            print(f"Minimax nodes searched: {self.minimax_nodes_searched}")
            print(f"Capture nodes searched: {self.capture_nodes_searched}")
            if self.color == chess.WHITE:
                print(f"White Engine eval: {max_eval}")
            else:
                print(f"Black Engine eval: {min_eval}")
            return best_move

        else:
            raise ValueError("Error: Player type invalid.")

    def capture_minimax(self, board, depth, maximizing_color):
        self.capture_nodes_searched += 1
        cap_moves= []
        best_eval = self.evaluation.evaluate(board)

        if depth == 0:
            return best_eval

        for move in board.legal_moves:
            if board.is_capture(move):
                cap_moves.append(move)
        if len(cap_moves) == 0:
            return best_eval

        cap_moves.sort(reverse=True, key=lambda x: self.move_score(board, x))
        for move in cap_moves:
            if maximizing_color == chess.WHITE:
                eval = self.capture_minimax(board, depth - 1, chess.BLACK)
                best_eval = max(best_eval, eval)
            else: 
                eval = self.capture_minimax(board, depth - 1, chess.WHITE)
                best_eval = min(best_eval, eval)
        return best_eval

    capture_score = {
        (chess.KING, chess.QUEEN):50,
        (chess.QUEEN, chess.QUEEN):51,
        (chess.ROOK, chess.QUEEN):52,
        (chess.BISHOP, chess.QUEEN):53,
        (chess.KNIGHT, chess.QUEEN):54,
        (chess.PAWN, chess.QUEEN):55,

        (chess.KING, chess.ROOK):40,
        (chess.QUEEN, chess.ROOK):41,
        (chess.ROOK, chess.ROOK):42,
        (chess.BISHOP, chess.ROOK):43,
        (chess.KNIGHT, chess.ROOK):44,
        (chess.PAWN, chess.ROOK):45,

        (chess.KING, chess.BISHOP):30,
        (chess.QUEEN, chess.BISHOP):31,
        (chess.ROOK, chess.BISHOP):32,
        (chess.BISHOP, chess.BISHOP):33,
        (chess.KNIGHT, chess.BISHOP):34,
        (chess.PAWN, chess.BISHOP):35,

        (chess.KING, chess.KNIGHT):20,
        (chess.QUEEN, chess.KNIGHT):21,
        (chess.ROOK, chess.KNIGHT):22,
        (chess.BISHOP, chess.KNIGHT):23,
        (chess.KNIGHT, chess.KNIGHT):24,
        (chess.PAWN, chess.KNIGHT):25,

        (chess.KING, chess.PAWN):10,
        (chess.QUEEN, chess.PAWN):11,
        (chess.ROOK, chess.PAWN):12,
        (chess.BISHOP, chess.PAWN):13,
        (chess.KNIGHT, chess.PAWN):14,
        (chess.PAWN, chess.PAWN):15
    }

    def move_score(self, board, move):
        attacker = board.piece_at(move.from_square)
        victim = board.piece_at(move.to_square)
        if victim is None:
            return 0
        return self.capture_score.get((attacker.piece_type, victim.piece_type))

    ########## KEY LOGIC OF SEARCHING IN GAME TREE ##########
    def minimax(self, board, depth, alpha, beta, maximizing_color, capture_square=None):
        self.minimax_nodes_searched += 1
        
        if board.is_game_over():
            return self.evaluation.evaluate(board)

        if maximizing_color == chess.WHITE:
            if depth == 0:
                eval = self.capture_minimax(board, self.conf.config["capture_depth"], chess.BLACK)
                return eval

            max_eval = float('-inf')
            legal_moves = list(board.legal_moves)
            legal_moves.sort(reverse=True, key=lambda x: self.move_score(board, x))

            for move in legal_moves:
                board.push(move)
                eval = self.minimax(board, depth - 1, alpha, beta, chess.BLACK, move.to_square)
                board.pop()
                max_eval = max(max_eval, eval)
                alpha = max(alpha, max_eval)
                if beta <= alpha:
                    break
            if depth == 1 and capture_square is not None and max_eval == float('-inf'):
                return self.evaluation.evaluate(board)
            return max_eval

        else:
            if depth == 0:
                eval = self.capture_minimax(board, self.conf.config["capture_depth"], chess.WHITE)
                return eval

            min_eval = float('inf')
            legal_moves = list(board.legal_moves)
            legal_moves.sort(reverse=True, key=lambda x: self.move_score(board, x))

            for move in legal_moves:
                board.push(move)
                eval = self.minimax(board, depth - 1, alpha, beta, chess.WHITE, move.to_square)
                board.pop()
                min_eval = min(min_eval, eval)
                beta = min(beta, min_eval)
                if beta <= alpha:
                    break
            if depth == 1 and capture_square is not None and min_eval == float('inf'):
                return self.evaluation.evaluate(board)
            return min_eval