#pragma once

#include "scene/main/node.h"

class PalimpsestEffect : public Node {
	GDCLASS(PalimpsestEffect, Node);

protected:
	static void _bind_methods();

public:
	PalimpsestEffect();
};
