// @copyright 2022-2024, Antoine Basset (CNES)
// This file is part of Linx <github.com/kabasset/Linx>
// SPDX-License-Identifier: Apache-2.0

#include "GrowiLa/Pipeline.h"
#include "Linx/Io/Fits.h"
#include "Linx/Run/ProgramOptions.h"

#include <string>

using namespace GrowiLa;

int main(int argc, char const* argv[])
{
  Linx::ProgramOptions options;
  options.positional<std::string>("input", "The input data file name");
  options.positional<std::string>("output", "The output mask file name");
  options.named("hdu,i", "The 0-based input HDU index slice", 0L);
  options.named("pfa,p", "The detection probability of false alarm", 0.01);
  options.flag("dilate", "Enable dilation");
  options.parse(argc, argv);
  Linx::Fits data_fits(options.as<std::string>("input"));
  Linx::Fits map_fits(options.as<std::string>("output"));
  const auto hdu = options.as<Linx::Index>("hdu");
  const auto pfa = options.as<double>("pfa");
  const auto radius = options.has("dilate");

  std::cout << "Reading data: " << data_fits.path() << std::endl;
  auto data = data_fits.read<Linx::Raster<float>>(hdu);

  std::cout << "Detecting cosmics..." << std::endl;
  Pipeline pipeline(data, pfa, radius);
  const auto& map = pipeline.get<Growing>();
  std::cout << "  Done in: " << pipeline.milliseconds() << std::endl;
  std::cout << "  - Enhancement: " << pipeline.milliseconds<Enhancement>() << std::endl;
  std::cout << "  - Detection: " << pipeline.milliseconds<Detection>() << std::endl;
  std::cout << "  - Growing: " << pipeline.milliseconds<Growing>() << std::endl;

  map_fits.write(map, 'x');
  std::cout << "Saved map as: " << map_fits.path() << std::endl;

  return 0;
}