#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <thread>
#include <chrono>
#include "board_shim.h"

using namespace std;

class MainClass {
public:
    /**
      Parses the command-line arguments to the program and extracts the relevant values.
      Returns true if the arguments are parsed successfully; otherwise, false.
      */
    bool parse_args(int argc, char* argv[], BrainFlowInputParams& params, int& board_id);

    /**
      Performs the streaming of data from a brain-sensing board, using the provided parameters and board ID.
      */
    void performStreaming(BrainFlowInputParams params, int board_id);
};

#endif  // MAIN_H
