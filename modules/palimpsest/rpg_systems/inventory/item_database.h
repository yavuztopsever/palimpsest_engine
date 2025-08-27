#pragma once

#include "scene/main/node.h"

class ItemDatabase : public Node {
	GDCLASS(ItemDatabase, Node);

protected:
	static void _bind_methods();

public:
	ItemDatabase();
};
