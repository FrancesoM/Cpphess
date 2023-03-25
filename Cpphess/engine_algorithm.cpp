//
//  engine_algorithm.cpp
//  chess_engine
//
//  Created by Francesco Maio on 25/03/23.
//

#include "engine_algorithm.hpp"
#include "dbg_helpers.h"

// Use this constructor to couple a move and an unrelated evaluation.
// For instance: if I do this move, I will have this evaluation in X moves.
MoveState::MoveState(move_t m,int e) : move(m),eva(e) {};

// Use this constructor to couple a move and its direct evaluation
MoveState::MoveState(move_t m,Board b) : move(m),bam(b) {
    eva = b.evaluate_board();
};

bool MoveState::operator == (const MoveState &other) const
{
    if(this->move.start_position.x == other.move.start_position.x &&
       this->move.start_position.y == other.move.start_position.y &&
       this->move.end_position.x == other.move.end_position.x     &&
       this->move.end_position.y == other.move.end_position.y     &&
       this->eva == other.eva                                     &&
       this->bam == other.bam ) {
        
        return 0;
    }
    return 1;
}

bool MoveState::operator < (const MoveState &other) const
{
    if (this->eva < other.eva) {
        return 1;
    }
    else {
        return 0;
    }
}

bool MoveState::operator > (const MoveState &other) const
{
    if (this->eva > other.eva) {
        return 1;
    }
    else {
        return 0;
    }
}


move_t select_best_move(team_t player, Board &init_board){
    
    // Create root of the node.
    
    // For loop that finds leaves. For each leaf
    Tree<MoveState> tr(MoveState(NULL_MOVE,init_board));
    
    auto bestMoves = std::vector<MoveState>();
    
    const int MAX_DEPTH = 6;
    for (int depth=0; depth<MAX_DEPTH; depth++) {
        
        
        // Get all leaves and iterate over them
        auto leaves = tr.find_leaves();
        
        LOG(INFO,"%d: with %ld leaves\n",depth,std::distance(leaves.begin(),leaves.end()) );
        
        for( auto l_idx: leaves) {

            auto all_moves = tr.nodes[l_idx].data.bam.get_all_moves(player);
            
            LOG(DBG,"\t%d: %ld moves possible\n",depth,std::distance(all_moves.begin(),all_moves.end()) );
            
            // For all the moves, sort them based on what gives the best evaluation after next player has chosen the best move
            auto move_and_response_eva = std::vector<MoveState>();
            
            int move_idx = 0;
            // Here we enter with all possible moves.
            for(auto move : all_moves) {
                Board temp_board(tr.nodes[l_idx].data.bam);
                temp_board.move_piece(move);
                
                // Set next player
                team_t next_player = (team_t)(player^1);
                
                // Create little subtree for prediction for next player assuming they always make the best move.
                auto response_moves = temp_board.get_all_moves(next_player);
                
                LOG(DBG,"\t%d: %c move #%d can be countered by %c with: \n",
                    depth,
                    player==WHITE_TEAM?'W':'B',
                    move_idx,
                    next_player==WHITE_TEAM?'W':'B');
                
                // Out of these response, assume the next player plays the ones that gets the best evaluation
                int pred_idx = 0;
                auto response_states = std::vector<MoveState>();
                for( auto rm : response_moves){
                    Board pred_board(temp_board);
                    pred_board.move_piece(rm);
                    
                    response_states.emplace_back(rm,pred_board);
                    LOG(DBGX,"\t\t%d %d eval %d \n",depth,pred_idx,pred_board.evaluate_board());
                    pred_idx++;
                    
                    LOG(DBGX,"%s",pred_board.get_status_str_repr().c_str());
                }
                
                if ( player==WHITE_TEAM ) {
                    std::sort(response_states.begin(), response_states.end());
                } else
                {
                    std::sort(response_states.rbegin(), response_states.rend());
                }
                
                
                int worst_scenario_eva = response_states.begin()->eva;
                
                //We want to associate which move has the best benefit for the opponent, so we take the largest (aka the first)
                LOG(DBG,"\t\t%d Worst scenario after move %d for %c is %d \n",
                    depth,
                    move_idx,
                    player==WHITE_TEAM?'W':'B',
                    worst_scenario_eva);
                
                auto am = MoveState(move, worst_scenario_eva );
                move_and_response_eva.push_back(am);
                
                // Debug - print what would be the best response for the next player
                Board pred_board(temp_board);
                pred_board.move_piece( response_states.begin()->move  );
                
                LOG(DBG,"%s",pred_board.get_status_str_repr().c_str());
                move_idx++;
            }
            
            // Now we have an association: current_move -> opponent_best_response (1 depth)
            // We want this as an heuristic to remove some of the possible moves.
            // So we sort the current_moves based on the evaluation after the opponent best response
            
            // If white reverse sort because we want the first elements to be the greatest for white
            if (player==WHITE_TEAM) {
                std::sort(move_and_response_eva.rbegin(), move_and_response_eva.rend());
            } else
            {
                std::sort(move_and_response_eva.begin(), move_and_response_eva.end());
            }
            
            // Be a little more short sighted the more you go in depth. Add only the nodes that are apparently the best.
            for(int i = 0; i<std::min((unsigned long)MAX_DEPTH-depth,move_and_response_eva.size()) ; i++  ) {
                
                auto good_move = move_and_response_eva[i].move;
                
                Board temp_board(tr.nodes[l_idx].data.bam);
                temp_board.move_piece(good_move);
                
                MoveState ms = MoveState(good_move,temp_board);
                tr.add_child_at(ms,l_idx);
                
                LOG(DBG,"\t\t%d Selecting this move: \n%s because the worst scenario for %s is only %d  \n",
                    depth,
                    temp_board.get_status_str_repr().c_str(),
                    player==WHITE_TEAM?'W':'B',
                    move_and_response_eva[i].eva);
                
            }
            
        }
        // Next player
        player = (player == WHITE_TEAM ? BLACK_TEAM : WHITE_TEAM);
    }
    
    auto final_leaves = tr.find_leaves();
#ifdef FILE_DEBUG
    int line_number=0;
    const int max_lines = 20;
#endif

    for( auto l_idx: final_leaves){
        std::vector<int> path_indexes = std::vector<int>();
        
        
        int eval = tr.nodes[l_idx].data.eva;

            
        LOG(DBGX,"Found a good position with eval %d\n",eval);
        tr.find_ancestors(path_indexes, l_idx);
        
        // Path index[0] is the root, so path index[1]
        move_t move_with_predicted_final_evaluation = tr.nodes[ *( path_indexes.end()-2 ) ].data.move;
        int    predicted_final_evaluation           = tr.nodes[ l_idx ].data.eva;
        
        MoveState bm = MoveState( move_with_predicted_final_evaluation, predicted_final_evaluation);
        bestMoves.push_back(bm);
            
#ifdef FILE_DEBUG
        
        if( line_number < 20 ) {
            std::ofstream myfile;
            char fname[256] = {0};
            snprintf(fname,256,"/Users/francescomaio/Documents/CODE/CHESS/chess_engine/lines/line_%d.txt", line_number);
            myfile.open (fname);
            
            auto p_idx = path_indexes.begin();
            while(  p_idx < path_indexes.end() -1 ) {
                tr.nodes[*p_idx].data.repr(myfile);
                p_idx++;
            }
            
            tr.nodes[*p_idx].data.repr(myfile);
            
            myfile.close();
            line_number++;
        }
#endif
        
    }
    
    
    // Now that we have the alledgedly best moves.. let's pick one.
    if (player==WHITE_TEAM) {
        std::sort(bestMoves.rbegin(), bestMoves.rend());
    } else
    {
        std::sort(bestMoves.begin(), bestMoves.end());
    }
     
    move_t ret_move =  bestMoves.begin()->move;
    
    LOG(INFO, "I think that the best move is %d %d -> %d %d because it gives an evaluation of %d with depth %d\n",
           ret_move.start_position.x,
           ret_move.start_position.y,
           ret_move.end_position.x,
           ret_move.end_position.y,
           bestMoves.begin()->eva,
           MAX_DEPTH );
           
    return ret_move;
    
    
}
