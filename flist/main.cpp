#include <iostream>
#include "flist.h"
#include "tests.h"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
