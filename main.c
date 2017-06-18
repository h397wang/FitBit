#include <stdio.h>

#include "layout.h"

int main(int argc, char** argv) {
    struct Layout layout;

    struct LayoutNode root_node;
    struct LayoutNode node_a, node_b, node_c;
    struct LayoutNode node_aa;

    layout_node_init(&root_node, "r", 0, (struct Position) { .x = 10, .y = 10 });
    layout_init(&layout, &root_node);

    layout_node_init(&node_a, "a", 1, (struct Position) { .x = 10, .y = 10 });
    layout_add_child(&layout, &root_node, &node_a);

    layout_node_init(&node_b, "b", 2, (struct Position) { .x = 20, .y = 40 });
    layout_add_child(&layout, &root_node, &node_b);

    layout_node_init(&node_aa, "aa", 3, (struct Position) { .x = 20, .y = 40 });
    layout_add_child(&layout, &node_a, &node_aa);

    struct Position pos_a = layout_get_position_for_node(&layout, &node_a);
    printf("Node a should be (20, 20), got (%u, %u)\n", pos_a.x, pos_a.y);

    struct Position pos_b = layout_get_position_for_name(&layout, "b");
    printf("Node b should be (30, 50), got (%u, %u)\n", pos_b.x, pos_b.y);

    struct Position pos_aa = layout_get_position_for_id(&layout, 3);
    printf("Node aa should be (40, 60), got (%u, %u)\n", pos_aa.x, pos_aa.y);

    layout_node_update_position(&layout, &node_a, (struct Position) { .x = 10, .y = 20 });

    pos_a = layout_get_position_for_node(&layout, &node_a);
    printf("Node a should be (20, 30), got (%u, %u)\n", pos_a.x, pos_a.y);

    pos_b = layout_get_position_for_node(&layout, &node_b);
    printf("Node b should be (30, 50), got (%u, %u)\n", pos_b.x, pos_b.y);

    pos_aa = layout_get_position_for_node(&layout, &node_aa);
    printf("Node aa should be (40, 70), got (%u, %u)\n", pos_aa.x, pos_aa.y);

    layout_node_update_position(&layout, &node_aa, (struct Position) { .x = 0, .y = 0 });
    pos_aa = layout_get_position_for_node(&layout, &node_aa);
    printf("Node aa should be (20, 30), got (%u, %u)\n", pos_aa.x, pos_aa.y);

    layout_node_update_position(&layout, &root_node, (struct Position) { .x = 0, .y = 0 });
    pos_b = layout_get_position_for_node(&layout, &node_b);
    printf("Node b should be (20, 40), got (%u, %u)\n", pos_b.x, pos_b.y);

    // Update the position of a node not in the tree
    layout_node_init(&node_c, "c", 5, (struct Position) { .x = 10, .y = 10 });
    layout_node_update_position(&layout, &node_c, (struct Position) { .x = 20, .y = 20 });
    struct Position pos_c = layout_get_position_for_node(&layout, &node_c);
    printf("Node c should be (20, 20), got (%u, %u)\n", pos_c.x, pos_c.y);

    // Add a third child to the root node
    layout_add_child(&layout, &root_node, &node_c);
    layout_node_update_position(&layout, &root_node, (struct Position) { .x = 100, .y = 100 });
    pos_c = layout_get_position_for_node(&layout, &node_c);
    printf("Node c should be (120, 120), got (%u, %u)\n", pos_c.x, pos_c.y);

    // Make sure child b is still good
    pos_b = layout_get_position_for_node(&layout, &node_b);
    printf("Node b should be (120, 140), got (%u, %u)\n", pos_b.x, pos_b.y);
}