#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <thread>
#include <chrono>
#include "board_shim.h"

using namespace std;

/**
  Parsing the command-line arguments to the program and extracting the relevant values
 */
bool parse_args(int argc, char* argv[], BrainFlowInputParams& params, int& board_id);
void performStreaming(BrainFlowInputParams params, int board_id);

/**
  Main Function
*/
int main(int argc, char* argv[])
{
    BoardShim::enable_dev_board_logger(); //logging for the development board
    
    BoardShim::get_board_presets(-1);

    // Initialize the parameters
    BrainFlowInputParams params;
    int board_id = 0;
    if (!parse_args(argc, argv, params, board_id))
    {
        return -1;
    }
  
    performStreaming(params, board_id);
    
    return 0;
}

bool parse_args(int argc, char* argv[], BrainFlowInputParams& params, int& board_id)
{
    unordered_map<string, string> argMap;
    bool board_id_found = false;
    
    for (int i = 1; i < argc; i += 2)
    {
        string arg = argv[i];
        string value = argv[i + 1];
        argMap[arg] = value;
    }
    
    if (argMap.count("--board-id") > 0)
    {
        board_id_found = true;
        board_id = stoi(argMap["--board-id"]);
    }
    else
    {
        cerr << "board id is not provided" << endl;
        return false;
    }
    
    // Assign parsed arguments to params object
    if (argMap.count("--ip-address") > 0)
    {
        params.ip_address = argMap["--ip-address"];
    }
    
    if (argMap.count("--ip-port") > 0)
    {
        params.ip_port = stoi(argMap["--ip-port"]);
    }
    
    if (argMap.count("--serial-port") > 0)
    {
        params.serial_port = argMap["--serial-port"];
    }
    
    if (argMap.count("--ip-protocol") > 0)
    {
        params.ip_protocol = stoi(argMap["--ip-protocol"]);
    }
    
    if (argMap.count("--timeout") > 0)
    {
        params.timeout = stoi(argMap["--timeout"]);
    }
    
    if (argMap.count("--other-info") > 0)
    {
        params.other_info = argMap["--other-info"];
    }
    
    if (argMap.count("--mac-address") > 0)
    {
        params.mac_address = argMap["--mac-address"];
    }
    
    if (argMap.count("--serial-number") > 0)
    {
        params.serial_number = argMap["--serial-number"];
    }
    
    if (argMap.count("--file") > 0)
    {
        params.file = argMap["--file"];
    }
    
    if (argMap.count("--master-board") > 0)
    {
        params.master_board = stoi(argMap["--master-board"]);
    }
    
    if (argMap.count("--preset") > 0)
    {
        params.preset = stoi(argMap["--preset"]);
    }
    
    return true;
}

void performStreaming(BrainFlowInputParams params, int board_id)
{
    try
    {
        unique_ptr<BoardShim> board(new BoardShim(board_id, params));
        
        board->prepare_session();
        board->start_stream();
        
        // Sleep for 5 seconds
        this_thread::sleep_for(chrono::seconds(5));
        
        board->stop_stream();
        BrainFlowArray<double, 2> data = board->get_current_board_data(10);
        board->release_session();
        
        // Print the data
        cout << data << endl;
    }
    catch (const BrainFlowException& err)
    {
        BoardShim::log_message((int)LogLevels::LEVEL_ERROR, err.what());
        if (board->is_prepared())
        {
            board->release_session();
        }
    }
}
