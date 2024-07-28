#include <iostream>

#include <gtest/gtest.h>

#include "main.h"
#include "reader.h"
#include "ipaddrpool.h"

struct ip_filter_fixture : public testing::Test
{
    ip_filter_fixture()
    {
        ipPoolSorted.pushBack(1, 1, 1, 1);
        ipPoolSorted.pushBack(1, 2, 1, 1);
        ipPoolSorted.pushBack(1, 2, 2, 1);
        ipPoolSorted.pushBack(1, 2, 2, 10);

        ipPoolSorted.pushBack(46, 70, 1, 1);
        ipPoolSorted.pushBack(46, 70, 10, 1);
        ipPoolSorted.pushBack(46, 70, 10, 3);
        ipPoolSorted.pushBack(46, 70, 100, 100);

        ipPoolSorted.pushBack(127, 0, 0, 46);
        ipPoolSorted.pushBack(12, 46, 0, 1);

        ipPoolSorted.sortDescending();
    }
    IpAddrPool ipPoolSorted;
};

TEST_F(ip_filter_fixture, FirstByte)
{
    /* Arranged in ip_filter_fixture. */
    /* Act. */
    IpAddrPool ipPoolFiltered = ipPoolSorted.filter({1});
    testing::internal::CaptureStdout();
    ipPoolFiltered.print();
    std::string output = testing::internal::GetCapturedStdout();
    /* Assert. */
    EXPECT_STREQ(output.c_str(), "1.2.2.10\n1.2.2.1\n1.2.1.1\n1.1.1.1\n");
}

TEST_F(ip_filter_fixture, FirstBytes)
{
    /* Arranged in ip_filter_fixture. */
    /* Act. */
    IpAddrPool ipPoolFiltered = ipPoolSorted.filter({46, 70});
    testing::internal::CaptureStdout();
    ipPoolFiltered.print();
    std::string output = testing::internal::GetCapturedStdout();
    /* Assert. */
    EXPECT_STREQ(output.c_str(), "46.70.100.100\n46.70.10.3\n46.70.10.1\n46.70.1.1\n");
}

TEST_F(ip_filter_fixture, AnyByte)
{
    /* Arranged in ip_filter_fixture. */
    /* Act. */
    IpAddrPool ipPoolFiltered = ipPoolSorted.filterAny(46);
    testing::internal::CaptureStdout();
    ipPoolFiltered.print();
    std::string output = testing::internal::GetCapturedStdout();
    /* Assert. */
    EXPECT_STREQ(output.c_str(), "127.0.0.46\n46.70.100.100\n46.70.10.3\n"
                                 "46.70.10.1\n46.70.1.1\n12.46.0.1\n");
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}