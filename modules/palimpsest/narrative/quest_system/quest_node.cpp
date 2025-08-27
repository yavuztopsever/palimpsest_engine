/**************************************************************************/
/*  quest_node.cpp                                                        */
/**************************************************************************/

#include "quest_node.h"
#include "core/object/class_db.h"

QuestNode::QuestNode() {
}

void QuestNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_quest_id", "id"), &QuestNode::set_quest_id);
	ClassDB::bind_method(D_METHOD("get_quest_id"), &QuestNode::get_quest_id);
	ClassDB::bind_method(D_METHOD("set_status", "status"), &QuestNode::set_status);
	ClassDB::bind_method(D_METHOD("get_status"), &QuestNode::get_status);
	
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "quest_id"), "set_quest_id", "get_quest_id");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "status"), "set_status", "get_status");
	
	BIND_ENUM_CONSTANT(STATUS_INACTIVE);
	BIND_ENUM_CONSTANT(STATUS_ACTIVE);
	BIND_ENUM_CONSTANT(STATUS_COMPLETED);
	BIND_ENUM_CONSTANT(STATUS_FAILED);
}