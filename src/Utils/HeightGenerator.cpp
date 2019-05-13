#include "HeightGenerator.hpp"
#include "../graphics/GraphicsDefinitions.hpp"
HeightGenerator::HeightGenerator(){
  seed = std::chrono::system_clock::now().time_since_epoch().count();
  random_generator.seed(seed);
  amplitude = 30.0f;
  octaves = 3;
  roughness = 0.3f;
}

HeightGenerator::~HeightGenerator(){

}

/**
\brief Generates a random height for coordinate x,z.
\param x --- x coordinate.
\param z --- z coordinate.
*/
float HeightGenerator::generateHeight(int x, int z){
  float total = 0;
  float d = pow(2,octaves);
  float freq=0;
  float amp=0;
  for(int i =0; i<octaves; i++){
    freq = pow(2,i)/d;
    amp = pow(roughness,i)*amplitude;
    total+=getInterpolatedNoise(x*freq, z*freq) * amp;
  }

  return total;
}

/**
\brief Returns noise that is smoothed according to the surrounding coordinates.
\param x --- x coordinate.
\param z --- z coordinate.
*/
float HeightGenerator::getSmoothNoise(int x, int z){
    float corners = (getNoise(x-1,z-1) + getNoise(x+1,z+1) + getNoise(x-1,z+1) + getNoise(x+1,z-1))/16.0f;
    float sides = (getNoise(x-1,z) + getNoise(x+1,z) + getNoise(x,z+1) + getNoise(x,z-1))/8.0f;
    float center = getNoise(x,z)/4.0f;
    return corners + sides + center;
}

/**
\brief Interpolates the noise using cosine interpolation.
\param x --- x coordinate.
\param z --- z coordinate.
*/
float HeightGenerator::getInterpolatedNoise(float x, float z){
  int ix = (int)x;
  int iz = (int)z;
  float fx = x - ix;
  float fz = z - iz;
  float v1 = getSmoothNoise(ix, iz);
  float v2 = getSmoothNoise(ix+1, iz);
  float v3 = getSmoothNoise(ix, iz+1);
  float v4 = getSmoothNoise(ix+1, iz+1);
  float i1 = interpolate(v1, v2, fx);
  float i2 = interpolate(v3,v4,fx);
  return interpolate(i1,i2,fz);
}

/**
\brief Applies cosine interpolation to two values.
*/
float HeightGenerator::interpolate(float a, float b, float blend){
  double theta = blend * PI;
  float f = (1.0f-cos(theta))*0.5f;
  return a * (1.0f - f) + b * f;
}

/**
\brief Returns a random number between 1 and -1
\param x --- x coordinate.
\param z --- z coordinate.
*/
float HeightGenerator::getNoise(int x, int z){
  random_generator.seed(seed + z * 11503 + x * 87337);
  uniform_real_distribution<float> dis(-1, 1);
  float n = dis(random_generator);
  return n;
}

/**
\brief Sets the amplitude of the height generator.
\param amp --- Amplitude to use.
*/
void HeightGenerator::setAmplitude(float amp){
  amplitude = amp;
}

/**
\brief Returns the current amplitude of the height generator.
*/
float HeightGenerator::getAmplitude(){
  return amplitude;
}
/**
\brief TODO
*/
unsigned int HeightGenerator::getSeed(){
  return seed;
}
/**
\brief TODO
*/
float HeightGenerator::getOctaves(){
  return octaves;
}
/**
\brief TODO
*/
float HeightGenerator::getRoughness(){
  return roughness;
}
/**
\brief TODO
*/
void HeightGenerator::setOctaves(float o){
  octaves = o;
}
/**
\brief TODO
*/
void HeightGenerator::setRoughness(float r){
  roughness = r;
}
/**
\brief TODO
*/
void HeightGenerator::setSeed(unsigned int s){
  seed = s;
}
