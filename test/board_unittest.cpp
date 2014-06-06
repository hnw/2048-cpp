#include <limits.h>
#include "../board.hpp"
#include "gtest/gtest.h"
#include "test.hpp"

#define ITERATION 10000000

namespace{
    static Board::Grid const grid0to15 = test::make0to15();
}

TEST(BoardTest, log2){
    EXPECT_EQ(0, Board::log2(0));
    EXPECT_EQ(1, Board::log2(2));
    EXPECT_EQ(2, Board::log2(4));
    EXPECT_EQ(3, Board::log2(8));
    EXPECT_EQ(4, Board::log2(16));
    EXPECT_EQ(5, Board::log2(32));
    EXPECT_EQ(6, Board::log2(64));
    EXPECT_EQ(7, Board::log2(128));
    EXPECT_EQ(8, Board::log2(256));
    EXPECT_EQ(9, Board::log2(512));
    EXPECT_EQ(10, Board::log2(1024));
    EXPECT_EQ(11, Board::log2(2048));
    EXPECT_EQ(12, Board::log2(4096));
    EXPECT_EQ(13, Board::log2(8192));
    EXPECT_EQ(14, Board::log2(16384));
    EXPECT_EQ(15, Board::log2(32768));
    EXPECT_EQ(16, Board::log2(65536));
}

TEST(BoardTest, pow2){
    EXPECT_EQ(0, Board::pow2(0));
    EXPECT_EQ(2, Board::pow2(1));
    EXPECT_EQ(4, Board::pow2(2));
    EXPECT_EQ(8, Board::pow2(3));
    EXPECT_EQ(16, Board::pow2(4));
    EXPECT_EQ(32, Board::pow2(5));
    EXPECT_EQ(64, Board::pow2(6));
    EXPECT_EQ(128, Board::pow2(7));
    EXPECT_EQ(256, Board::pow2(8));
    EXPECT_EQ(512, Board::pow2(9));
    EXPECT_EQ(1024, Board::pow2(10));
    EXPECT_EQ(2048, Board::pow2(11));
    EXPECT_EQ(4096, Board::pow2(12));
    EXPECT_EQ(8192, Board::pow2(13));
    EXPECT_EQ(16384, Board::pow2(14));
    EXPECT_EQ(32768, Board::pow2(15));
    EXPECT_EQ(65536, Board::pow2(16));
}

TEST(BoardTest, gridMirrorIDRAND){
    std::mt19937 mt = test::mtInit();

    for(int i(0); i < ITERATION; ++i){
        Board::Grid grid = mt();
        EXPECT_TRUE(grid == Board::gridMirror( Board::gridMirror( grid )));
    }
}

TEST(BoardTest, gridMirror0to15){
    int xs[] = {
	3, 2, 1, 0,
	7, 6, 5, 4,
	11, 10, 9, 8,
	15, 14, 13, 12
    };
    Board::Grid grid = 0;
    for(int i(0); i < 16; ++i){
	grid = Board::set(grid, i / 4, i % 4, xs[i]);
    }
    EXPECT_EQ(grid, Board::gridMirror(grid0to15));
}

TEST(BoardTest, flipVIDRAND){
    std::mt19937 mt = test::mtInit();

    for(int i(0); i < ITERATION; ++i){
        Board::Grid grid = mt();
        EXPECT_TRUE(grid == Board::flipV( Board::flipV( grid )));
    }
}

TEST(BoardTest, flipV0to15){
    int xs[] = {
	12, 13, 14, 15,
	8, 9, 10, 11,
	4, 5, 6, 7,
	0, 1, 2, 3
    };
    Board::Grid grid = 0;
    for(int i(0); i < 16; ++i){
	grid = Board::set(grid, i / 4, i % 4, xs[i]);
    }
    EXPECT_EQ(grid, Board::flipV(grid0to15));
}

TEST(BoardTest, transposeIDRAND){
    std::mt19937 mt = test::mtInit();

    for(int i(0); i < ITERATION; ++i){
        Board::Grid grid = mt();
        EXPECT_TRUE(grid == Board::transpose( Board::transpose( grid )));
    }
}

TEST(BoardTest, popCountRAND){
#ifdef __GNUC__
    std::mt19937 mt = test::mtInit();

    for(int i(0); i < ITERATION; ++i){
        Board::Grid grid = mt();
        EXPECT_EQ(__builtin_popcount(grid) , Board::popCount(grid) );
    }
#endif
}

TEST(BoardTest, movableMovedRAND){
    std::mt19937 mt = test::mtInit();

    for(int i(0); i < ITERATION; ++i){
        Board::Grid grid = mt();
        Dir dir = allDirs[mt()% 4];
        EXPECT_EQ(grid != Board::moved(grid, dir), Board::movable(grid, dir));
    }
}

TEST(BoardTest, countZeroGridRAND){
    std::mt19937 mt = test::mtInit();

    for(int i(0); i < ITERATION; ++i){
        Board::Grid grid = mt();
        int zeros = 0;
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j)
                if(Board::get(grid, i, j) == 0) ++ zeros;
        
        EXPECT_EQ(zeros, Board::countZeroGrid(grid));
    }
}

