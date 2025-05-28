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

bitboard random64()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    return dis(gen);
}

bitboard generateMagics(int square)
{
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
