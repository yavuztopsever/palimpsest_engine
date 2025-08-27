#pragma once

#include "scene/main/node.h"

class TileConstraint : public Node {
	GDCLASS(TileConstraint, Node);

protected:
	static void _bind_methods();

public:
	TileConstraint();
};
