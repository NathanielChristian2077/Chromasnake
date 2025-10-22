#pragma once
#include <random>

class RNG {
public:
    static RNG& instance(){
	static RNG r;
	return r;
    }

    int irand(int a, int b) {
	std::uniform_int_distribution<int> d(a,b);
	return d(mt);
    }

    float frand(float a, float b){
	std::uniform_real_distribuition<float> d(a,b);
	return d(mt);
    }

private:
    RNG(): mt(1337) {}
    std::mt19937 mt;
};
