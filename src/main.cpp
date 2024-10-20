#include <ctype.h>
#include <unistd.h>

#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

typedef uint64_t bitboard;

enum Piece
{
    PAWN_WHITE = 0,
    KNIGHT_WHITE = 1,
    BISHOP_WHITE = 2,
    ROOK_WHITE = 3,
    QUEEN_WHITE = 4,
    KING_WHITE = 5,
    PAWN_BLACK = 8,
    KNIGHT_BLACK = 9,
    BISHOP_BLACK = 10,
    ROOK_BLACK = 11,
    QUEEN_BLACK = 12,
    KING_BLACK = 13
};

enum CastlingMasks
{
    WHITE_KINGSIDE = 0b1000,
    WHITE_QUEENSIDE = 0b0100,
    BLACK_KINGSIDE = 0b0010,
    BLACK_QUEENSIDE = 0b0001
};

std::vector<std::string> splitString(std::string string, char splitter)
{
    std::vector<std::string> strings;
    std::string currentString = "";
    string += " ";

    for (char character : string)
    {
        if (character == splitter)
        {
            strings.push_back(currentString);
            currentString = "";
        }
        else
        {
            currentString += character;
        }
    }

    return strings;
}

int squareToIndex(std::string square)
{
    return (square[0] - 97) * 8 + (square[1] - 49);
}

void printBitboard(bitboard board)
{
    std::cout << std::bitset<64> (board) << std::endl;
}

std::string numberToPiece(int piece)
{
    switch (piece & 0b1111)
    {
        case PAWN_WHITE:
            return "♟";
            break;
        case KNIGHT_WHITE:
            return "♞";
            break;
        case BISHOP_WHITE:
            return "♝";
            break;
        case ROOK_WHITE:
            return "♜";
            break;
        case QUEEN_WHITE:
            return "♛";
            break;
        case KING_WHITE:
            return "♚";
            break;
        case PAWN_BLACK:
            return "♙";
            break;
        case KNIGHT_BLACK:
            return "♘";
            break;
        case BISHOP_BLACK:
            return "♗";
            break;
        case ROOK_BLACK:
            return "♖";
            break;
        case QUEEN_BLACK:
            return "♕";
            break;
        case KING_BLACK:
            return "♔";
            break;
        default:
            return "E";
    }
}

struct Move
{
    int startSquare;
    int endSquare;
};

class Game
{
  public:
    int currentTurn = 1; // 1 for white, -1 for black
    uint8_t castlingRights;
    uint64_t enpassant = 0;
    int halfMoves = 0;

    bitboard bitboards[16];

    void setFromFen(std::string fen)
    {
        uint64_t currentSquare = 1;
        std::vector<std::string> fenParts = splitString(fen, ' ');
        for (char &letter : fenParts[0])
        {
            if (isdigit(letter))
            {
                currentSquare = currentSquare << ((letter)-48);
            }
            else
            {
                switch (letter)
                {
                    case 'P':
                        bitboards[PAWN_WHITE] = bitboards[PAWN_WHITE] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'p':
                        bitboards[PAWN_BLACK] = bitboards[PAWN_BLACK] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'R':
                        bitboards[ROOK_WHITE] = bitboards[ROOK_WHITE] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'r':
                        bitboards[ROOK_BLACK] = bitboards[ROOK_BLACK] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'N':
                        bitboards[KNIGHT_WHITE] = bitboards[KNIGHT_WHITE] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'n':
                        bitboards[KNIGHT_BLACK] = bitboards[KNIGHT_BLACK] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'B':
                        bitboards[BISHOP_WHITE] = bitboards[BISHOP_WHITE] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'b':
                        bitboards[BISHOP_BLACK] = bitboards[BISHOP_BLACK] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'Q':
                        bitboards[QUEEN_WHITE] = bitboards[QUEEN_WHITE] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'q':
                        bitboards[QUEEN_BLACK] = bitboards[QUEEN_BLACK] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'K':
                        bitboards[KING_WHITE] = bitboards[KING_WHITE] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case 'k':
                        bitboards[KING_BLACK] = bitboards[KING_BLACK] | currentSquare;
                        currentSquare = currentSquare << 1;
                        break;
                    case '/':
                        break;
                    default:
                        std::cerr << "invalid fen (Includes invalid pieces)" << std::endl;
                }
            }

            if (fenParts[1] == "w")
            {
                currentTurn = 1;
            }
            else
            {
                currentTurn = -1;
            }

            for (char &letter : fenParts[2])
            {
                switch (letter)
                {
                    case 'K':
                        castlingRights = castlingRights | WHITE_KINGSIDE;
                        break;
                    case 'Q':
                        castlingRights = castlingRights | WHITE_QUEENSIDE;
                        break;
                    case 'k':
                        castlingRights = castlingRights | BLACK_KINGSIDE;
                        break;
                    case 'q':
                        castlingRights = castlingRights | BLACK_QUEENSIDE;
                        break;
                }
            }

            if (fenParts[3] == "-")
            {
                continue;
            }
            else
            {
                enpassant = squareToIndex(fenParts[3]);
            }

            halfMoves = stoi(fenParts[4]);
        }
    }

    Game(std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
    {
        setFromFen(fen);
    }

    void print()
    {
        std::string board[64];
        int pieces[12] = {PAWN_WHITE, PAWN_BLACK, KNIGHT_WHITE, KNIGHT_BLACK, BISHOP_WHITE, BISHOP_BLACK, ROOK_WHITE, ROOK_BLACK, QUEEN_WHITE, QUEEN_BLACK, KING_WHITE, KING_BLACK};
        for (int piece : pieces)
        {
            std::bitset<64> pieceLocations(bitboards[piece]);
            for (int i = 0; i < pieceLocations.size(); i++)
            {
                if (pieceLocations[i])
                {
                    board[i] = numberToPiece(piece);
                }
            }
        }

        std::cout << "+-----+-----+-----+-----+-----+-----+-----+-----+" << std::endl;
        for (int i = 0; i < 8; i++)
        {
            std::cout << "|     |     |     |     |     |     |     |     |" << std::endl;
            std::cout << "|";
            for (int j = 0; j < 8; j++)
            {
                std::cout << "  " << (board[i * 8 + j] == "" ? " " : board[i * 8 + j]) << "  ";
                std::cout << "|";
            }
            std::cout << std::endl;
            std::cout << "|     |     |     |     |     |     |     |     |" << std::endl;
            std::cout << "+-----+-----+-----+-----+-----+-----+-----+-----+" << std::endl;
        }
    }

    std::vector<Move> getMoves() 
    {
        std::vector<Move> moves;
        bitboard whitePieces = bitboards[PAWN_WHITE]|bitboards[KNIGHT_WHITE]|bitboards[BISHOP_WHITE]|bitboards[ROOK_WHITE]|bitboards[QUEEN_WHITE]|bitboards[KING_WHITE];
        bitboard blackPieces = bitboards[PAWN_BLACK]|bitboards[KNIGHT_BLACK]|bitboards[BISHOP_BLACK]|bitboards[ROOK_BLACK]|bitboards[QUEEN_BLACK]|bitboards[KING_BLACK];
        bitboard emptySquares = ~(whitePieces|blackPieces);
        
        int offset[6][8] = {
            {   0,   0,  0,  0, 0,  0,  0,  0 },
            { -21, -19,-12, -8, 8, 12, 19, 21 }, /* KNIGHT */
            { -11,  -9,  9, 11, 0,  0,  0,  0 }, /* BISHOP */
            { -10,  -1,  1, 10, 0,  0,  0,  0 }, /* ROOK */
            { -11, -10, -9, -1, 1,  9, 10, 11 }, /* QUEEN */
            { -11, -10, -9, -1, 1,  9, 10, 11 }  /* KING */
        };

        if (currentTurn > 0) // White 
        {
            //Pawn pushes
            bitboard singlePushTargets = bitboards[PAWN_WHITE]>>8 & emptySquares;
            std::bitset<64> singlePushTargetsBS(singlePushTargets);

            for (int i = 0; i < 64; i++)
            {
                if(singlePushTargetsBS[i])
                {
                    moves.push_back(*new Move{i+8, i});
                }
            }

            bitboard doublePushTargets = singlePushTargets>>8 & emptySquares & 0x000000FF00000000;
            std::bitset<64> doublePushTargetsBS(doublePushTargets);

            for (int i = 0; i < 64; i++)
            {
                if(doublePushTargetsBS[i])
                {
                    moves.push_back(*new Move{i+16, i});
                }
            }

        }
        else // Black 
        {

        }

        return moves;
    }
};

int main()
{
    Game *game = new Game();
    game->print();
    std::vector<Move> moves = game->getMoves();
    for (Move move : moves) 
    {
        std::cout << move.startSquare << " " << move.endSquare << std::endl;
    }
}
