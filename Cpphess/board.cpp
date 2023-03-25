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
                if ( turn_number > 6 )
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
    int x = 0;
    int y = 0;
    auto all_player_moves = std::forward_list<move_t>();
    for(auto row = this->BoardState.begin(); row != this->BoardState.end(); ++row){
        y = 0;
        for(auto col = row->begin(); col != row->end(); ++col){
            if ( (*col) != nullptr ) {
                if ( (*col)->get_teamID() == player ) {
                    
                    // TODO: Possible optimization: lots of copies
                    auto moves = (*col)->get_AllMoves(this->BoardState, {x,y});
                    for( auto m : moves ) {
                        all_player_moves.push_front(m);
                    }
                }
            }
            y++;
        }
        x++;
    }
    
    return all_player_moves;
}
