#pragma once

#include "scene/main/node.h"

class SkillCheckNode : public Node {
	GDCLASS(SkillCheckNode, Node);

protected:
	static void _bind_methods();

public:
	SkillCheckNode();
};
