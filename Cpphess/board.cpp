//
//  board.cpp
//  chess_engine
//
//  Created by Francesco Maio on 11/03/23.
//

#include "board.hpp"

// Init an empty board as default
Board::Board() {
    for( std::array< Piece*,N_SQUARES> row : BoardState ){
        for (Piece* col : row) {
            col = nullptr;
        }
    }
    turn_number = 0;
}

Board::Board(board_state_t bs) {
    for (int i = 0; i<N_SQUARES; i++) {
        for (int j=0; j<N_SQUARES; j++) {
            BoardState[i][j] = bs[i][j];
        }
    }
    turn_number = 0;
    
};


Board::Board(const Board& copyBoard) {
    for (int i = 0; i<N_SQUARES; i++) {
        for (int j=0; j<N_SQUARES; j++) {
            BoardState[i][j] = copyBoard.BoardState[i][j];
        }
    }
    turn_number = copyBoard.turn_number + 1;
    
};

bool Board::operator == (const Board &b) const
{
    for (int i = 0; i<N_SQUARES; i++) {
        for (int j=0; j<N_SQUARES; j++) {
            if (this->BoardState[i][j] != b.BoardState[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

void Board::print_board_status(std::ostream & strm) {
        
    strm << this->get_status_str_repr() ;
}

std::string Board::get_status_str_repr() {
    
    constexpr int STRLEN = 1024;
    constexpr char FILES[N_SQUARES] = {'A','B','C','D','E','F','G','H'};
    
    char repr[STRLEN] = {0};
    int string_pos = 0;
    
    int row_n = 7;
    
    
    string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"\n");
    
    for(auto row = this->BoardState.rbegin(); row != this->BoardState.rend(); ++row){
        
        string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"%d ",row_n+1);
        
        row_n --;
        for(auto col = row->begin(); col != row->end(); ++col){
            
            if (*col == nullptr ) {
                string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,". ");
            } else {
                string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"%c ",(*col)->get_board_identifier() );
            }
        }
        string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"\n");
    }
    
    string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"  ");
    
    for (int x = 0; x<N_SQUARES; x++) {
        string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"%c ",FILES[x]);
    }
    
    string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"\n\n\n");
    
    return std::string(repr);
}

// Evaluate the board
int Board::evaluate_board()
{
    int score = 0;
    for(auto row = 0; row < N_SQUARES; ++row){
        for(auto col = 0; col < N_SQUARES; ++col){
            if ( this->BoardState[row][col] != nullptr )
            {
                if ( turn_number > 12 )
                {
                    score += this->BoardState[row][col]->get_value() * this->position_correction_late_game[row][col];
                } else {
                    score += this->BoardState[row][col]->get_value() * this->position_correction_early_game[row][col];
                }
                
            }
        }
    }
    return score;
}

void Board::move_piece(point_t pos_s,point_t pos_e) {
    
    int xs = pos_s.x;
    int ys = pos_s.y;
    int xe = pos_e.x;
    int ye = pos_e.y;
    
    this->BoardState[xe][ye] = this->BoardState[xs][ys];
    this->BoardState[xs][ys] = nullptr; // Piece has moved
}

void Board::move_piece(move_t move) {
    
    int xs = move.start_position.x;
    int ys = move.start_position.y;
    int xe = move.end_position.x;
    int ye = move.end_position.y;
    
    this->BoardState[xe][ye] = this->BoardState[xs][ys];
    this->BoardState[xs][ys] = nullptr; // Piece has moved
}

std::forward_list<move_t> Board::get_all_moves(team_t player)
{
    std::forward_list<move_t> all_player_moves;
    
    // First we want to find the captures to see if there is any check. And also it might be helpful for evaluation.
    // So we need to run this for the opponent, to see if they can capture our king

    // Find player's king
    point_t kings_pos;
    for (int x= 0; x<N_SQUARES; x++) {
        for (int y=0; y<N_SQUARES; y++) {
            Piece * p = this->BoardState[x][y];
            if ( p != nullptr ) {
                if ( p->get_teamID() == player and p->get_name() == "KING") {
                    
                    kings_pos = {x,y};
               }
            }
        }
    }



    // TODO: This has to be an iterator over every piece of the player. Too much code.
    
    for (int x= 0; x<N_SQUARES; x++) {
        for (int y=0; y<N_SQUARES; y++) {
            Piece * p = this->BoardState[x][y];
            if ( p != nullptr ) {
                if ( p->get_teamID() == player ) {
                    
                    auto all_captures = std::forward_list<move_t>();
                    auto all_moves    = std::forward_list<move_t>();
                    p->get_AllMoves(this->BoardState, {x,y}, all_moves,all_captures);
                    
                    // Pointer to avoid copies all around
                    auto every_move = std::forward_list<move_t*>();
                    
                    for( auto it = all_moves.begin(); it != all_moves.end(); it++ ) {
                        every_move.push_front( (move_t*)&(*it) );
                    }
                    
                    for( auto it = all_captures.begin(); it != all_captures.end(); it++ ) {
                        every_move.push_front( (move_t*)&(*it) );
                    }
                    
                    for( auto move_ptr : every_move ) {
                        
                        // For every move, if after the move the opponent has any capture on my king, that move is illegal and
                        // I need to remove it from the possible moves
                        
                        // Also note that if I was moving the king, the kings_position will change
                        if( kings_pos.x == move_ptr->start_position.x && kings_pos.y == move_ptr->start_position.y ) {
                            kings_pos.x = move_ptr->end_position.x;
                            kings_pos.y = move_ptr->end_position.y;
                        }
                        
                        Board tb = Board(*this);
                        tb.move_piece(*move_ptr);
                        
                        // Prepare lists to get the opponent captures
                        int check_on_board = 0;
                        for (int i= 0; i<N_SQUARES; i++) {
                            for (int j=0; j<N_SQUARES; j++) {
                                // op as opponent piece
                                auto opponent_captures = std::forward_list<move_t>();
                                auto opponent_moves    = std::forward_list<move_t>();
                                Piece  *op = tb.BoardState[i][j];
                                if ( op != nullptr ) {
                                    if ( op->get_teamID() == (team_t)(player^1) ) {
                                        	
                                        op->get_AllMoves(tb.BoardState, {i,j}, opponent_moves,opponent_captures);
                                        for( auto om : opponent_captures ) {
                                            
                                            if ( om.end_position.x == kings_pos.x && om.end_position.y == kings_pos.y )
                                            {
                                                /*
                                                printf("\nIn this position: %s the opponent can capture the king!\n",tb.get_status_str_repr().c_str());
                                                printf("Because %s %c%c -> %c%c gets the opponent kind, which is at %c%c\n",
                                                       op->get_name().c_str(),
                                                       Board::FILE(om.start_position.y),
                                                       Board::ROW(om.start_position.x),
                                                       Board::FILE(om.end_position.y),
                                                       Board::ROW(om.end_position.x),
                                                       Board::FILE(kings_pos.y),
                                                       Board::ROW(kings_pos.x));
                                                */
                                                check_on_board = check_on_board | 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        
                        // Finally, if none of the opponent captures resulted in my king being taken, the move (*move_ptr) was valid
                        // And I can add it to the list of moves. 
                        if( !check_on_board ){
                            all_player_moves.push_front(*move_ptr);
                        }
                        
                    }
                }
            }
        }
    }
    
    return all_player_moves;
}
