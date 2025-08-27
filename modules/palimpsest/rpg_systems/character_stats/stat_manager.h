#pragma once

#include "scene/main/node.h"

class StatManager : public Node {
	GDCLASS(StatManager, Node);

protected:
	static void _bind_methods();

public:
	StatManager();
};
