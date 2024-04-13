#ifndef _TEST_H_
#define _TEST_H_

#include <iostream>

#include "DSALB.h"
#include "DTALB.h"
#include "SequeGraph.h"

/*
// type : string
// type I	: minimize the number of active workstations for a given cycle time
// type II	: minimize the cycle time for a given number of workstations
// type E	: minimize the number of active workstations and cycle time
// type F	: check the feasibility for a given number of workstations and cycle time
*/

void testModel(SequeGraph& graph) {
  for (int num_station = 4; num_station < 14; num_station++) {
    cout << endl
         << "******************"
         << "num of station = " << num_station << "******************" << endl;
    DSALB DS(graph, num_station, "II");
    DS.solveModel();
    cout << "=======================================================" << endl;
  }

  for (int num_station = 3; num_station <= 8; num_station++) {
    cout << endl
         << "******************"
         << "num of station = " << num_station << "******************" << endl;
    DTALB DT(graph, num_station, "II");
    DT.solveModel();
    cout << "=======================================================" << endl;
  }
}

#endif  // !_TEST_H_
