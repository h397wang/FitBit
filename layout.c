#include "layout.h"
#include <stdio.h>

#define FOUND 1
#define NOT_FOUND 0

//! Forward declarations for my helper functions 
int DFSByName(struct LayoutNode* node, const char* name, struct Position* posPtr);
int DFSById(struct LayoutNode* node, const int id, struct Position* posPtr);
void updateChildrenAbsPositions(struct LayoutNode* node, struct Position deltaPosition);

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
	node->absolutePosition = position; // to be reassigned as required
}

//! Add an initialized node as the root of the tree
//! Assume that the node will only be added to the tree once and only after being initialized
void layout_init(struct Layout* layout, struct LayoutNode* root) {
	layout->rootPtr = root; // initialized such that absolute position = relative position
}

//! Add an intialized node as the child of another node already in the Layout tree
//! Adding a child takes O(1) time
void layout_add_child(struct Layout* layout, struct LayoutNode* parent, struct LayoutNode* child) {
	child->absolutePosition.x = parent->absolutePosition.x + child->relativePosition.x;
	child->absolutePosition.y = parent->absolutePosition.y + child->relativePosition.y;
	
	if (parent->childrenHeadPtr == NULL) { // parent has no children
		parent->childrenHeadPtr = child;
		parent->childrenEndPtr = child;
	} else { // parent has at least one child
		parent->childrenEndPtr->nextSiblingPtr = child;
		parent->childrenEndPtr = child;
	}
}

//! Change the position of a node. This should work whether or not the node is already in a tree
//! Updating the position of some node in the tree takes O(n) time because all child nodes must be updated
//! Implementation accounts for the case if the node is not in the layout
void layout_node_update_position(struct Layout* layout, struct LayoutNode* node, struct Position position) {
	struct Position deltaPosition;
	struct Position newAbsPosition;

	deltaPosition.x = position.x - node->relativePosition.x;
	deltaPosition.y = position.y - node->relativePosition.y;

	newAbsPosition.x = node->absolutePosition.x + deltaPosition.x;
	newAbsPosition.y = node->absolutePosition.y + deltaPosition.y;

	node->absolutePosition = newAbsPosition;
	node->relativePosition = position;

	updateChildrenAbsPositions(node, deltaPosition);
}

//! @return the absolute position for the node with the given memory address
//! O(1) runtime as the absolute position is already calculated and stored when added
struct Position layout_get_position_for_node(struct Layout* layout, struct LayoutNode* node) {
	return node->absolutePosition; 
}

//! @return the absolute position for the node with the given name
//! Assume that the node exists in the tree and is initialized properly
struct Position layout_get_position_for_name(struct Layout* layout, const char* name) {
	struct Position ret;

	if (layout->rootPtr->name == name) { // not deep copied so this works
		return layout->rootPtr->absolutePosition;
	}
	DFSByName(layout->rootPtr, name, &ret);
	return ret;
}

//! @return the absolute position for the node with the given id
struct Position layout_get_position_for_id(struct Layout* layout, int id) {
	struct Position ret;

	if (layout->rootPtr->id == id) {
		return layout->rootPtr->absolutePosition;
	} 
	DFSById(layout->rootPtr, id, &ret);
	return ret;
}

//! Implementations of my helper functions
int DFSByName(struct LayoutNode* node, const char* name, struct Position* posPtr) {
	struct LayoutNode* currentChildPtr;

	currentChildPtr = node->childrenHeadPtr;
	while (currentChildPtr != NULL) {
		if (currentChildPtr->name == name) {
			*posPtr = currentChildPtr->absolutePosition;
			return FOUND;
		} else {
			if (DFSByName(currentChildPtr, name, posPtr)) { 
				return FOUND; // return early if found
			} // otherwise, onto the sibling
		}
		currentChildPtr = currentChildPtr->nextSiblingPtr;
	}
	return NOT_FOUND;
}

// Unforturnate instance of code repetition
int DFSById(struct LayoutNode* node, const int id, struct Position* posPtr) {
	struct LayoutNode* currentChildPtr;

	currentChildPtr = node->childrenHeadPtr;
	while (currentChildPtr != NULL) {
		if (currentChildPtr->id == id) {
			*posPtr = currentChildPtr->absolutePosition;
			return FOUND;
		} else {
			if (DFSById(currentChildPtr, id, posPtr)) {
				return FOUND;
			}
		}
		currentChildPtr = currentChildPtr->nextSiblingPtr;
	}
	return NOT_FOUND;
}

void updateChildrenAbsPositions(struct LayoutNode* node, struct Position deltaPosition) {
	struct LayoutNode* currentChildPtr;

	currentChildPtr = node->childrenHeadPtr;
	while (currentChildPtr != NULL) {
		currentChildPtr->absolutePosition.x += deltaPosition.x;
		currentChildPtr->absolutePosition.y += deltaPosition.y;
		updateChildrenAbsPositions(currentChildPtr, deltaPosition);
		currentChildPtr = currentChildPtr->nextSiblingPtr;
	}
}