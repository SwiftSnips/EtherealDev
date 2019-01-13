/*
  Ethereal is a UCI chess playing engine authored by Andrew Grant.
  <https://github.com/AndyGrant/Ethereal>     <andrew@grantnet.us>

  Ethereal is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Ethereal is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "assert.h"
#include "bitboards.h"
#include "evaluate.h"
#include "psqt.h"
#include "types.h"

int PSQT[32][SQUARE_NB];

#define S(mg, eg) MakeScore((mg), (eg))

const int PawnPSQT32[32] = {
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
    S( -16,   7), S(   9,   1), S(  -6,   4), S(  -3,  -3),
    S( -20,   3), S(  -1,   0), S(  -6,  -4), S(  -4, -13),
    S( -14,  14), S(  -1,   7), S(  16, -10), S(  12, -19),
    S(  -2,  19), S(   7,   8), S(   6,   0), S(  16, -18),
    S(   0,  27), S(  10,  25), S(  16,   3), S(  25, -20),
    S( -43,   6), S( -34,   8), S(  -1, -16), S(   1, -31),
    S(   0,   0), S(   0,   0), S(   0,   0), S(   0,   0),
};

const int KnightPSQT32[32] = {
    S( -39, -47), S(  -2, -37), S( -11, -15), S(   2,  -7),
    S(   3, -48), S(   6, -11), S(   4, -22), S(  12,  -3),
    S(   0, -23), S(  22, -18), S(  14,   1), S(  28,  12),
    S(   6,   5), S(  27,   7), S(  34,  31), S(  45,  31),
    S(  28,   5), S(  43,  12), S(  44,  40), S(  49,  43),
    S( -27,   8), S(  25,   4), S(  40,  37), S(  48,  33),
    S( -33, -21), S( -35,   4), S(  40, -32), S(  13,  -1),
    S(-169, -34), S(-102, -30), S(-156,  -6), S( -39, -26),
};

const int BishopPSQT32[32] = {
    S(  24, -22), S(  19, -27), S(  -7,  -9), S(  16, -15),
    S(  33, -28), S(  27, -25), S(  23, -13), S(  10,  -2),
    S(  22, -11), S(  30, -11), S(  19,   0), S(  24,   6),
    S(   9,  -5), S(  18,   0), S(  18,  13), S(  31,  17),
    S( -13,  10), S(  34,   2), S(   4,  16), S(  29,  19),
    S(   0,   5), S(   0,   7), S(  26,   7), S(  21,   6),
    S( -68,   1), S(  -3,  -3), S(  -8, -10), S( -39,   0),
    S( -49,   0), S( -61,  -1), S(-125,   3), S(-110,  10),
};

const int RookPSQT32[32] = {
    S(  -3, -30), S(  -9, -17), S(   4, -19), S(  10, -26),
    S( -41, -25), S(  -7, -29), S(  -2, -24), S(   4, -32),
    S( -18, -19), S(   4, -14), S(  -3, -20), S(   0, -21),
    S( -15,   0), S(  -9,   4), S(  -1,   2), S(   0,   3),
    S(  -9,  12), S(  -7,  11), S(  17,   5), S(  21,   7),
    S( -14,  15), S(  15,   9), S(  10,  13), S(  18,  12),
    S(  -1,  15), S(  -6,  16), S(  33,   0), S(  20,   7),
    S(   0,  23), S(  11,  14), S( -21,  23), S(   3,  27),
};

const int QueenPSQT32[32] = {
    S(   0, -46), S( -13, -30), S(  -4, -21), S(   8, -40),
    S(   7, -46), S(  13, -37), S(  19, -54), S(   8, -14),
    S(   5, -21), S(  22, -16), S(   5,   5), S(   1,   0),
    S(   4,  -4), S(  10,   4), S(  -4,  15), S(  -6,  45),
    S(  -9,  10), S( -12,  33), S(  -7,  21), S( -21,  51),
    S( -11,   3), S(  -4,  18), S(   0,  20), S(  -9,  45),
    S(  -3,  12), S( -73,  55), S(  21,  10), S( -19,  65),
    S( -19, -21), S(   2, -11), S(   8,  -3), S( -17,   9),
};

const int KingPSQT32[32] = {
    S(  79,-106), S(  96, -80), S(  35, -33), S(  21, -39),
    S(  66, -54), S(  54, -43), S(   5,  -1), S( -16,   5),
    S(   0, -41), S(  44, -28), S(  16,   2), S( -11,  20),
    S( -52, -33), S(  33, -20), S(   1,  17), S( -44,  37),
    S( -18, -19), S(  53,   0), S(   8,  30), S( -29,  37),
    S(  37, -18), S(  83,  -1), S(  74,  18), S(   9,  16),
    S(  14, -17), S(  49,  -4), S(  33,   0), S(   7,   0),
    S(  26, -81), S(  83, -67), S( -21, -34), S( -15, -35),
};

#undef S

int relativeSquare32(int s, int c) {
    assert(0 <= c && c < COLOUR_NB);
    assert(0 <= s && s < SQUARE_NB);
    static const int edgeDistance[FILE_NB] = {0, 1, 2, 3, 3, 2, 1, 0};
    return 4 * relativeRankOf(c, s) + edgeDistance[fileOf(s)];
}

void initializePSQT() {

    for (int s = 0; s < SQUARE_NB; s++) {
        const int w32 = relativeSquare32(s, WHITE);
        const int b32 = relativeSquare32(s, BLACK);

        PSQT[WHITE_PAWN  ][s] = +MakeScore(PieceValues[PAWN  ][MG], PieceValues[PAWN  ][EG]) +   PawnPSQT32[w32];
        PSQT[WHITE_KNIGHT][s] = +MakeScore(PieceValues[KNIGHT][MG], PieceValues[KNIGHT][EG]) + KnightPSQT32[w32];
        PSQT[WHITE_BISHOP][s] = +MakeScore(PieceValues[BISHOP][MG], PieceValues[BISHOP][EG]) + BishopPSQT32[w32];
        PSQT[WHITE_ROOK  ][s] = +MakeScore(PieceValues[ROOK  ][MG], PieceValues[ROOK  ][EG]) +   RookPSQT32[w32];
        PSQT[WHITE_QUEEN ][s] = +MakeScore(PieceValues[QUEEN ][MG], PieceValues[QUEEN ][EG]) +  QueenPSQT32[w32];
        PSQT[WHITE_KING  ][s] = +MakeScore(PieceValues[KING  ][MG], PieceValues[KING  ][EG]) +   KingPSQT32[w32];

        PSQT[BLACK_PAWN  ][s] = -MakeScore(PieceValues[PAWN  ][MG], PieceValues[PAWN  ][EG]) -   PawnPSQT32[b32];
        PSQT[BLACK_KNIGHT][s] = -MakeScore(PieceValues[KNIGHT][MG], PieceValues[KNIGHT][EG]) - KnightPSQT32[b32];
        PSQT[BLACK_BISHOP][s] = -MakeScore(PieceValues[BISHOP][MG], PieceValues[BISHOP][EG]) - BishopPSQT32[b32];
        PSQT[BLACK_ROOK  ][s] = -MakeScore(PieceValues[ROOK  ][MG], PieceValues[ROOK  ][EG]) -   RookPSQT32[b32];
        PSQT[BLACK_QUEEN ][s] = -MakeScore(PieceValues[QUEEN ][MG], PieceValues[QUEEN ][EG]) -  QueenPSQT32[b32];
        PSQT[BLACK_KING  ][s] = -MakeScore(PieceValues[KING  ][MG], PieceValues[KING  ][EG]) -   KingPSQT32[b32];
    }
}
