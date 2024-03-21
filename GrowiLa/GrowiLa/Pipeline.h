// @copyright 2022-2024, Antoine Basset (CNES)
// This file is part of Linx <github.com/kabasset/Linx>
// SPDX-License-Identifier: Apache-2.0

#ifndef _GROWILA_PIPELINE_H
#define _GROWILA_PIPELINE_H

#include "GrowiLa/Filters.h"
#include "Linx/Run/StepperPipeline.h"

namespace GrowiLa {

/**
 * @brief The pixel type.
 */
using Pixel = float;

/**
 * @brief The flag type.
 */
using Flag = char;

/**
 * @brief The detection pipeline.
 */
class Pipeline : public Linx::StepperPipeline<Pipeline> {
public:

  Pipeline(const Linx::Raster<Pixel>& in, Pixel pfa, Linx::Index radius) :
      m_in(in), m_enhanced(in.shape()), m_detected(in.shape()), m_grown(in.shape()), m_pfa(pfa), m_threshold(0),
      m_radius(radius)
  {}

  /**
   * @brief Evaluate a given step.
   */
  template <typename S>
  void evaluate_impl();

  /**
   * @brief Get the result of a given step.
   */
  template <typename S>
  typename S::Return get_impl();

private:

  Linx::Raster<Pixel> m_in;
  Linx::Raster<Pixel> m_enhanced;
  Linx::Raster<Flag> m_detected;
  Linx::Raster<Flag> m_grown;
  Pixel m_pfa;
  Pixel m_threshold;
  Linx::Index m_radius;
};

/**
 * @brief The enhancement step.
 */
struct Enhancement : Linx::PipelineStep<void, const Linx::Raster<Pixel>&> {};

template <>
void Pipeline::evaluate_impl<Enhancement>()
{
  laplacian(m_in, m_enhanced);
}

template <>
Enhancement::Return Pipeline::get_impl<Enhancement>()
{
  return m_enhanced;
}

/**
 * @brief The detection step.
 */
struct Detection : Linx::PipelineStep<Enhancement, const Linx::Raster<Flag>&> {};

template <>
void Pipeline::evaluate_impl<Detection>()
{
  m_threshold = -Linx::norm<1>(m_enhanced) / m_enhanced.size() * std::log(2.0 * m_pfa);
  m_detected.generate(
      [=](auto e) {
        return e > m_threshold;
      },
      m_enhanced);
}

template <>
Detection::Return Pipeline::get_impl<Detection>()
{
  return m_detected;
}

/**
 * @brief The growing step.
 */
struct Growing : Linx::PipelineStep<Detection, const Linx::Raster<Flag>&> {};

template <>
void Pipeline::evaluate_impl<Growing>()
{
  if (m_radius > 0) {
    dilate(m_detected, m_radius, m_grown);
  }
}

template <>
Growing::Return Pipeline::get_impl<Growing>()
{
  return m_radius > 0 ? m_grown : m_detected;
}

} // namespace GrowiLa

#endif
