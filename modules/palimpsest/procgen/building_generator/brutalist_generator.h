#pragma once

#include "scene/main/node.h"

class BrutalistGenerator : public Node {
	GDCLASS(BrutalistGenerator, Node);

protected:
	static void _bind_methods();

public:
	BrutalistGenerator();
};
