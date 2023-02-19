#include <ctype.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

enum Piece
{
    PAWN_WHITE = 9,
    KNIGHT_WHITE = 10,
    BISHOP_WHITE = 11,
    ROOK_WHITE = 12,
    QUEEN_WHITE = 13,
    KING_WHITE = 14,
    PAWN_BLACK = 1,
    KNIGHT_BLACK = 2,
    BISHOP_BLACK = 3,
    ROOK_BLACK = 4,
    QUEEN_BLACK = 5,
    KING_BLACK = 6,
    EMPTY = 0,
    INVALID = -1
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
    return (square[1] - 47) * 10 + (square[0] - 95);
}

std::string numberToPiece(int piece)
{
    switch (piece & 0b1111)
    {
        case EMPTY:
            return " ";
            break;
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

struct CastlingRights
{
    bool blackKingSide = true;
    bool blackQueenSide = true;
    bool whiteKingSide = true;
    bool whiteQueenSide = true;
};

struct Move
{
    int startSquare;
    int endSquare;
};

class Game
{
  public:
    int board[120] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 0, 0, 0, 0, 0, 0, 0, 0, -1,
        -1, 0, 0, 0, 0, 0, 0, 0, 0, -1,
        -1, 0, 0, 0, 0, 0, 0, 0, 0, -1,
        -1, 0, 0, 0, 0, 0, 0, 0, 0, -1,
        -1, 0, 0, 0, 0, 0, 0, 0, 0, -1,
        -1, 0, 0, 0, 0, 0, 0, 0, 0, -1,
        -1, 0, 0, 0, 0, 0, 0, 0, 0, -1,
        -1, 0, 0, 0, 0, 0, 0, 0, 0, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

    int currentTurn = 1; // 1 for white, -1 for black
    CastlingRights castlingAvailability;
    int enpassant = 0;
    int halfMoves = 0;

    Game()
    {
    }

    Game(std::string fen)
    {
        setFromFen(fen);
    }

    void setFromFen(std::string fen)
    {
        std::vector<std::string> fenParts = splitString(fen, ' ');

        if (fenParts.size() != 6 || fenParts[1].size() != 1)
        {
            std::cerr << "Hurr durr, huono fen (fen väärän pituinen)" << std::endl;
            return;
        }

        for (int i = 2; i < 10; i++) // clear board
        {
            for (int j = 1; j < 9; j++)
            {
                board[i * 10 + j] = EMPTY;
            }
        }

        int currentSquare = 21;
        for (char fenPart : fenParts[0])
        {
            switch (fenPart)
            {
                case 'P':
                    board[currentSquare] = PAWN_WHITE;
                    currentSquare++;
                    break;
                case 'p':
                    board[currentSquare] = PAWN_BLACK;
                    currentSquare++;
                    break;
                case 'N':
                    board[currentSquare] = KNIGHT_WHITE;
                    currentSquare++;
                    break;
                case 'n':
                    board[currentSquare] = KNIGHT_BLACK;
                    currentSquare++;
                    break;
                case 'B':
                    board[currentSquare] = BISHOP_WHITE;
                    currentSquare++;
                    break;
                case 'b':
                    board[currentSquare] = BISHOP_BLACK;
                    currentSquare++;
                    break;
                case 'R':
                    board[currentSquare] = ROOK_WHITE;
                    currentSquare++;
                    break;
                case 'r':
                    board[currentSquare] = ROOK_BLACK;
                    currentSquare++;
                    break;
                case 'Q':
                    board[currentSquare] = QUEEN_WHITE;
                    currentSquare++;
                    break;
                case 'q':
                    board[currentSquare] = QUEEN_BLACK;
                    currentSquare++;
                    break;
                case 'K':
                    board[currentSquare] = KING_WHITE;
                    currentSquare++;
                    break;
                case 'k':
                    board[currentSquare] = KING_BLACK;
                    currentSquare++;
                    break;
                case '/':
                    currentSquare += 2;
                    break;
                case '1':
                    currentSquare += 1;
                    break;
                case '2':
                    currentSquare += 2;
                    break;
                case '3':
                    currentSquare += 3;
                    break;
                case '4':
                    currentSquare += 4;
                    break;
                case '5':
                    currentSquare += 5;
                    break;
                case '6':
                    currentSquare += 6;
                    break;
                case '7':
                    currentSquare += 7;
                    break;
                case '8':
                    currentSquare += 8;
                    break;
            }
        }

        switch (fenParts[1][0])
        {
            case 'w':
                currentTurn = 1;
                break;
            case 'b':
                currentTurn = -1;
                break;
            default:
                std::cerr << "Hurr durr, huono fen (Vuoroindikaattori)" << std::endl;
                break;
        }

        for (char castlingDirection : fenParts[2])
        {
            switch (castlingDirection)
            {
                case 'K':
                    castlingAvailability.whiteKingSide = true;
                    break;
                case 'Q':
                    castlingAvailability.whiteKingSide = true;
                    break;
                case 'k':
                    castlingAvailability.blackKingSide = true;
                    break;
                case 'q':
                    castlingAvailability.blackQueenSide = true;
                    break;
                case '-':
                    break;
                default:
                    std::cerr << "Hurr durr, huono fen (linnoittautumis suunnat)" << std::endl;
                    break;
            }
        }

        if (fenParts[3] != "-")
            enpassant = squareToIndex(fenParts[3]);
        else
            enpassant = -1;
    }

    void print()
    {
        std::string row = "+-----+-----+-----+-----+-----+-----+-----+-----+";
        for (int i = 2; i < 10; i++)
        {
            std::cout << row << '\n';
            std::cout << "|";

            for (int j = 1; j < 9; j++)
            {
                std::cout << "  " << numberToPiece(board[i * 10 + j]) << "  ";
                std::cout << "|";
            }
            std::cout << std::endl;
        }
        std::cout << row << std::endl;
    }
};

int main()
{
    Game* game = new Game();
    game->print();
}
