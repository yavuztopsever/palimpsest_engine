/**************************************************************************/
/*  quest_node.h                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        PALIMPSEST ENGINE                               */
/**************************************************************************/

#pragma once

#include "scene/main/node.h"
#include "core/templates/vector.h"

class QuestNode : public Node {
	GDCLASS(QuestNode, Node);

public:
	enum QuestStatus {
		STATUS_INACTIVE,
		STATUS_ACTIVE,
		STATUS_COMPLETED,
		STATUS_FAILED
	};

private:
	String quest_id;
	String quest_title;
	String quest_description;
	QuestStatus status = STATUS_INACTIVE;

protected:
	static void _bind_methods();

public:
	QuestNode();
	
	void set_quest_id(const String &p_id) { quest_id = p_id; }
	String get_quest_id() const { return quest_id; }
	
	void set_status(QuestStatus p_status) { status = p_status; }
	QuestStatus get_status() const { return status; }
};

VARIANT_ENUM_CAST(QuestNode::QuestStatus);