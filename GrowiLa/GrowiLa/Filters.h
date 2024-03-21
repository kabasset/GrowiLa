// @copyright 2022-2024, Antoine Basset (CNES)
// This file is part of Linx <github.com/kabasset/Linx>
// SPDX-License-Identifier: Apache-2.0

#ifndef _GROWILA_FILTERS_H
#define _GROWILA_FILTERS_H

#include "Linx/Data/Raster.h"
#include "Linx/Transforms/Filters.h"

namespace GrowiLa {

/**
 * @brief Compute the isotropic Laplacian of an image.
 * @tparam T The output pixel type
 */
template <typename TIn, typename TOut>
void laplacian(const TIn& in, TOut& out)
{
  using T = typename TOut::Value;
  static const auto filter = Linx::correlation(Linx::Raster<T>(
      {3, 3},
      {-1. / 6., -2. / 3., -1. / 6., -2. / 3., 10. / 3., -2. / 3., -1. / 6., -2. / 3., -1. / 6.}));
  filter.transform(Linx::extrapolation<Linx::Nearest>(in), out); // FIXME filter.transform<Nearest>(in, out)
}

/**
 * @brief Dilate an image with a box of given radius.
 * @tparam T The output pixel type
 */
template <typename TIn, typename TOut>
void dilate(const TIn& in, Linx::Index radius, TOut& out)
{
  using T = typename TOut::Value;
  auto filter = Linx::dilation<T>(Linx::Box<2>::from_center(radius)); // FIXME L2-ball?
  filter.transform(Linx::extrapolation<Linx::Nearest>(in), out); // FIXME filter.transform<Nearest>(in, out)
}

/**
 * @brief Blur an image with a box of given radius.
 * @tparam T The output pixel type
 */
template <typename TIn, typename TOut>
void blur(const TIn& in, Linx::Index radius, TOut& out)
{
  using T = typename TOut::Value;
  auto filter = Linx::mean_filter<T>(Linx::Box<2>::from_center(radius)); // FIXME L2-ball?
  filter.transform(Linx::extrapolation<Linx::Nearest>(in), out); // FIXME filter.transform<Nearest>(in, out)
}

} // namespace GrowiLa

#endif