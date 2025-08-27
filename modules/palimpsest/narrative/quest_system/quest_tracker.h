#pragma once

#include "scene/main/node.h"

class QuestTracker : public Node {
	GDCLASS(QuestTracker, Node);

protected:
	static void _bind_methods();

public:
	QuestTracker();
};
