#pragma once

#include "scene/main/node.h"

class MonochromeConverter : public Node {
	GDCLASS(MonochromeConverter, Node);

protected:
	static void _bind_methods();

public:
	MonochromeConverter();
};
