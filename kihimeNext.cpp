#include "kihimeNext.hpp"

std::random_device KihimeNext::rnd;
std::mt19937 KihimeNext::mt = KihimeNext::mtInit();

std::mt19937 KihimeNext::mtInit(){
    std::vector<std::uint_least32_t> v(10);
    for(auto& e: v) e = rnd();
    std::seed_seq seq(begin(v), end(v));
    std::mt19937 mt(seq);
    return mt;
}

Dir KihimeNext::decideDir(){
    std::array<unsigned int, 4> cnt, depths;
    cnt.fill(0);
    depths.fill(0);
    std::array<double, 4> aves;
    aves.fill(0);
    Dir max = Dir::Up;
    double maxAve = 0.0;
    bool flg = true;

label:;
    for(int i(0); i < ITERATION; ++i){
        Dir dir = allDirs[mt()%4];
        while(! Board::movable(grid, dir)) dir = allDirs[mt()%4];
        auto moved = Board::moved(grid, dir);
        unsigned depth = toDead(moved, 0);
        depths[dirToInt(dir)] += depth;
        ++cnt[dirToInt(dir)];
    }
    for(int i(0); i < 4; ++i){
        if(cnt[i] == 0){
            aves[i] = 0;
            continue;
        }
        aves[i] = depths[i] / (cnt[i] * 1.0);
    }
    for(int i(0); i < 4; ++i){
        if(aves[i] > maxAve){
            max = allDirs[i];
            maxAve = aves[i];
        }
    }
    if(flg && maxAve < DANGER){
        flg = false;
        maxAve = 0;
        goto label;
    }
    return max;
}

int KihimeNext::toDead(Board::Grid grid, int depth) {
    if(Board::alive(grid)){
        auto dir = allDirs[mt()%4];
        while(! Board::movable(grid, dir)) dir = allDirs[mt()%4];
        auto moved = moveAndBirth(grid, dir);
        return toDead(moved, depth + 1);
    }
    return depth;
}

inline Board::Grid KihimeNext::moveAndBirth(Board::Grid grid, Dir dir){
    auto moved = Board::moved(grid, dir);
    int zeros = Board::countZeroGrid(grid);
    if(zeros > 0){
        int point = mt() % zeros;
        int birth(0);
        if(mt() % 10) birth = 1;
        else birth = 2;
        for(int i(0); i < 4; ++i)
            for(int j(0); j < 4; ++j)
                if(Board::get(moved, i, j) == 0)
                    if(point-- == 0) moved = Board::set(moved, i, j, birth);
    }else{
        // 動けなかった時だけだと思う
    }
    return moved;
}