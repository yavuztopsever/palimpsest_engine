#pragma once

#include "scene/main/node.h"

class ConsciousnessArchitecture : public Node {
	GDCLASS(ConsciousnessArchitecture, Node);

protected:
	static void _bind_methods();

public:
	ConsciousnessArchitecture();
};
