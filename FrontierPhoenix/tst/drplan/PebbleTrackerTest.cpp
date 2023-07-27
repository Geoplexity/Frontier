#include <gtest/gtest.h>
#include <boost/graph/adjacency_list.hpp>
#include "cluster/Cluster.h"

#include "drplan/plans/pebble_game/PebbleGame2D.h"

using namespace ffnx::pebblegame;


TEST(PebbleTrackerTest, testCreate)
{
    PebbleTracker<int, int> pebbleTracker(10, 1, 1);
}

TEST(PebbleTrackerTest, testPlaceVertPebble)
{
    PebbleTracker<int, int> pebbleTracker(10, 1, 1);

    pebbleTracker.place_vert_pebble(0, 1);
}

TEST(PebbleTrackerTest, testPlaceEdgePebble)
{
    PebbleTracker<int, int> pebbleTracker(10, 1, 1);

    pebbleTracker.place_edge_pebble(0, 1);
}

TEST(PebbleTrackerTest, testDuplicatePlaceFails)
{
    PebbleTracker<int, int> pebbleTracker(10, 1, 1);

    pebbleTracker.place_vert_pebble(0, 1);

    ASSERT_THROW(pebbleTracker.place_vert_pebble(0, 1), std::runtime_error);
}


