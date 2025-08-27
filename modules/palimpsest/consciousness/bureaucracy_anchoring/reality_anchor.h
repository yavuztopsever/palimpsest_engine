#pragma once

#include "scene/main/node.h"

class RealityAnchor : public Node {
	GDCLASS(RealityAnchor, Node);

protected:
	static void _bind_methods();

public:
	RealityAnchor();
};
