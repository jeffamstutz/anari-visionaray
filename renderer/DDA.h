#pragma once

#include "scene/volume/spatial_field/GridAccel.h"
#include "DeviceCopyableObjects.h"

namespace visionaray {

  typedef vec3i GridIterationState;
  
  template <typename Func>
  VSNRAY_FUNC
  inline void dda3(const Ray   &ray,
                   const vec3i &gridDims,
                   const box3f &modelBounds,
                   const Func  &func)
  {
    const vec3 rcp_dir(ray.dir.x != 0.f ? 1.f / ray.dir.x : 0.f,
        ray.dir.y != 0.f ? 1.f / ray.dir.y : 0.f,
        ray.dir.z != 0.f ? 1.f / ray.dir.z : 0.f);

    const vec3f lo = (modelBounds.min - ray.ori) * rcp_dir;
    const vec3f hi = (modelBounds.max - ray.ori) * rcp_dir;

    const vec3f tnear = min(lo,hi);
    const vec3f tfar  = max(lo,hi);

    vec3i cellID = projectOnGrid(ray.ori,gridDims,modelBounds);

    // Distance in world space to get from cell to cell
    const vec3f dist((tfar-tnear)/vec3f(gridDims));

    // Cell increment
    const vec3i step = {
      ray.dir.x > 0.f ? 1 : -1,
      ray.dir.y > 0.f ? 1 : -1,
      ray.dir.z > 0.f ? 1 : -1
    };

    // Stop when we reach grid borders
    const vec3i stop = {
      ray.dir.x > 0.f ? gridDims.x : -1,
      ray.dir.y > 0.f ? gridDims.y : -1,
      ray.dir.z > 0.f ? gridDims.z : -1
    };

    // Increment in world space
    vec3f tnext = {
      ray.dir.x > 0.f ? tnear.x + float(cellID.x+1) * dist.x
                      : tnear.x + float(gridDims.x-cellID.x) * dist.x,
      ray.dir.y > 0.f ? tnear.y + float(cellID.y+1) * dist.y
                      : tnear.y + float(gridDims.y-cellID.y) * dist.y,
      ray.dir.z > 0.f ? tnear.z + float(cellID.z+1) * dist.z
                      : tnear.z + float(gridDims.z-cellID.z) * dist.z
    };


    float t0 = max(ray.tmin,0.f);

    while (1) { // loop over grid cells
      const float t1 = min(min_element(tnext),ray.tmax);
      if (!func(linearIndex(cellID,gridDims),t0,t1))
        return;

#if 0
      int axis = arg_min(tnext);
      tnext[axis] += dist[axis];
      cellID[axis] += step[axis];
      if (cellID[axis]==stop[axis]) {
        break;
      }
#else
      const float t_closest = min_element(tnext);
      if (tnext.x == t_closest) {
        tnext.x += dist.x;
        cellID.x += step.x;
        if (cellID.x==stop.x) {
          break;
        }
      }
      if (tnext.y == t_closest) {
        tnext.y += dist.y;
        cellID.y += step.y;
        if (cellID.y==stop.y) {
          break;
        }
      }
      if (tnext.z == t_closest) {
        tnext.z += dist.z;
        cellID.z += step.z;
        if (cellID.z==stop.z) {
          break;
        }
      }
#endif
      t0 = t1;
    }
  }
} // namespace visionaray