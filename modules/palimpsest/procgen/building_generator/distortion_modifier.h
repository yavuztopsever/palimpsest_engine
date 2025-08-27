#pragma once

#include "scene/main/node.h"

class DistortionModifier : public Node {
	GDCLASS(DistortionModifier, Node);

protected:
	static void _bind_methods();

public:
	DistortionModifier();
};
