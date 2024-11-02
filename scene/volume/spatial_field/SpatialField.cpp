// Copyright 2022 The Khronos Group
// SPDX-License-Identifier: Apache-2.0

#include "SpatialField.h"
// subtypes
#include "BlockStructuredField.h"
#include "StructuredRegularField.h"
#include "UnstructuredField.h"

namespace visionaray {

SpatialField::SpatialField(VisionarayGlobalState *s)
    : Object(ANARI_SPATIAL_FIELD, s)
    , m_gridAccel(s)
{
  s->objectCounts.spatialFields++;
  vfield.fieldID = deviceState()->dcos.spatialFields.alloc(vfield);
  m_gridAccel.visionarayAccel().fieldID = vfield.fieldID;
  m_gridAccel.visionarayAccel().dims = int3(0);
  m_gridAccel.visionarayAccel().valueRanges = nullptr;
  m_gridAccel.visionarayAccel().maxOpacities = nullptr;
}

SpatialField::~SpatialField()
{
  deviceState()->dcos.spatialFields.free(vfield.fieldID);

  deviceState()->objectCounts.spatialFields--;
}

SpatialField *SpatialField::createInstance(
    std::string_view subtype, VisionarayGlobalState *s)
{
  if (subtype == "structuredRegular")
     return new StructuredRegularField(s);
  else if (subtype == "unstructured")
    return new UnstructuredField(s);
  else if (subtype == "amr" || subtype == "blockStructured")
     return new BlockStructuredField(s);
  else
    return (SpatialField *)new UnknownObject(ANARI_SPATIAL_FIELD, s);
}

dco::SpatialField SpatialField::visionaraySpatialField() const
{
  return vfield;
}

GridAccel &SpatialField::gridAccel()
{
  return m_gridAccel;
}

void SpatialField::buildGrid()
{
  reportMessage(ANARI_SEVERITY_WARNING,
      "buildGrid() not implemented for field type");
}

void SpatialField::setGradientDelta(float delta)
{
  vfield.delta = delta;
}

void SpatialField::dispatch()
{
  deviceState()->dcos.spatialFields.update(vfield.fieldID, vfield);

  // Upload/set accessible pointers
  deviceState()->onDevice.spatialFields = deviceState()->dcos.spatialFields.devicePtr();
}

} // namespace visionaray

VISIONARAY_ANARI_TYPEFOR_DEFINITION(visionaray::SpatialField *);
