#pragma once

#include <random>

#include "board.hpp"

class Nona7{
public:
    Nona7(Board::Grid grid) : grid(grid){ }
    Dir decideDir() const;

    using Grid = Board::Grid;
    static int staticEval(Grid);
    static bool nurseryTime(Grid);
    static int const constexpr MATURED = 10;
    static int const constexpr SPACE_WEIGHT = 500;
    struct CompStatic{
        bool operator()(std::pair<Grid, Dir> a, std::pair<Grid, Dir> b){
            return staticEval(a.first) < staticEval(b.first);
        }
        bool operator()(Grid a, Grid b){
            return staticEval(a) < staticEval(b);
        }
    };
    struct CompGrid{
        bool operator()(std::pair<Grid, Dir> a, std::pair<Grid, Dir> b){
            return a.first < b.first;
        }
    };
    struct CompEqual{
        bool operator()(std::pair<Grid, Dir> a, std::pair<Grid, Dir> b){
            return a.first == b.first;
        }
    };
    using GridList = std::vector<Grid>;
    using GridMap = std::vector<std::pair<Grid, Dir>>;
    static GridMap nextPossibleWorld(Grid);
    static GridList nextPossibleWorldLeft(Grid);
    static void uniq(GridMap&);
private:
    Grid grid;
    static int sqrt(int);
};

void Nona7::uniq(Nona7::GridMap& list) {
    auto newEnd = std::unique(std::begin(list), std::end(list), CompEqual());
    list.erase(newEnd, std::end(list));
}

Dir Nona7::decideDir() const{
    using std::make_pair;
    auto npw = nextPossibleWorld(grid);
    auto top = npw;
    decltype(npw) npw2, npw3, npw4, npw5, npw6;
    // if(npw.empty()) std::cout << "!!!!!!!!!!!!!!!!" << std::endl;
    // for(auto e: npw) {
    //     std::cout << "!!----------------!!" << std::endl;
    //     Board::show(e.first);
    //     std::cout << "!!----------------!!" << std::endl;
    // }
    std::cout << "size of 1: " << npw.size() << std::endl;
    uniq(npw);
    npw2.reserve(1024 * 12);
    for(auto e: npw){
        for(auto e2: nextPossibleWorld(e.first))
            npw2.push_back(make_pair(e2.first, e.second));
    }
    if(npw2.empty()) {
        top = std::move(npw);
        goto empty;
    }
    std::cout << "size of 2: " << npw2.size() << std::endl;
    uniq(npw2);
    std::cout << "size of 2: " << npw2.size() << std::endl;
    // npw3.reserve(1024 * 20);
    for(auto e: npw2){
        for(auto e2: nextPossibleWorld(e.first))
            npw3.push_back(make_pair(e2.first, e.second));
    }
    if(npw3.empty()) {
        top = std::move(npw);
        goto empty;
    }
    top = npw3;
    std::cout << "size of 3: " << npw3.size() << std::endl;
    uniq(npw3);
    std::cout << "size of 3: " << npw3.size() << std::endl;
    if(! nurseryTime(grid)) {
        npw4.reserve(1024 * 512);
        for(auto const& e: npw3){
            for(auto const& e2: nextPossibleWorld(e.first))
                npw4.push_back(make_pair(e2.first, e.second));
        }
        if(npw4.empty()) {
            top = std::move(npw);
            goto empty;
        }
        top = npw4;
        uniq(npw4);
        for(auto const& e: npw4){
            for(auto const& e2: nextPossibleWorld(e.first))
                npw5.push_back(make_pair(e2.first, e.second));
        }
        if(npw5.empty()) goto empty;
        top = npw5;
        for(auto const& e: npw5){
            for(auto const& e2: nextPossibleWorld(e.first))
                npw6.push_back(make_pair(e2.first, e.second));
        }
        if(npw6.empty()) goto empty;
        top = npw6;

    }
    empty: ;
    return (std::max_element(std::begin(top), std::end(top), CompStatic()))->second;
}

int Nona7::staticEval(Board::Grid grid){
    static int const valss[4][4][4] = {
	{
	    {80, 40, 40, 40},
	    {30, 10, 10, 10},
	    {03, 01, 01, 03},
	    {01, 01, 01, 01}
	},
	{
	    {40, 40, 40, 80},
	    {10, 10, 10, 30},
	    {03, 01, 01, 03},
	    {01, 01, 01, 01}
	},
	{
	    {80, 30, 03, 01},
	    {40, 10, 01, 01},
	    {40, 10, 01, 01},
	    {40, 10, 03, 01}
	},
	{
	    {01, 03, 30, 80},
	    {01, 01, 10, 40}, 
	    {01, 01, 10, 40},
	    {01, 03, 10, 40}
	}
    };
    std::array<int, 4> sums;
     sums.fill(0);
    for(int i(0); i < 4; ++i)
	for(int j(0); j < 4; ++j)
	    for(int k(0); k < 4; ++k)
		sums[k] += Board::pow2(Board::get(grid, i, j)) * sqrt(Board::pow2(Board::get(grid, i, j))) * valss[k][i][j];
    return *(std::max_element(std::begin(sums), std::end(sums)));
}

int Nona7::sqrt(int i){
    switch(i){
    case 0:
	return 0;
    case 2:
	return 1;
    case 4:
	return 2;
    case 8:
	return 3;
    case 16:
	return 4;
    case 32:
	return 1;
    case 64:
	return 8;
    case 128:
	return 11;
    case 256:
	return 17;
    case 512:
	return 23;
    case 1024:
	return 32;
    case 2048:
	return 45;
    case 4096:
	return 64;
    case 8192:
	return 91;
    case 16384:
	return 128;
    default: 
	return 181;
    };
}

bool Nona7::nurseryTime(Board::Grid grid){
    for(int i(0); i < 4;++i)
        for(int j(0); j < 4;++j)
            if(Board::get(grid, i, j) >= MATURED) return false;
    return true;
}

Nona7::GridMap Nona7::nextPossibleWorld(Board::Grid grid){
    GridMap map;
    GridList lefts = nextPossibleWorldLeft(grid);
    for(auto e: lefts) map.push_back(std::make_pair(e, Dir::Left));
    GridList right = nextPossibleWorldLeft(Board::gridMirror(grid));
    for(auto e: right) map.push_back(std::make_pair(Board::gridMirror(e), Dir::Right));
    GridList down = nextPossibleWorldLeft(Board::gridMirror(Board::transpose(grid)));
    for(auto e: down) map.push_back(std::make_pair(Board::transpose(Board::gridMirror(e)), Dir::Down));
    GridList up = nextPossibleWorldLeft(Board::transpose(grid));
    for(auto e: up) map.push_back(std::make_pair(Board::transpose(e), Dir::Up));
    return map;
}

Nona7::GridList Nona7::nextPossibleWorldLeft(Board::Grid grid){
    auto left = Board::moveLeft(grid);
    if(left == grid) return {};
    for(int i(0); i < 4; ++i)
        for(int j(0); j < 4; ++j)
            if(Board::get(left, i, j) == 0)
                left = Board::set(left, i, j, 1);
    // printf("####----####----####\n");
    // Board::show(left);
    // printf("####----####----####\n");
    return { left };
}
