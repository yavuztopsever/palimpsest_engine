#pragma once

#include "scene/main/node.h"

class ExpressionistMaterial : public Node {
	GDCLASS(ExpressionistMaterial, Node);

protected:
	static void _bind_methods();

public:
	ExpressionistMaterial();
};
