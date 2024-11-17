// Copyright 2022 The Khronos Group
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "VisionarayGlobalState.h"
#include "common.h"
// helium
#include "helium/BaseObject.h"
#include "helium/utility/ChangeObserverPtr.h"
// visionaray
#include "visionaray/math/math.h"
// std
#include <string_view>

namespace visionaray {

struct Object : public helium::BaseObject
{
  Object(ANARIDataType type, VisionarayGlobalState *s);
  virtual ~Object() = default;

  virtual bool getProperty(const std::string_view &name,
      ANARIDataType type,
      void *ptr,
      uint32_t flags);

  virtual void commit();

  virtual bool isValid() const;

  VisionarayGlobalState *deviceState() const;
};

struct UnknownObject : public Object
{
  UnknownObject(ANARIDataType type, VisionarayGlobalState *s);
  ~UnknownObject() override = default;
  bool isValid() const override;
};

} // namespace visionaray

VISIONARAY_ANARI_TYPEFOR_SPECIALIZATION(visionaray::Object *, ANARI_OBJECT);
