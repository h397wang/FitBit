#include "layout.h"
#include <stdio.h>
//! Initialize node with the given values
//! @param node the node to initialize
//! @param name A name for the node. You do not have to deep copy this value
//! @param id An id for the node
//! @param position A relative position for this node with respect to it's parent
void layout_node_init(struct LayoutNode* node, const char* name, int id, struct Position position) {
	node->childrenHeadPtr = NULL;
	node->childrenEndPtr = NULL;
    node->nextSiblingPtr = NULL;
	node->name = name;
	node->id = id;
	node->relativePosition = position;
	node->absolutePosition = position; // will be adjusted later
}

//! Add an initialized node as the root of the tree
void layout_init(struct Layout* layout, struct LayoutNode* root) {
	// Assume that the node will only be added to the tree once and only after being initialized
	layout->rootPtr = root;
	// For the root, the absolute position is equal to relative position, which is already initialized as required
}

//! Add an intialized node as the child of another node already in the Layout tree
void layout_add_child(struct Layout* layout, struct LayoutNode* parent, struct LayoutNode* child) {
	struct Position childAbsPosition;

	childAbsPosition.x = parent->absolutePosition.x + child->relativePosition.x;
	childAbsPosition.y = parent->absolutePosition.y + child->relativePosition.y;

	child->absolutePosition = childAbsPosition;

	if (parent->childrenHeadPtr == NULL) { // parent has no children
		parent->childrenHeadPtr = child;
		parent->childrenEndPtr = child; // REDUNDANT
	} else { // parent has at least one child
		parent->childrenEndPtr->nextSiblingPtr = child;
	}

}

//! Change the position of a node. This should work whether or not the node is already in a tree
void layout_node_update_position(struct Layout* layout, struct LayoutNode* node, struct Position position) {
	struct Position deltaPosition;
	struct Position newAbsPosition;
	struct Position oldAbsPosition = node->absolutePosition;

	// Without a reference to the parent, calculate its parents absolute position
	newAbsPosition.x = node->absolutePosition.x - node->relativePosition.x + position.x;
	newAbsPosition.y = node->absolutePosition.y - node->relativePosition.y + position.y;

	node->absolutePosition = newAbsPosition;
	node->relativePosition = position;
	
	deltaPosition.x = newAbsPosition.x - oldAbsPosition.x;
	deltaPosition.y = newAbsPosition.y - oldAbsPosition.y;	
	// Update the positions of all its descendants
	updateChildrenAbsPositions(node, deltaPosition);
}

//! @return the absolute position for the node with the given memory address
struct Position layout_get_position_for_node(struct Layout* layout, struct LayoutNode* node) {
	return node->absolutePosition; 
}

//! @return the absolute position for the node with the given name
//! Assume that the node exists in the tree and is initialized properly
struct Position layout_get_position_for_name(struct Layout* layout, const char* name) {
	struct Position ret;
	// Check if we're referring to the root node
	if (layout->rootPtr->name == name) { // not deep copied so this works
		return layout->rootPtr->absolutePosition;
	} 

	// Otherwise search with BFS
	DFSByName(layout->rootPtr, name, &ret);
	return ret;
}

//! @return the absolute position for the node with the given id
struct Position layout_get_position_for_id(struct Layout* layout, int id) {
	struct Position ret;
	// Check if we're referring to the root node
	if (layout->rootPtr->id == id) { // not deep copied so this works
		return layout->rootPtr->absolutePosition;
	} 
	// Otherwise search with BFS
	DFSById(layout->rootPtr, id, &ret);
	return ret;
}

void DFSByName(struct LayoutNode* node, const char* name, struct Position* posPtr) {
	struct LayoutNode* currentChildPtr = node->childrenHeadPtr;
	while (currentChildPtr != NULL) {
		if (currentChildPtr->name == name) {
			*posPtr = currentChildPtr->absolutePosition;
			return;
		} else {
			DFSByName(currentChildPtr, name, posPtr);
		}
		currentChildPtr = currentChildPtr->nextSiblingPtr;
	}
}

void DFSById(struct LayoutNode* node, const int id, struct Position* posPtr) {
	struct LayoutNode* currentChildPtr = node->childrenHeadPtr;
	while (currentChildPtr != NULL) {
		if (currentChildPtr->id == id) {
			*posPtr = currentChildPtr->absolutePosition;
			return;
		} else {
			DFSById(currentChildPtr, id, posPtr);
		}
		currentChildPtr = currentChildPtr->nextSiblingPtr;
	}
}

void updateChildrenAbsPositions(struct LayoutNode* node, struct Position deltaPosition) {
	struct LayoutNode* currentChildPtr = node->childrenHeadPtr;
	while (currentChildPtr != NULL) {
		currentChildPtr->absolutePosition.x += deltaPosition.x;
		currentChildPtr->absolutePosition.y += deltaPosition.y;
		updateChildrenAbsPositions(currentChildPtr, deltaPosition);
		currentChildPtr = currentChildPtr->nextSiblingPtr;
	}
}
