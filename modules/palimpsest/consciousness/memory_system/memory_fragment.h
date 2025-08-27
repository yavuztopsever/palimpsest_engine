#pragma once

#include "scene/main/node.h"

class MemoryFragment : public Node {
	GDCLASS(MemoryFragment, Node);

protected:
	static void _bind_methods();

public:
	MemoryFragment();
};
