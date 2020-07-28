#include <Image.h>

#include <gtest/gtest.h>

TEST(Image, Size)
{
    constexpr auto image_height = 20ull;
    constexpr auto image_width = 20ull;
    const auto image = Image<image_width, image_height>();
    EXPECT_EQ(400 * sizeof(Pixel), sizeof(image));
}

TEST(Image, PixelNumbering)
{
    constexpr auto image_height = 3ull;
    constexpr auto image_width = 3ull;
    auto image = Image<image_width, image_height>();
    EXPECT_EQ(0, image.At(0).at(0).u);
    EXPECT_EQ(0, image.At(0).at(0).v);
    EXPECT_EQ(1, image.At(0).at(1).u);
    EXPECT_EQ(0, image.At(0).at(1).v);
    EXPECT_EQ(2, image.At(0).at(2).u);
    EXPECT_EQ(0, image.At(0).at(2).v);

    EXPECT_EQ(0, image.At(1).at(0).u);
    EXPECT_EQ(1, image.At(1).at(0).v);
    EXPECT_EQ(1, image.At(1).at(1).u);
    EXPECT_EQ(1, image.At(1).at(1).v);
    EXPECT_EQ(2, image.At(1).at(2).u);
    EXPECT_EQ(1, image.At(1).at(2).v);

    EXPECT_EQ(0, image.At(2).at(0).u);
    EXPECT_EQ(2, image.At(2).at(0).v);
    EXPECT_EQ(1, image.At(2).at(1).u);
    EXPECT_EQ(2, image.At(2).at(1).v);
    EXPECT_EQ(2, image.At(2).at(2).u);
    EXPECT_EQ(2, image.At(2).at(2).v);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
