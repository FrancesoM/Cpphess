//
//  engine_algorithm.hpp
//  chess_engine
//
//  Created by Francesco Maio on 25/03/23.
//

#ifndef engine_algorithm_hpp
#define engine_algorithm_hpp

#include <fstream>
#include <algorithm>
#include "chess_tree.hpp"

#include "board.hpp"
#include "piece.hpp"

#define NULL_MOVE {{0,0},{0,0}}


class MoveState{
public:
    
    move_t move; //Move
    Board  bam;  //Board after move
    int    eva;  //Evaluation
    
    // Use this constructor to couple a move and an unrelated evaluation.
    // For instance: if I do this move, I will have this evaluation in X moves.
    MoveState(move_t m,int e);
    
    // Use this constructor to couple a move and its direct evaluation
    MoveState(move_t m,Board b);
    
    bool operator == (const MoveState &other) const;
    
    bool operator < (const MoveState &other) const;
    
    bool operator > (const MoveState &other) const;
    
    
};

// Given an initial configuration of the boards, iterates over the possible moves and finds the
// estimated best one for "player"
move_t select_best_move(team_t, Board &init_board);



#endif /* engine_algorithm_hpp */
