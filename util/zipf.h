#ifndef YCSB_C_ZIPFIAN_GENERATOR_H_
#define YCSB_C_ZIPFIAN_GENERATOR_H_

extern long items; //initialized in init_zipf_generator function
extern long base; //initialized in init_zipf_generator function
extern double zipfianconstant; //initialized in init_zipf_generator function
extern double alpha; //initialized in init_zipf_generator function
extern double zetan; //initialized in init_zipf_generator function
extern double eta; //initialized in init_zipf_generator function
extern double theta; //initialized in init_zipf_generator function
extern double zeta2theta; //initialized in init_zipf_generator function
extern long countforzeta; //initialized in init_zipf_generator function
extern long lastVal; //initialized in setLastValue
void init_zipf_generator(long min, long max, double zipfianconstant);
double zeta(long st, long n, double initialsum);
double zetastatic(long st, long n, double initialsum);
long nextLong(long itemcount);
long nextValue();
void setLastValue(long val);


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <exception>
#include <random>
#include <locale>


const uint64_t kFNVOffsetBasis64 = 0xCBF29CE484222325ull;
const uint64_t kFNVPrime64 = 1099511628211ull;

inline uint64_t FNVHash64(uint64_t val) {
  uint64_t hash = kFNVOffsetBasis64;

  for (int i = 0; i < 8; i++) {
    uint64_t octet = val & 0x00ff;
    val = val >> 8;

    hash = hash ^ octet;
    hash = hash * kFNVPrime64;
  }
  return hash;
}

inline double ThreadLocalRandomDouble(double min = 0.0, double max = 1.0) {
  static thread_local std::random_device rd;
  static thread_local std::minstd_rand rn(rd());
  static thread_local std::uniform_real_distribution<double> uniform(min, max);
  return uniform(rn);
}

class ZipfianGenerator {
 public:
  static constexpr double kZipfianConst = 0.99;
  static constexpr uint64_t kMaxNumItems = (UINT64_MAX >> 24);

  ZipfianGenerator(uint64_t num_items) :
      ZipfianGenerator(0, num_items - 1) {}

  ZipfianGenerator(uint64_t min, uint64_t max, double zipfian_const = kZipfianConst) :
      ZipfianGenerator(min, max, zipfian_const, Zeta(max - min + 1, zipfian_const)) {}

  ZipfianGenerator(uint64_t min, uint64_t max, double zipfian_const, double zeta_n) :
      items_(max - min + 1), base_(min), theta_(zipfian_const), allow_count_decrease_(false) {
    assert(items_ >= 2 && items_ < kMaxNumItems);

    zeta_2_ = Zeta(2, theta_);

    alpha_ = 1.0 / (1.0 - theta_);
    zeta_n_ = zeta_n;
    count_for_zeta_ = items_;
    eta_ = Eta();

    Next();
  }

  uint64_t Next(uint64_t num_items);

  uint64_t Next() { return Next(items_); }

  uint64_t Last();
  uint64_t ScrambledNext() { return Scramble(Next()); }
 
 private:

  uint64_t Scramble(uint64_t value) const;
  double Eta() {
    return (1 - std::pow(2.0 / items_, 1 - theta_)) / (1 - zeta_2_ / zeta_n_);
  }

  ///
  /// Calculate the zeta constant needed for a distribution.
  /// Do this incrementally from the last_num of items to the cur_num.
  /// Use the zipfian constant as theta. Remember the new number of items
  /// so that, if it is changed, we can recompute zeta.
  ///
  static double Zeta(uint64_t last_num, uint64_t cur_num, double theta, double last_zeta) {
    double zeta = last_zeta;
    for (uint64_t i = last_num + 1; i <= cur_num; ++i) {
      zeta += 1 / std::pow(i, theta);
    }
    return zeta;
  }

  static double Zeta(uint64_t num, double theta) {
    return Zeta(0, num, theta, 0);
  }

  uint64_t items_;
  uint64_t base_; /// Min number of items to generate

  // Computed parameters for generating the distribution
  double theta_, zeta_n_, eta_, alpha_, zeta_2_;
  uint64_t count_for_zeta_; /// Number of items used to compute zeta_n
  uint64_t last_value_;
  bool allow_count_decrease_;
};

inline uint64_t ZipfianGenerator::Next(uint64_t num) {
  assert(num >= 2 && num < kMaxNumItems);
  if (num != count_for_zeta_) {
    // recompute zeta and eta
    if (num > count_for_zeta_) {
      zeta_n_ = Zeta(count_for_zeta_, num, theta_, zeta_n_);
      count_for_zeta_ = num;
      eta_ = Eta();
    } else if (num < count_for_zeta_ && allow_count_decrease_) {
      // TODO
    }
  }

  double u = ThreadLocalRandomDouble();
  double uz = u * zeta_n_;

  if (uz < 1.0) {
    return last_value_ = base_;
  }

  if (uz < 1.0 + std::pow(0.5, theta_)) {
    return last_value_ = base_ + 1;
  }

  return last_value_ = base_ + num * std::pow(eta_ * u - eta_ + 1, alpha_);
}

inline uint64_t ZipfianGenerator::Last() {
  return last_value_;
}

inline uint64_t ZipfianGenerator::Scramble(uint64_t value) const {
  return base_ + FNVHash64(value) % items_;
}

#endif // YCSB_C_ZIPFIAN_GENERATOR_H_