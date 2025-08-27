#pragma once

#include "scene/main/node.h"

class FormProcessor : public Node {
	GDCLASS(FormProcessor, Node);

protected:
	static void _bind_methods();

public:
	FormProcessor();
};
