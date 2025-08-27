#pragma once

#include "scene/main/node.h"

class InventoryContainer : public Node {
	GDCLASS(InventoryContainer, Node);

protected:
	static void _bind_methods();

public:
	InventoryContainer();
};
