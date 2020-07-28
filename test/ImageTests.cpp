#include <Image.h>

#include <gtest/gtest.h>

TEST(Image, Size)
{
    constexpr auto image_height = 20ull;
    constexpr auto image_width = 20ull;
    const auto image = Image<image_width, image_height>();
    EXPECT_EQ(400 * sizeof(Pixel), sizeof(image));
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
