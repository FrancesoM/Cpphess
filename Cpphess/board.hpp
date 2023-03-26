//
//  board.hpp
//  chess_engine
//
//  Created by Francesco Maio on 11/03/23.
//

#ifndef board_hpp
#define board_hpp

#include <forward_list>
#include <stdio.h>
#include <iostream>
#include <string>
#include "piece.hpp"


class Board{
    
public:
    
    
    // ############################################################
    // ######################     Members    ######################
    // ############################################################

    board_state_t BoardState;
    int turn_number;
    
    static constexpr int position_correction_early_game[N_SQUARES][N_SQUARES] =
    {
        {10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10},
        {10,10,11,11,11,11,10,10},
        {10,10,11,12,12,11,10,10},
        {10,10,11,12,12,11,10,10},
        {10,10,11,11,11,11,10,10},
        {10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10},
    };
    
    static constexpr int position_correction_late_game[N_SQUARES][N_SQUARES] =
    {
        {10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10},
        {10,10,10,10,10,10,10,10},
    };

    
    // ############################################################
    // ###################### Constructors   ######################
    // ############################################################

    Board();
    Board(board_state_t bs);
    Board(const Board& copyBoard);
    
    // ############################################################
    // ###################### Core functions ######################
    // ############################################################
    
    // Get an evaluation based on the current pieces on the board
    int evaluate_board();
    
    // Move a piece
    void move_piece(point_t pos_s,point_t pos_e);
    void move_piece(move_t move) ;
    
    // Get all legal moves on the board for selected player
    std::forward_list<move_t> get_all_moves(team_t player);
    
    
    // ############################################################
    // ###################### Representation ######################
    // ############################################################
    
    // Print board to stream
    void print_board_status(std::ostream & strm);
    
    // Returns the representation string used to print the board
    std::string get_status_str_repr();
    
    // Print a generic board
    static void print_board_status(board_state_t bs) ;
    
    
    // ############################################################
    // ######################    Utilities   ######################
    // ############################################################
    
    

    static constexpr int Y(char board_ID) {
        return board_ID - 'A';
    }

    static constexpr int X(char board_ID) {
        return board_ID - '1';
        
    }
    
    static constexpr char FILE(int y) {
        return 'A'+y;
    }

    static constexpr char ROW(int x) {
        return '1'+x;
        
    }
    
    bool operator == (const Board &b) const;
    
};

#endif /* board_hpp */
