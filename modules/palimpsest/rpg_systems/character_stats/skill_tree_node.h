#pragma once

#include "scene/main/node.h"

class SkillTreeNode : public Node {
	GDCLASS(SkillTreeNode, Node);

protected:
	static void _bind_methods();

public:
	SkillTreeNode();
};
