#ifndef _EnumText_h_
#define _EnumText_h_
/// \file Methods to turn values defined in Enums.h into user readable strings.

#include <string>

#include "../util/Export.h"
#include "../universe/Enums.h"

/// Returns a user readable string for a GalaxySetupOption
FO_COMMON_API const std::string& TextForGalaxySetupSetting(GalaxySetupOption gso);

/// Returns a user readable string for a Shape
FO_COMMON_API const std::string& TextForGalaxyShape(Shape shape);

/// Returns a user readable string for an Aggression
FO_COMMON_API const std::string& TextForAIAggression(Aggression a);

#endif // _EnumText_h_