#include <lib/BST.cpp>
#include <gtest/gtest.h>
#include <vector>


TEST(BSTTestSuite, InOrderDefault) {
    BinarySearchTree<int> a = {6, 13, 8, 9, 4, 7};
    std::vector<int> ans;
    auto it1 = a.begin();
    auto it2 = a.end();
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<int> ans_correct{4, 6, 7, 8, 9, 13};
    ASSERT_TRUE(ans == ans_correct);
}

TEST(BSTTestSuite, ReverseTest) {
    BinarySearchTree<int> a = {6, 13, 8, 9, 4, 7};
    std::vector<int> ans;
    auto it1 = a.rbegin();
    auto it2 = a.rend();
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<int> ans_correct{13, 9, 8, 7, 6, 4};
    ASSERT_TRUE(ans == ans_correct);
}

TEST(BSTTestSuite, InOrder) {
    BinarySearchTree<int> a = {6, 13, 8, 9, 4, 7};
    std::vector<int> ans;
    auto it1 = a.begin<inorder_tag>();
    auto it2 = a.end<inorder_tag>();
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<int> ans_correct{4, 6, 7, 8, 9, 13};
    ASSERT_TRUE(ans == ans_correct);
}

TEST(BSTTestSuite, PreOrder) {
    BinarySearchTree<int> a;
    a.insert<preorder_tag>(6);
    a.insert<preorder_tag>(1);
    a.insert<preorder_tag>(10);
    a.insert<preorder_tag>(4);
    a.insert<preorder_tag>(5);
    std::vector<int> ans;
    auto it1 = a.begin<preorder_tag>();
    auto it2 = a.end<preorder_tag>();
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<int> ans_correct{6, 1, 4, 5, 10};
    ASSERT_TRUE(ans == ans_correct);
}

TEST(BSTTestSuite, PostOrder) {
    BinarySearchTree<int> a;
    a.insert<postorder_tag>(6);
    a.insert<postorder_tag>(1);
    a.insert<postorder_tag>(10);
    a.insert<postorder_tag>(4);
    a.insert<postorder_tag>(5);
    std::vector<int> ans;
    auto it1 = a.begin<postorder_tag>();
    auto it2 = a.end<postorder_tag>();
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<int> ans_correct{5, 4, 1, 10, 6};
    ASSERT_TRUE(ans == ans_correct);
}

TEST(BSTTestSuite, SizeTest) {
    BinarySearchTree<int> a;
    a.insert<postorder_tag>(6);
    a.insert<postorder_tag>(1);
    a.insert<postorder_tag>(10);
    a.insert<postorder_tag>(4);
    a.insert<postorder_tag>(5);
    std::vector<int> ans;
    auto it1 = a.begin<postorder_tag>();
    auto it2 = a.end<postorder_tag>();
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    ASSERT_TRUE(a.size() == 5);
}

TEST(BSTTestSuite, EmptyTest) {
    BinarySearchTree<int> a;
    
    ASSERT_TRUE(a.empty());
    a.insert(5);
    ASSERT_FALSE(a.empty());
}

TEST(BSTTestSuite, DereferenceTest) {
    BinarySearchTree<int> a(10);

    ASSERT_TRUE(*(a.begin()) == 10);
}

TEST(BSTTestSuite, FindOrder) {
    BinarySearchTree<int> a;
    a.insert<postorder_tag>(6);
    a.insert<postorder_tag>(1);
    a.insert<postorder_tag>(10);
    a.insert<postorder_tag>(4);
    a.insert<postorder_tag>(5);
    
    ASSERT_TRUE(*(a.find(5)) == 5);
}

TEST(BSTTestSuite, EraseTest) {
    BinarySearchTree<int> a = {6, 13, 8, 9, 4, 7};
    std::vector<int> ans;
    auto it1 = a.begin();
    auto it2 = a.end();
    a.erase(a.find(7));
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<int> ans_correct{4, 6, 8, 9, 13};
    ASSERT_EQ(ans, ans_correct);
}

TEST(BSTTestSuite, MultiErase) {
    BinarySearchTree<int> a = {6, 13, 8, 9, 4, 7};
    std::vector<int> ans;
    auto it1 = a.begin();
    auto it2 = a.end();
    a.erase((--(--(--a.end()))), (--a.end()));
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<int> ans_correct{4, 6, 7, 13};
    ASSERT_EQ(ans, ans_correct);
}

TEST(BSTTestSuite, StringInorderTest) {
    BinarySearchTree<std::string> a = {"abs", "mn", "abb", "mnk", "zrt"};
    std::vector<std::string> ans;
    auto it1 = a.begin();
    auto it2 = a.end();
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<std::string> ans_correct{"abb", "abs", "mn", "mnk", "zrt"};
    ASSERT_TRUE(ans == ans_correct);
}

TEST(BSTTestSuite, StringEraseTest) {
    BinarySearchTree<std::string> a = {"abs", "mn", "abb", "mnk", "zrt"};
    std::vector<std::string> ans;
    auto it1 = a.begin();
    auto it2 = a.end();
    a.erase(a.find("mn"));
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<std::string> ans_correct{"abb", "abs", "mnk", "zrt"};
    ASSERT_TRUE(ans == ans_correct);
}

TEST(BSTTestSuite, ContainsTest) {
    BinarySearchTree<std::string> a = {"abs", "mn", "abb", "mnk", "zrt"};
    std::vector<std::string> ans;
    auto it1 = a.begin();
    auto it2 = a.end();
    ASSERT_TRUE(a.contains("mnk"));
    ASSERT_FALSE(a.contains("ki"));
}

TEST(BSTTestSuite, LowerBound) {
    BinarySearchTree<int> a = {6, 13, 8, 9, 4, 7};
    
    ASSERT_EQ(*(a.lower_bound(13)), 13);
    ASSERT_EQ(*(a.lower_bound(3)), 4);
}

TEST(BSTTestSuite, UpperBound) {
    BinarySearchTree<int> a = {6, 13, 8, 9, 4, 7};
    ASSERT_EQ(*(a.upper_bound(8)), 9);
    ASSERT_EQ(*(a.upper_bound(9)), 13);
}

TEST(BSTTestSuite, RefEraseTest) {
    BinarySearchTree<int> a = {6, 13, 8, 9, 4, 7};
    std::vector<int> ans;
    auto it1 = a.begin();
    auto it2 = a.end();
    a.erase(7);
    while (it1 != it2) {
        ans.push_back(*(it1));
        ++it1;
    }
    std::vector<int> ans_correct{4, 6, 8, 9, 13};
    ASSERT_EQ(ans, ans_correct);
}


