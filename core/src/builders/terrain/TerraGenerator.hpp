#ifndef BUILDERS_TERRAIN_TERRAGENERATOR_HPP_DEFINED
#define BUILDERS_TERRAIN_TERRAGENERATOR_HPP_DEFINED

#include "builders/BuilderContext.hpp"
#include "builders/terrain/RegionTypes.hpp"
#include "builders/terrain/LineGridSplitter.hpp"

#include <functional>
#include <memory>

namespace utymap {
namespace builders {

/// Defines behaviour for generating objects on/under/above terrain.
class TerraGenerator {
 public:
  TerraGenerator(const utymap::builders::BuilderContext &context,
                 const utymap::mapcss::Style &style,
                 const utymap::math::IntPath &tileRect,
                 utymap::math::Mesh &&mesh);

  /// Called when new region is added to layer collection.
  virtual void onNewRegion(const std::string &type,
                           const utymap::entities::Element &element,
                           const utymap::mapcss::Style &style,
                           const std::shared_ptr<Region> &region) = 0;

  /// Generates mesh for given rect.
  virtual void generateFrom(const std::vector<Layer> &layers) = 0;

  virtual ~TerraGenerator() = default;

 protected:
  /// Adds geometry to mesh.
  void addGeometry(int level,
                   utymap::math::IntPaths &paths,
                   const RegionContext &regionContext,
                   const std::function<void(const utymap::math::IntPath &)> &geometryVisitor);

  /// Adds geometry to mesh.
  virtual void addGeometry(int level,
                           utymap::math::Polygon &polygon,
                           const RegionContext &regionContext) = 0;

  const utymap::builders::BuilderContext &context_;
  const utymap::mapcss::Style &style_;
  const utymap::math::IntPath &tileRect_;
  utymap::math::Mesh mesh_;

 private:

  /// Checks whether given point is on tile border.
  inline bool isOnBorder(const utymap::math::Vector2 &p) const { return rect_.isOnBorder(p); }

  /// Builds height contour shape.
  void buildHeightOffset(const std::vector<utymap::math::Vector2> &points, const RegionContext &regionContext);

  /// Restores geometry from clipper format.
  std::vector<utymap::math::Vector2> restoreGeometry(const utymap::math::IntPath &geometry) const;

  const utymap::math::Rectangle rect_;
  utymap::builders::LineGridSplitter splitter_;
};

}
}

#endif // BUILDERS_TERRAIN_TERRAGENERATOR_HPP_DEFINED
