//
//  piece.cpp
//  chess_engine
//
//  Created by Francesco Maio on 11/03/23.
//

#include "piece.hpp"
#include <iostream>

bool Piece::check_square_is_occupied(const board_state_t & board_state,const point_t new_pos) {
    return board_state[new_pos.x][new_pos.y] != nullptr;
}

bool Piece::check_square_is_friend(const board_state_t & board_state,const point_t new_pos) {
    return board_state[new_pos.x][new_pos.y]->get_teamID() == this->str_teamID ;
}

bool Piece::check_pos_within_borders(const move_t &test) {
    if (test.end_position.x >= 0 and test.end_position.x < N_SQUARES and test.end_position.y >= 0 and test.end_position.y < N_SQUARES) {
        return true;
    }else {
        return false;
    }
};

auto Piece::remove_outbound_positions(std::forward_list<move_t> & all_positions) {
    
    auto count = all_positions.remove_if([this](move_t test){ return !check_pos_within_borders(test); });
    return count;
};

void Piece::append_position_with_logic(
        std::forward_list<move_t> &moves_list,
        std::forward_list<move_t> &capture_list,
        const board_state_t &board_state,
        const point_t& curr_pos,
        update_pos_fptr update_pos_logic ,
        const int limit,
        const bool stop_if_occupied) {
    
    bool occupied = 0;
    int  n_moves_done = 0;
    point_t new_pos;
    
    std::array<int,2> directions = {{-1,1}};
    for( int d : directions) {
        
        // Reset position and conditions
        new_pos = curr_pos;
        n_moves_done = 0;
        occupied     = 0; // This is bypassed if stop_if_occupied is false, like when the knight moves
        
        update_pos_logic(new_pos,d);
        while( new_pos.x >= 0 and new_pos.x < N_SQUARES and new_pos.y >= 0 and new_pos.y < N_SQUARES and !(occupied&stop_if_occupied) and n_moves_done < limit ) {
            
            if( check_square_is_occupied(board_state, new_pos)) {
                if( !check_square_is_friend(board_state, new_pos)) {
                    // Capture
                    //printf("%s captures at %d %d\n",this->get_name().c_str(), new_pos.x,new_pos.y);
                    capture_list.push_front({curr_pos,new_pos});
                }
                // We have reached a limit by finding another piece, if you can capture you add the move.
                occupied = 1;
            } else {
                //printf("%s moves to %d %d\n",this->get_name().c_str(), new_pos.x,new_pos.y);
                moves_list.push_front({curr_pos,new_pos});
            }
            
            n_moves_done++;
            update_pos_logic(new_pos,d);
        }
    }
    
}

 void Pawn::get_AllMoves(const board_state_t & board_state,
                         const point_t curr_pos,
                         std::forward_list<move_t> & moves,
                         std::forward_list<move_t> & captures ) {
        
    // Find all legal moves
    
    point_t new_pos;
    
    // Moving works as follow. We try all the positions until we are blocked by another piece on the board.
    // By starting at the closest position we know that we are blocked at the first piece encountered.
    
    // Normal advancement
    new_pos = curr_pos;
    new_pos.x += this->i_fwd*1;
    
    if( !check_square_is_occupied(board_state,new_pos) ) {
        moves.push_front({curr_pos,new_pos});
        
        // We can move by 2 only if there wasn't another piece in front (so when we were allowed to move by 1)
        new_pos = curr_pos;
        // Check if x position is 1 for white or 6 for blac, with the help of forward
        // If fwd is 1, 0+1
        // If fwd is -1, 7-1
        // How to generalize?  ( 3.5 - (fwd*2.5) ) ==  ( 7 - (fwd*5) / 2 ) so it's always an int
        if (curr_pos.x == (7 - (this->i_fwd*5)) / 2) {
            new_pos.x += this->i_fwd*2;
            if( !check_square_is_occupied(board_state,new_pos)) {
                moves.push_front({curr_pos,new_pos});
            }
        }
        
    }

    // Check if any capture
    // Capture left
    new_pos = curr_pos;
    new_pos.x += this->i_fwd*1;
    new_pos.y += 1;
    // This is legal only if there is a piece that we want to capture, by the nature of pawns
    if ( check_square_is_occupied(board_state,new_pos) ) {
        if ( !check_square_is_friend(board_state,new_pos) ) {
            captures.push_front({curr_pos,new_pos});
        }
    }
    
    // Repeat to capture right
    new_pos = curr_pos;
    new_pos.x += this->i_fwd*1;
    new_pos.y -= 1;
    
    if ( check_square_is_occupied(board_state,new_pos) ) {
        if ( !check_square_is_friend(board_state,new_pos) ) {
            captures.push_front({curr_pos,new_pos});
        }
    }
    
    // TODO: En passant is missing.
    
    //Piece::remove_outbound_positions(moves);
    
    
};

void Rook::get_AllMoves(const board_state_t & board_state,
                         const point_t curr_pos,
                         std::forward_list<move_t> & moves,
                         std::forward_list<move_t> & captures ) {
    
    // Move horizontal: new_pos.x += d
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += d;
        });
 
    // Move Vertical:
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.y += d;
        });
    
    //moves.unique();
    //Piece::remove_outbound_positions(moves);
};


void Bishop::get_AllMoves(const board_state_t & board_state,
                         const point_t curr_pos,
                         std::forward_list<move_t> & moves,
                         std::forward_list<move_t> & captures ) {
    
    // Move one diagonal
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += d;
        pos.y += d;
        });

    // Move other diagonal
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x -= d;
        pos.y += d;
        });
        
    //moves.unique();
    //Piece::remove_outbound_positions(moves);
};
	
void Queen::get_AllMoves(const board_state_t & board_state,
                         const point_t curr_pos,
                         std::forward_list<move_t> & moves,
                         std::forward_list<move_t> & captures ) {

    
    // Move horizontally
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += d;
        });

    
    // Move Vertical:
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.y += d;
        });
    
    // Move one diagonal
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += d;
        pos.y += d;
        });
    
    // Move other diagonal
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x -= d;
        pos.y += d;
        });
        
    //moves.unique();
    //Piece::remove_outbound_positions(moves);
};

void Knight::get_AllMoves(const board_state_t & board_state,
                         const point_t curr_pos,
                         std::forward_list<move_t> & moves,
                         std::forward_list<move_t> & captures ) {

    append_position_with_logic(moves,  captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += 2*d;
        pos.y += d;
        },1,0);

    
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += d;
        pos.y += 2*d;
        },1,0);
    
    append_position_with_logic(moves,  captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += 2*d;
        pos.y += -d;
        },1,0);

    
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += -d;
        pos.y += 2*d;
        },1,0);
    
    //Piece::remove_outbound_positions(moves);
};

void King::get_AllMoves(const board_state_t & board_state,
                         const point_t curr_pos,
                         std::forward_list<move_t> & moves,
                         std::forward_list<move_t> & captures ) {
    
    // Move horizontally
    append_position_with_logic(moves,  captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += d;
        },1);

    
    // Move Vertical:
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.y += d;
        },1);
    
    // Move one diagonal
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x += d;
        pos.y += d;
        },1);
    
    // Move other diagonal
    append_position_with_logic(moves, captures, board_state, curr_pos,[](point_t &pos,int d){
        pos.x -= d;
        pos.y += d;
        },1);
    
    //moves.unique();
    //Piece::remove_outbound_positions(moves);
};
