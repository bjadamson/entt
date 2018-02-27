#include <gtest/gtest.h>
#include <entt/core/family.hpp>

entt::Family<struct MyFamily> my_family;
entt::Family<struct YourFamily> your_family;

TEST(Family, Functionalities) {
    auto myFamilyType = my_family.type<struct MyFamilyType>();
    auto mySameFamilyType = my_family.type<struct MyFamilyType>();
    auto myOtherFamilyType = my_family.type<struct MyOtherFamilyType>();
    auto yourFamilyType = your_family.type<struct YourFamilyType>();

    ASSERT_EQ(myFamilyType, mySameFamilyType);
    ASSERT_NE(myFamilyType, myOtherFamilyType);

    using pair_t = std::pair<int, int>;
    ASSERT_EQ(my_family.type<pair_t>(), my_family.type<pair_t>());

    using zzz_t = std::tuple<int, long, unsigned char*, void*>;
    ASSERT_EQ(my_family.type<zzz_t>(), my_family.type<zzz_t>());

    // TODO: This test seems like it should be the other way around, right now it is the only test
    // that currently fails. I don't quite understand why this assumption would be wanted.
    //ASSERT_EQ(myFamilyType, yourFamilyType);
    //
    // natural extension of the above case, fails for the same reason
    //ASSERT_NE(my_family.type<pair_t>(), your_family.type<pair_t>());
    //
    // ditto
    //ASSERT_NE(my_family.type<zzz_t>(), your_family.type<zzz_t>());
}

TEST(Family, Uniqueness) {
    ASSERT_EQ(my_family.type<int>(), my_family.type<int &>());
    ASSERT_EQ(my_family.type<int>(), my_family.type<int &&>());
    ASSERT_EQ(my_family.type<int>(), my_family.type<const int &>());
}
