REPLACE INTO creature_waypoints_manual
   (`group_id`, `waypoint_id`, `position_x`, `position_y`, `position_z`, `wait_time`, `flags`, `forward_emote_oneshot`, `forward_emote_id`, `backward_emote_oneshot`, `backward_emote_id`, `forward_skin_id`, `backward_skin_id`)
VALUES
   (17, 1, -2240.521729, -407.114532, -9.424648, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 2, -2225.764404, -419.251404, -9.360950, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 3, -2200.883789, -440.999634, -5.606637, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 4, -2143.712646, -468.068665, -9.401191, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 5, -2100.811279, -420.980194, -5.322747, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 6, -2079.469482, -392.465576, -10.262321, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 7, -2043.699707, -343.802155, -6.971242, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 8, -2001.858521, -242.533966, -10.763200, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 9, -1924.751343, -119.969292, -11.770116, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 10, -1794.804565, -7.919222, -9.326016, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 11, -1755.206421, 72.430847, 1.121445, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 12, -1734.550049, 116.842003, -4.337420, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 13, -1720.041870, 125.933235, -2.325089, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 14, -1704.406860, 183.596405, 12.065074, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 15, -1674.317261, 201.597565, 11.235887, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 16, -1624.068726, 223.555389, 2.074259, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 17, -1572.863403, 234.714813, 2.305119, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 18, -1542.866943, 277.896759, 20.543165, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 19, -1541.813232, 316.415649, 49.910889, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 20, -1526.979126, 329.664001, 61.835552, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 21, -1524.173584, 335.237610, 63.325703, 0, 256, 0, 0, 0, 0, 0, 0),
   (17, 22, -1513.968506, 355.759338, 63.064487, 0, 256, 0, 0, 0, 0, 0, 0),
   (18, 1, 269.29, -1433.32, 50.31, 0, 0, 0, 0, 0, 0, 0, 0),
   (18, 2, 328.52, -1442.03, 40.5, 0, 0, 0, 0, 0, 0, 0, 0),
   (18, 3, 333.31, -1453.69, 42.01, 0, 0, 0, 0, 0, 0, 0, 0);

   
--
-- Update world_db_version
--
UPDATE `world_db_version` SET `LastUpdate` = '2016-04-25_01_waypoint_manual' WHERE `LastUpdate` = '2016-04-20_01_misc_spawns';