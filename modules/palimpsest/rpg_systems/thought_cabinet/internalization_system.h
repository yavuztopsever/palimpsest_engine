#pragma once

#include "scene/main/node.h"

class InternalizationSystem : public Node {
	GDCLASS(InternalizationSystem, Node);

protected:
	static void _bind_methods();

public:
	InternalizationSystem();
};
