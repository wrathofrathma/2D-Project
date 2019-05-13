#ifndef HEIGHT_GENERATOR
#define HEIGHT_GENERATOR
#include <random>
#include <chrono>
#include <iostream>
#include <cmath>
using namespace std;

class HeightGenerator {
  private:
    float amplitude;
    int octaves;
    float roughness;
    mt19937_64 random_generator;
    unsigned int seed;
  public:
    HeightGenerator();
    ~HeightGenerator();
    float generateHeight(int x, int y);
    float getNoise(int x, int z);
    float getSmoothNoise(int x, int z);
    float getInterpolatedNoise(float x, float z);
    float interpolate(float a, float b, float blend);

    //Setters and getters for the generator.
    unsigned int getSeed();
    float getAmplitude();
    float getOctaves();
    float getRoughness();
    void setAmplitude(float amp);
    void setOctaves(float o);
    void setRoughness(float r);
    void setSeed(unsigned int s);
};

#endif
