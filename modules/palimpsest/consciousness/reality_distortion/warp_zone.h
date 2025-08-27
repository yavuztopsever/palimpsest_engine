#pragma once

#include "scene/main/node.h"

class WarpZone : public Node {
	GDCLASS(WarpZone, Node);

protected:
	static void _bind_methods();

public:
	WarpZone();
};
