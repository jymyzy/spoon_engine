#include <random>
#include <vector>
#include <set>
typedef uint64_t bitboard;

void printBitboard(bitboard board) {
    for (int row = 7; row >= 0; --row) {
        for (int col = 0; col < 8; ++col) {
            int square = row * 8 + col;
            std::cout << ((board >> square) & 1) << " ";
        }
        std::cout << std::endl;
    }
}

const uint64_t RMagic[64] = {
    108086546749494402,
    9529617155117555720,
    1873506311948222980,
    72093878190083080,
    72066390399656704,
    648537038173373440,
    1297060882005693064,
    4719772548173137152,
    2305983816495366177,
    4904490364085211136,
    13835198861498392576,
    94857239542628608,
    1153273401084379136,
    36169552761063424,
    5333388426740240384,
    52778722408448,
    180149207777148952,
    4638707688166162432,
    585486644331216896,
    1333075385574825984,
    37163527386955776,
    325455626540941312,
    14700101577247555584,
    38295992159698944,
    324541750611804161,
    147510550044082176,
    76570265173098497,
    144748665964068864,
    298521857064173568,
    1478447324299526272,
    3566886100054048784,
    16767820759040,
    4621819395151236098,
    9224251663336870020,
    21399250424301572,
    606350445073400064,
    37183353014714368,
    17695332368384,
    38852356874436608,
    110338740630585344,
    10282634890444800,
    1171296638226923537,
    283124782071808,
    76561228302909440,
    4918005630763532352,
    18058653886578692,
    88029801906176,
    361784510746066944,
    175641023270223873,
    1765974416469524480,
    1207248442864701440,
    2468420784758784,
    666827517054683136,
    90143671300655104,
    5332270823655670784,
    144116579955704320,
    1229520648674510848,
    281560880529408,
    37300141701270048,
    4538803733791008,
    1477216420529504848,
    2377971110047514888,
    1585276483555295748,
    198444258779209986
};
  
const uint64_t BMagic[64] = {
    2286986400432160,
    9012736558170240,
    13661433334071872,
    298439350210790473,
    9368616459879601154,
    2370305324063132672,
    1165491052027904,
    5188429517051921538,
    5514805645860,
    2314876906392092704,
    2310914519548889280,
    596802009483706880,
    9583661176356667394,
    4616191821650362528,
    288863986941214723,
    3379917031478336,
    2252075902174592,
    2308130010849911302,
    4521200814395648,
    1132531405572608,
    1225542341195005952,
    4623507969733298688,
    306394429709881376,
    38497982107649,
    4710819088983785993,
    11530345947499332688,
    11533868179545669713,
    567348402849800,
    288375512634499072,
    849956881600513,
    9295572723114181121,
    72339902255339648,
    9261657925091395113,
    36316181871764481,
    1136899322806336,
    1152925907083854081,
    326513189188014336,
    148638587502731392,
    2344124791465726464,
    9228158219761813700,
    9512747142318393384,
    1267221644976388,
    9228720612258615298,
    108989376366593,
    144194357212218368,
    4521261206093888,
    9016003980167233,
    2309229513621390634,
    5075362890974400,
    2306409277063955520,
    18014542459047940,
    4801118678300493888,
    148970771316998160,
    26457419679744,
    4614518914565865472,
    9800976298435183116,
    396391587820670976,
    2324180116131942400,
    2269671189906948,
    9223653546201749504,
    4720898309962744832,
    144119657560802048,
    17729658814793,
    20292589028901120
  };

int countSetBits(bitboard bb) {
    int count = 0;
    while (bb) {
        if (bb & 1) count++;
        bb >>= 1;
    }
    return count;
}

bitboard generateRookMask(int square) {
    bitboard mask = 0;
    int rank = square / 8;
    int file = square % 8;

    for (int r = rank + 1; r <= 6; r++) mask |= (1ULL << (r * 8 + file));
    for (int r = rank - 1; r >= 1; r--) mask |= (1ULL << (r * 8 + file));
    for (int f = file + 1; f <= 6; f++) mask |= (1ULL << (rank * 8 + f));
    for (int f = file - 1; f >= 1; f--) mask |= (1ULL << (rank * 8 + f));

    return mask;
}

bitboard generateBishopMask(int square) {
    bitboard mask = 0;
    int rank = square / 8;
    int file = square % 8;

    for (int r = rank + 1, f = file + 1; r <= 6 && f <= 6; r++, f++) mask |= (1ULL << (r * 8 + f));
    for (int r = rank + 1, f = file - 1; r <= 6 && f >= 1; r++, f--) mask |= (1ULL << (r * 8 + f));
    for (int r = rank - 1, f = file + 1; r >= 1 && f <= 6; r--, f++) mask |= (1ULL << (r * 8 + f));
    for (int r = rank - 1, f = file - 1; r >= 1 && f >= 1; r--, f--) mask |= (1ULL << (r * 8 + f));
    
    return mask;
}

bitboard rookAttacks(int square, bitboard occupancy) {
    bitboard attacks = 0ULL;
    
    int rank = square / 8;
    int file = square % 8;

    for (int i = square - 8; i >= 0; i -= 8) {
        if (occupancy & (1ULL << i)) {
            attacks |= (1ULL << i); 
            break;  
        }
        attacks |= (1ULL << i);
    }

    for (int i = square + 8; i < 64; i += 8) {
        if (occupancy & (1ULL << i)) {
            attacks |= (1ULL << i); 
            break; 
        }
        attacks |= (1ULL << i); 
    }

    for (int i = square - 1; i >= rank * 8; --i) {
        if (occupancy & (1ULL << i)) {
            attacks |= (1ULL << i); 
            break;  
        }
        attacks |= (1ULL << i); 
    }

    for (int i = square + 1; i <= (rank + 1) * 8; ++i) {
        if (occupancy & (1ULL << i)) {
            attacks |= (1ULL << i); 
            break;  
        }
        attacks |= (1ULL << i);
    }

    return attacks;
}

bitboard bishopAttacks(int square, bitboard occupancy) {
    bitboard attacks = 0ULL;

    int rank = square / 8;
    int file = square % 8;

    for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; ++r, ++f) {
        int i = r * 8 + f;
        attacks |= (1ULL << i);
        if (occupancy & (1ULL << i)) break;
    }

    for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; ++r, --f) {
        int i = r * 8 + f;
        attacks |= (1ULL << i);
        if (occupancy & (1ULL << i)) break;
    }

    for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; --r, ++f) {
        int i = r * 8 + f;
        attacks |= (1ULL << i);
        if (occupancy & (1ULL << i)) break;
    }

    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; --r, --f) {
        int i = r * 8 + f;
        attacks |= (1ULL << i);
        if (occupancy & (1ULL << i)) break;
    }

    return attacks;
}

// This function is used to generate masks of occupied squares from an index
bitboard setOccupancy(int index, int bits_in_mask, bitboard mask) {
    bitboard occupancy = 0ULL;
    int bit = 0;

    for (int sq = 0; sq < 64; sq++) {
        if (mask & (1ULL << sq)) {
            if (index & (1 << bit)) {
                occupancy |= (1ULL << sq);
            }
            bit++;
        }
    }

    return occupancy;
}

bitboard BAttackMasks[64][512];
bitboard RAttackMasks[64][4096];

void precomputeRookMoves(int square) {
    bitboard mask = generateRookMask(square);
    int relevantBits = countSetBits(mask);
    int occupancyVariations = 1 << relevantBits;

    for (int index = 0; index < occupancyVariations; index++) {
        bitboard occupancy = setOccupancy(index, relevantBits, mask);
        bitboard attacks = rookAttacks(square, occupancy);
        
        RAttackMasks[square][(RMagic[square]*occupancy) >> (64-relevantBits)] = attacks;
    }
}

void precomputeBishopMoves(int square) {
    bitboard mask = generateBishopMask(square);
    int relevantBits = countSetBits(mask);
    int occupancyVariations = 1 << relevantBits;

    for (int index = 0; index < occupancyVariations; index++) {
        bitboard occupancy = setOccupancy(index, relevantBits, mask);
        bitboard attacks = bishopAttacks(square, occupancy);
        
        BAttackMasks[square][(BMagic[square]*occupancy) >> (64-relevantBits)] = attacks;
    }
}

bitboard random64()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    return dis(gen);
}

bitboard generateMagics(int square, bool bishop)
{
    if (!bishop) {
        while (true)
        {
            bitboard testArr[4096] = {};
            
            bitboard possibleMagic = random64() & random64() & random64();

            bool foundConfilct = false;
            bitboard mask = generateRookMask(square);
            int relevantBits = countSetBits(mask);
            int occupancyVariations = 1 << relevantBits;

            for (int index = 0; index < occupancyVariations; index++) 
            {
                bitboard occupancy = setOccupancy(index, relevantBits, mask);
                bitboard attacks = rookAttacks(square, occupancy);

                if (testArr[((possibleMagic*occupancy) >> (64-relevantBits))] == 0)
                {
                    testArr[((possibleMagic*occupancy) >> (64-relevantBits))] = attacks;
                } else if (testArr[((possibleMagic*occupancy) >> (64-relevantBits))] != attacks)
                {
                    foundConfilct = true;
                }
            }

            if (!foundConfilct)
            {
                return possibleMagic;
                break;
            }
        }
    }
    
    if (bishop) {
        while (true)
        {
            bitboard testArr[512] = {};
            
            bitboard possibleMagic = random64() & random64() & random64();

            bool foundConfilct = false;
            bitboard mask = generateBishopMask(square);
            int relevantBits = countSetBits(mask);
            int occupancyVariations = 1 << relevantBits;

            for (int index = 0; index < occupancyVariations; index++) 
            {
                bitboard occupancy = setOccupancy(index, relevantBits, mask);
                bitboard attacks = bishopAttacks(square, occupancy);

                if (testArr[((possibleMagic*occupancy) >> (64-relevantBits))] == 0)
                {
                    testArr[((possibleMagic*occupancy) >> (64-relevantBits))] = attacks;
                } else if (testArr[((possibleMagic*occupancy) >> (64-relevantBits))] != attacks)
                {
                    foundConfilct = true;
                }
            }

            if (!foundConfilct)
            {
                return possibleMagic;
                break;
            }
        }
    }
    
}
