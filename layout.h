//! This header file describes a basic API for implementing a hierarchical layout system.
//! A series of nodes are laid out in a tree where each node has a position relative to it's
//! parent. A user should be able to add nodes to this tree and update their positions and
//! then later query the tree for the absolute positions of the various nodes. There are no
//! limits on how deep the tree can be or how many children a given node may have.
//!
//! When implementing this API, feel free to assume that the user isn't going to screw anything
//! up (nodes will only be added to the tree once and only after being initialized, every
//! node in the tree will have a unique name and id). You can also assumed that once a node is
//! added to the tree it will not be removed or moved to a different position in the tree.
//!
//! When considering various designs, please try to satisfy the following criteria in order
//! (from most important to least important)
//! 1) Readable and maintainable. Less code is more.
//! 2) Minimize memory utilization
//! 3) Minimize runtime.
//!
//! You may only use the standard C library.
//!
//! Thanks so much for partipating! Good luck!

struct Position {
    int x;
    int y;
};

//! Individual nodes in the tree
struct LayoutNode {
    struct LayoutNode* childrenHeadPtr;
    struct LayoutNode* childrenEndPtr;
    struct LayoutNode* nextSiblingPtr;
    struct Position relativePosition; // relative position or absolute position?
    struct Position absolutePosition;
    char* name;
    int id;
};

//! The layout tree
struct Layout {
    struct LayoutNode* rootPtr;
};

//! Initialize node with the given values
//! @param node the node to initialize
//! @param name A name for the node. You do not have to deep copy this value
//! @param id An id for the node
//! @param position A relative position for this node with respect to it's parent
void layout_node_init(struct LayoutNode* node, const char* name, int id, struct Position position);

//! Add an initialized node as the root of the tree
void layout_init(struct Layout* layout, struct LayoutNode* root);
//! Add an intialized node as the child of another node already in the Layout tree
void layout_add_child(struct Layout* layout, struct LayoutNode* parent, struct LayoutNode* child);

//! Change the position of a node. This should work whether or not the node is already in a tree
void layout_node_update_position(struct Layout* layout, struct LayoutNode* node, struct Position position);

//! @return the absolute position for the node with the given memory address
struct Position layout_get_position_for_node(struct Layout* layout, struct LayoutNode* node);
//! @return the absolute position for the node with the given name
struct Position layout_get_position_for_name(struct Layout* layout, const char* name);
//! @return the absolute position for the node with the given id
struct Position layout_get_position_for_id(struct Layout* layout, int id); 