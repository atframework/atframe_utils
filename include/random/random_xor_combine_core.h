﻿/**
 * @file random_xor_combine_core.h
 * @brief 伪随机数生成器  - 线性回归核心
 *
 *
 * Licensed under the MIT licenses.
 * @version 1.0
 * @author OWenT
 * @date 2013年8月6日
 *
 * @history
 *
 */

#ifndef UTIL_RANDOM_RANDOMXORCOMBINECORE_H
#define UTIL_RANDOM_RANDOMXORCOMBINECORE_H

#pragma once

#include <cstring>
#include <memory>

#include <config/atframe_utils_build_feature.h>

namespace util {
namespace random {
namespace core {
template <class UIntType, int w, int k, int q, int s>
class LIBATFRAME_UTILS_API_HEAD_ONLY linear_feedback_shift_engine {
 public:
  typedef UIntType result_type;

 private:
  /// \endcond
  result_type xor_c_seed_;

  /// \cond show_private
  static UTIL_CONFIG_CONSTEXPR result_type wordmask() UTIL_CONFIG_NOEXCEPT {
    return (~((~(static_cast<uint64_t>(0))) << (sizeof(result_type) * 8)));
  }

 public:
  linear_feedback_shift_engine() {}
  linear_feedback_shift_engine(result_type xor_c_seed) { init_seed(xor_c_seed); }

  void init_seed(result_type xor_c_seed) UTIL_CONFIG_NOEXCEPT {
    xor_c_seed_ = xor_c_seed & wordmask();
    if (xor_c_seed_ < (1 << (w - k))) {
      xor_c_seed_ += 1 << (w - k);
    }
  }

  inline size_t block_size() const UTIL_CONFIG_NOEXCEPT { return sizeof(xor_c_seed_); }

  inline bool dump(unsigned char *output, size_t size) const UTIL_CONFIG_NOEXCEPT {
    if (NULL == output || size < block_size()) {
      return false;
    }

    memcpy(output, &xor_c_seed_, sizeof(xor_c_seed_));
    return true;
  }

  inline bool load(const unsigned char *input, size_t size) UTIL_CONFIG_NOEXCEPT {
    if (NULL == input || size < block_size()) {
      return false;
    }

    memcpy(&xor_c_seed_, input, sizeof(xor_c_seed_));
    return true;
  }

  result_type random() UTIL_CONFIG_NOEXCEPT {
    const result_type b = (((xor_c_seed_ << q) ^ xor_c_seed_) & wordmask()) >> (k - s);
    const result_type mask = (wordmask() << (w - k)) & wordmask();
    xor_c_seed_ = ((xor_c_seed_ & mask) << s) ^ b;
    return xor_c_seed_;
  }

  result_type operator()() UTIL_CONFIG_NOEXCEPT { return random(); }
};

template <class URNG1, int s1, class URNG2, int s2>
class LIBATFRAME_UTILS_API_HEAD_ONLY xor_combine_engine {
 public:
  typedef URNG1 base_left_type;
  typedef URNG2 base_right_type;
  typedef typename base_left_type::result_type result_type;

 private:
  base_left_type xor_c_rng_left_;
  base_right_type xor_c_rng_right_;

 public:
  xor_combine_engine() UTIL_CONFIG_NOEXCEPT {}
  xor_combine_engine(result_type xor_c_seed) UTIL_CONFIG_NOEXCEPT { init_seed(xor_c_seed); }

  void init_seed(result_type xor_c_seed) UTIL_CONFIG_NOEXCEPT {
    xor_c_rng_left_.init_seed(xor_c_seed);
    xor_c_rng_right_.init_seed(xor_c_seed);
  }

  template <class It>
  LIBATFRAME_UTILS_API_HEAD_ONLY void init_seed(It &first, It last) UTIL_CONFIG_NOEXCEPT {
    It begin = first;
    if (begin != last) {
      xor_c_rng_left_.init_seed(*begin);
      ++begin;
    }

    if (begin != last) {
      xor_c_rng_right_.init_seed(*begin);
      ++begin;
    }
  }

  inline size_t block_size() const UTIL_CONFIG_NOEXCEPT {
    return xor_c_rng_left_.block_size() + xor_c_rng_right_.block_size();
  }

  inline bool dump(unsigned char *output, size_t size) const UTIL_CONFIG_NOEXCEPT {
    if (NULL == output || size < block_size()) {
      return false;
    }

    return xor_c_rng_left_.dump(output, xor_c_rng_left_.block_size()) &&
           xor_c_rng_right_.dump(output + xor_c_rng_left_.block_size(), xor_c_rng_right_.block_size());
  }

  inline bool load(const unsigned char *input, size_t size) UTIL_CONFIG_NOEXCEPT {
    if (NULL == input || size < block_size()) {
      return false;
    }

    return xor_c_rng_left_.load(input, xor_c_rng_left_.block_size()) &&
           xor_c_rng_right_.load(input + xor_c_rng_left_.block_size(), xor_c_rng_right_.block_size());
  }

  /** Returns the first base generator. */
  const base_left_type &GetLeftBase() const UTIL_CONFIG_NOEXCEPT { return xor_c_rng_left_; }

  /** Returns the second base generator. */
  const base_right_type &GetRightBase() const UTIL_CONFIG_NOEXCEPT { return xor_c_rng_right_; }

  result_type random() UTIL_CONFIG_NOEXCEPT { return (xor_c_rng_left_() << s1) ^ (xor_c_rng_right_() << s2); }

  result_type operator()() UTIL_CONFIG_NOEXCEPT { return random(); }
};
}  // namespace core
}  // namespace random
}  // namespace util

#endif /* _UTIL_RANDOM_RANDOMXORCOMBINECORE_H_ */
