#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <math.h>
// numerical recipies p. 279


// simple random numbers by numerical recipies
// ran0 returns a value between 0.0 and 1.0
class Random {
private:
    int   idum;
public:
    Random(int seed) {idum = seed;}
    float ran0() ;
};


static const int   RAND_IA   = 16807;
static const int   RAND_IM   = 2147483647;
static const float RAND_AM   = 1.0/RAND_IM;
static const int   RAND_IQ   = 127773;
static const int   RAND_IR   = 2836;
static const int   RAND_MASK = 123459876;

float Random::ran0() {
    long int   k;
    float    ans;
    idum ^= RAND_MASK;
    k  = idum/RAND_IQ;
    idum = RAND_IA * (idum - k*RAND_IQ) - RAND_IR*k;
    if (idum < 0) idum += RAND_IM;
    ans = RAND_AM * idum;
    idum ^= RAND_MASK;
    return ans;
}

//Refere IEEE 745 standard format for 64-bit floating- point number at
//Computer architecture and organization by Hayes (ISBN 0-07-027366-9) p.196
float toFloat(int readed)
{
      int i, exponent;
      float mantissa, floatNumber;
      exponent = (readed>>20)&2047;      // 2^11 - 1 == 2047
      exponent = exponent - 1023;
      mantissa = (readed&1048575);       // 2^20 - 1 == 1048575
      mantissa =mantissa / 1048575.0;    // use 20 bit mantissa

      if(readed&(1<<31))
         floatNumber=-1.0;
      else
         floatNumber=1.0;

      if(exponent>0)
         for(i=0;i<exponent;i++)
            floatNumber=floatNumber*2.0;
      else
         for(i=0;i<-exponent;i++)
            floatNumber=floatNumber/2.0;
      floatNumber=floatNumber*(1.0+mantissa);
      return floatNumber;
}



#endif