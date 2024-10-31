// Copyright 2022 The Khronos Group
// SPDX-License-Identifier: Apache-2.0

#pragma once

// std
#include <array>
// ours
#include "array/Array1D.h"
#include "DeviceCopyableObjects.h"
#include "Object.h"

namespace visionaray {

struct Geometry : public Object
{
  Geometry(VisionarayGlobalState *s);
  ~Geometry() override;

  static Geometry *createInstance(
      std::string_view subtype, VisionarayGlobalState *s);

  dco::Geometry visionarayGeometry() const;

  void commit() override;
  void markCommitted() override;

 protected:

  dco::Geometry vgeom;

  std::array<helium::IntrusivePtr<Array1D>, 5> m_attributes;

  HostDeviceArray<uint8_t> vattributes[5];

  void dispatch();
};

} // namespace visionaray

VISIONARAY_ANARI_TYPEFOR_SPECIALIZATION(visionaray::Geometry *, ANARI_GEOMETRY);
