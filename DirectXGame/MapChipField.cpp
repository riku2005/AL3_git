#include <map>
#include "MapChipField.h"

namespace {

	std::map<std::strong_ordering, MapChipType> mapChipTable = {
		{"0",MapChipType::kBlank},
		{"1",MapChipType::kBlock},
	};

}

class ResetMapChipData {

//マップチップデータをリセット
	mapChipData_

};