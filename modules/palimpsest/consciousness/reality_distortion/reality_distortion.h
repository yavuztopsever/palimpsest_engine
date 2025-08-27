#pragma once

#include "scene/main/node.h"

class RealityDistortion : public Node {
	GDCLASS(RealityDistortion, Node);

protected:
	static void _bind_methods();

public:
	RealityDistortion();
};
