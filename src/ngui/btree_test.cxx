#include <catch2/catch_test_macros.hpp>
#include <ngui/btree.hxx>
#include <ngui/catpuccin.hxx>
#include <ngui/frame.hxx>
#include <fmt/format.h>
#include <optional>


TEST_CASE( "Tree Construction", "[btree]" ) {
    fmt::println("\nTEST_CASE( Tree Construction, [btree] )");
    using namespace ngui;
    FullBTree<u32> tree(10u);
    tree.split_construct(tree.head(), 12u);
    REQUIRE(tree.head()->data.has_value() == false);
    tree.print();
    REQUIRE(tree.at(0)->data.value() == 10);
    REQUIRE(tree.at(1)->data.has_value() == false);
    REQUIRE(tree.at(2)->data.value() == 12);

    fmt::println("------");
    tree.split_construct(tree.at(2), 14u);
    tree.print_in_order();
    tree.print();

    fmt::println("------");
    tree.split_construct(tree.at(2), 17u);
    tree.print_in_order();
    tree.print();
}

TEST_CASE( "Color Conversion", "[colorscheme]" ) {
    ColorScheme catpuccin(ColorScheme::Red);
    auto str = fmt::format("[{}, {}, {}]", catpuccin.vec3_normalized().r, catpuccin.vec3_normalized().g, catpuccin.vec3_normalized().b);
    INFO(str);
    catpuccin = ColorScheme::Green;
    str = fmt::format("[{}, {}, {}]", catpuccin.vec3_normalized().r, catpuccin.vec3_normalized().g, catpuccin.vec3_normalized().b);
    INFO(str);
    catpuccin = ColorScheme::Blue;
    str = fmt::format("[{}, {}, {}]", catpuccin.vec3_normalized().r, catpuccin.vec3_normalized().g, catpuccin.vec3_normalized().b);
    INFO(str);
}
TEST_CASE( "BTree basic", "[btree]" ) {
    fmt::println("\nTEST_CASE( BTree basic, [btree] )");
    using namespace ngui;
    FullBTree<u32> tree(10u);
    REQUIRE( tree.height() == 1 );
    tree.split(tree.at(0));
    REQUIRE( tree.height() == 2 );
    tree.split(tree.at(0));
    REQUIRE( tree.height() == 3 );
    tree.split(tree.at(0));
    REQUIRE( tree.height() == 4 );
    tree.print_in_order();
}
#ifdef ADSFLJSDLKFJLKAJ
TEST_CASE( "BTree split", "[btree]" ) {
    using namespace ngui;
    FullBTree<u32> tree(10);
    REQUIRE( tree.height() == 1 );
    tree.split(&(tree.head));
    REQUIRE( tree.height() == 2 );
    auto _0 = tree.at(0);
    auto _1 = tree.at(1);
    auto _2 = tree.at(2);
    REQUIRE(_0 != nullptr);
    REQUIRE(_1 != nullptr);
    REQUIRE(_2 != nullptr);
    REQUIRE(_0->data == std::optional<u32>(10));
    REQUIRE(_1->data == std::nullopt);
    REQUIRE(_2->data == std::optional<u32>(10));
}
TEST_CASE( "BTree Traversal", "[btree]" ) {
    using namespace ngui;
    FullBTree<u32> tree(1);

    tree.head.left = new BTreeNode<u32>(2);
    tree.head.right = new BTreeNode<u32>(3);

    tree.head.left->left = new BTreeNode<u32>(4);
    tree.head.left->right = new BTreeNode<u32>(5);
    tree.head.right->left = new BTreeNode<u32>(6);
    tree.head.right->right = new BTreeNode<u32>(7);

    SECTION("Pre Order traversal") {
        std::vector<u32> result;
        tree.pre_order(&tree.head, [&result](BTreeNode<u32> node) {
                result.push_back(node.data.value());
                });
        REQUIRE( result == std::vector<u32>{ 1, 2, 4, 5, 3, 6, 7 } );
    }

    SECTION("In Order traversal") {
        std::vector<u32> result;
        tree.in_order(&tree.head, [&result](BTreeNode<u32> node) {
                result.push_back(node.data.value());
                });
        REQUIRE( result == std::vector<u32>{ 4, 2, 5, 1, 6, 3, 7 } );
        auto x = tree.at(0);
        REQUIRE(x != nullptr);
        REQUIRE( x->data.value() == 4 );
        x = tree.at(1);
        REQUIRE(x != nullptr);
        REQUIRE( x->data.value() == 2 );
        x = tree.at(2);
        REQUIRE(x != nullptr);
        REQUIRE( x->data.value() == 5 );
        x = tree.at(6);
        REQUIRE(x != nullptr);
        REQUIRE( x->data.value() == 7 );
        x = tree.at(10);
        REQUIRE(x == nullptr);
    }

    SECTION("Post Order traversal") {
        std::vector<u32> result;
        tree.post_order(&tree.head, [&result](BTreeNode<u32> node) {
                result.push_back(node.data.value());
                });
        REQUIRE( result == std::vector<u32>{ 4, 5, 2, 6, 7, 3, 1 } );
    }
}
#endif
