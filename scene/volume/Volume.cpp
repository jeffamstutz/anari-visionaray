// Copyright 2022 The Khronos Group
// SPDX-License-Identifier: Apache-2.0

// visionaray
#ifdef WITH_CUDA
#include <visionaray/texture/cuda_texture.h> // include to prevent name collisions
#endif
// ours
#include "Volume.h"
// subtypes
#include "TransferFunction1D.h"

namespace visionaray {

Volume::Volume(VisionarayGlobalState *s) : Object(ANARI_VOLUME, s)
{
  vvol = dco::createVolume();
  vvol.volID = deviceState()->dcos.volumes.alloc(vvol);
}

Volume::~Volume()
{
  deviceState()->dcos.volumes.free(vvol.volID);
}

void Volume::commitParameters()
{
   m_id = getParam<uint32_t>("id", ~0u);
}

uint32_t Volume::id() const
{
  return m_id;
}

Volume *Volume::createInstance(std::string_view subtype, VisionarayGlobalState *s)
{
  if (subtype == "transferFunction1D")
    return new TransferFunction1D(s);
  else
    return (Volume *)new UnknownObject(ANARI_VOLUME, s);
}

dco::Volume Volume::visionarayVolume() const
{
  return vvol;
}

} // namespace visionaray

VISIONARAY_ANARI_TYPEFOR_DEFINITION(visionaray::Volume *);
