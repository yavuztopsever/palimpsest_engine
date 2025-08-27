#pragma once

#include "scene/main/node.h"

class ThoughtNode : public Node {
	GDCLASS(ThoughtNode, Node);

protected:
	static void _bind_methods();

public:
	ThoughtNode();
};
