#include <ctype.h>
#include <unistd.h>

#include <bitset>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

typedef uint64_t bitboard;

const uint64_t RMagic[64] = {
    0x2080020500400f0ULL,
    0x28444000400010ULL,
    0x20000a1004100014ULL,
    0x20010c090202006ULL,
    0x8408008200810004ULL,
    0x1746000808002ULL,
    0x2200098000808201ULL,
    0x12c0002080200041ULL,
    0x104000208e480804ULL,
    0x8084014008281008ULL,
    0x4200810910500410ULL,
    0x100014481c20400cULL,
    0x4014a4040020808ULL,
    0x401002001010a4ULL,
    0x202000500010001ULL,
    0x8112808005810081ULL,
    0x40902108802020ULL,
    0x42002101008101ULL,
    0x459442200810c202ULL,
    0x81001103309808ULL,
    0x8110000080102ULL,
    0x8812806008080404ULL,
    0x104020000800101ULL,
    0x40a1048000028201ULL,
    0x4100ba0000004081ULL,
    0x44803a4003400109ULL,
    0xa010a00000030443ULL,
    0x91021a000100409ULL,
    0x4201e8040880a012ULL,
    0x22a000440201802ULL,
    0x30890a72000204ULL,
    0x10411402a0c482ULL,
    0x40004841102088ULL,
    0x40230000100040ULL,
    0x40100010000a0488ULL,
    0x1410100200050844ULL,
    0x100090808508411ULL,
    0x1410040024001142ULL,
    0x8840018001214002ULL,
    0x410201000098001ULL,
    0x8400802120088848ULL,
    0x2060080000021004ULL,
    0x82101002000d0022ULL,
    0x1001101001008241ULL,
    0x9040411808040102ULL,
    0x600800480009042ULL,
    0x1a020000040205ULL,
    0x4200404040505199ULL,
    0x2020081040080080ULL,
    0x40a3002000544108ULL,
    0x4501100800148402ULL,
    0x81440280100224ULL,
    0x88008000000804ULL,
    0x8084060000002812ULL,
    0x1840201000108312ULL,
    0x5080202000000141ULL,
    0x1042a180880281ULL,
    0x900802900c01040ULL,
    0x8205104104120ULL,
    0x9004220000440aULL,
    0x8029510200708ULL,
    0x8008440100404241ULL,
    0x2420001111000bdULL,
    0x4000882304000041ULL,
};
  
const uint64_t BMagic[64] = {
    0x100420000431024ULL,
    0x280800101073404ULL,
    0x42000a00840802ULL,
    0xca800c0410c2ULL,
    0x81004290941c20ULL,
    0x400200450020250ULL,
    0x444a019204022084ULL,
    0x88610802202109aULL,
    0x11210a0800086008ULL,
    0x400a08c08802801ULL,
    0x1301a0500111c808ULL,
    0x1280100480180404ULL,
    0x720009020028445ULL,
    0x91880a9000010a01ULL,
    0x31200940150802b2ULL,
    0x5119080c20000602ULL,
    0x242400a002448023ULL,
    0x4819006001200008ULL,
    0x222c10400020090ULL,
    0x302008420409004ULL,
    0x504200070009045ULL,
    0x210071240c02046ULL,
    0x1182219000022611ULL,
    0x400c50000005801ULL,
    0x4004010000113100ULL,
    0x2008121604819400ULL,
    0xc4a4010000290101ULL,
    0x404a000888004802ULL,
    0x8820c004105010ULL,
    0x28280100908300ULL,
    0x4c013189c0320a80ULL,
    0x42008080042080ULL,
    0x90803000c080840ULL,
    0x2180001028220ULL,
    0x1084002a040036ULL,
    0x212009200401ULL,
    0x128110040c84a84ULL,
    0x81488020022802ULL,
    0x8c0014100181ULL,
    0x2222013020082ULL,
    0xa00100002382c03ULL,
    0x1000280001005c02ULL,
    0x84801010000114cULL,
    0x480410048000084ULL,
    0x21204420080020aULL,
    0x2020010000424a10ULL,
    0x240041021d500141ULL,
    0x420844000280214ULL,
    0x29084a280042108ULL,
    0x84102a8080a20a49ULL,
    0x104204908010212ULL,
    0x40a20280081860c1ULL,
    0x3044000200121004ULL,
    0x1001008807081122ULL,
    0x50066c000210811ULL,
    0xe3001240f8a106ULL,
    0x940c0204030020d4ULL,
    0x619204000210826aULL,
    0x2010438002b00a2ULL,
    0x884042004005802ULL,
    0xa90240000006404ULL,
    0x500d082244010008ULL,
    0x28190d00040014e0ULL,
    0x825201600c082444ULL,
};

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

void printBitboard(bitboard board) {
    for (int row = 7; row >= 0; --row) {   // Loop through rows (from row 7 to row 0)
        for (int col = 0; col < 8; ++col) {  // Loop through columns
            int square = row * 8 + col;      // Calculate the square index
            std::cout << ((board >> square) & 1) << " ";  // Print the bit for the square
        }
        std::cout << std::endl;  // Move to the next row
    }
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

            // Knight moves
            bitboard notAFile = 0xfefefefefefefefe;
            bitboard notHFile = 0x7f7f7f7f7f7f7f7f;
            bitboard notABFile = 0xfcfcfcfcfcfcfcfc;
            bitboard notHGFile = 0x3f3f3f3f3f3f3f3f;
            
            bitboard knightSquare = 1;

            for (int i = 0; i<64; i++)
            {
                if (bitboards[KNIGHT_WHITE] & knightSquare)
                {
                    bitboard targetSquares = 0;
                    
                    targetSquares |= ((knightSquare & notHFile) << 17) & emptySquares; // NNE
                    targetSquares |= ((knightSquare & notHGFile) << 10) & emptySquares; // NEE
                    targetSquares |= ((knightSquare & notHGFile) >> 6) & emptySquares; // SEE
                    targetSquares |= ((knightSquare & notHFile) >> 15) & emptySquares; // SSE
                    targetSquares |= ((knightSquare & notAFile) << 15) & emptySquares; // NNW  
                    targetSquares |= ((knightSquare & notABFile) << 6) & emptySquares; // NWW
                    targetSquares |= ((knightSquare & notABFile) >> 10) & emptySquares; // SWW
                    targetSquares |= ((knightSquare & notAFile) >> 17) & emptySquares; //SSW


                    for (int j = 0; j<64; ++j)
                    {
                        if (targetSquares & bitboard(1) << j)
                        {
                            moves.push_back(*new Move{i, j});
                        }
                    }

                }
                 
                knightSquare <<= 1;
            }
        }
        else // Black 
        {
            //Pawn pushes
            bitboard singlePushTargets = bitboards[PAWN_BLACK]<<8 & emptySquares;
            std::bitset<64> singlePushTargetsBS(singlePushTargets);

            for (int i = 0; i < 64; i++)
            {
                if(singlePushTargetsBS[i])
                {
                    moves.push_back(*new Move{i-8, i});
                }
            }

            bitboard doublePushTargets = singlePushTargets<<8 & emptySquares & 0x00000000FF000000;
            std::bitset<64> doublePushTargetsBS(doublePushTargets);

            for (int i = 0; i < 64; i++)
            {
                if(doublePushTargetsBS[i])
                {
                    moves.push_back(*new Move{i-16, i});
                }
            }

            // Knight moves
            bitboard notAFile = 0xfefefefefefefefe;
            bitboard notHFile = 0x7f7f7f7f7f7f7f7f;
            bitboard notABFile = 0xfcfcfcfcfcfcfcfc;
            bitboard notHGFile = 0x3f3f3f3f3f3f3f3f;
            
            bitboard knightSquare = 1;

            for (int i = 0; i<64; i++)
            {
                if (bitboards[KNIGHT_BLACK] & knightSquare)
                {
                    bitboard targetSquares = 0;
                    
                    targetSquares |= ((knightSquare & notHFile) << 17) & emptySquares; // NNE
                    targetSquares |= ((knightSquare & notHGFile) << 10) & emptySquares; // NEE
                    targetSquares |= ((knightSquare & notHGFile) >> 6) & emptySquares; // SEE
                    targetSquares |= ((knightSquare & notHFile) >> 15) & emptySquares; // SSE
                    targetSquares |= ((knightSquare & notAFile) << 15) & emptySquares; // NNW  
                    targetSquares |= ((knightSquare & notABFile) << 6) & emptySquares; // NWW
                    targetSquares |= ((knightSquare & notABFile) >> 10) & emptySquares; // SWW
                    targetSquares |= ((knightSquare & notAFile) >> 17) & emptySquares; //SSW


                    for (int j = 0; j<64; ++j)
                    {
                        if (targetSquares & bitboard(1) << j)
                        {
                            moves.push_back(*new Move{i, j});
                        }
                    }

                }
                 
                knightSquare <<= 1;
            }
        }

        return moves;
    }
};

int main()
{
    Game *game = new Game(/** "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"*/);
    game->print();
    std::vector<Move> moves = game->getMoves();
    for (Move move : moves) 
    {
        std::cout << move.startSquare << " " << move.endSquare << std::endl;
    }
}
