#pragma once

#include "game/state/agent.h"
#include "game/state/battle/battle.h"
#include "game/state/battle/battlecommonimagelist.h"
#include "game/state/battle/battlecommonsamplelist.h"
#include "game/state/battle/battlemap.h"
#include "game/state/battle/battlemappart_type.h"
#include "game/state/battle/battlemapsector.h"
#include "game/state/battle/battlemaptileset.h"
#include "game/state/battle/battleunitanimationpack.h"
#include "game/state/battle/battleunitimagepack.h"
#include "game/state/city/baselayout.h"
#include "game/state/city/building.h"
#include "game/state/city/city.h"
#include "game/state/gamestate.h"
#include "game/state/organisation.h"
#include "game/state/rules/aequipment_type.h"
#include "game/state/rules/damage.h"
#include "game/state/rules/doodad_type.h"
#include "game/state/rules/facility_type.h"
#include "game/state/rules/scenery_tile_type.h"
#include "game/state/rules/vammo_type.h"
#include "game/state/rules/vehicle_type.h"
#include "game/state/rules/vequipment_type.h"
#include "game/state/tileview/tile.h"
#include "library/sp.h"
#include "library/strings.h"
#include "library/strings_format.h"
#include "library/vec.h"
#include "library/voxel.h"
#include "library/xorshift.h"
#include <SDL_main.h>
#include <algorithm>
#include <array>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>