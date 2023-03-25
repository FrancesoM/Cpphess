//
//  board.hpp
//  chess_engine
//
//  Created by Francesco Maio on 11/03/23.
//

#ifndef board_hpp
#define board_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include "piece.hpp"


class Board{
    
public:
    
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

    
    // Init an empty board as default
    Board() {
        for( std::array< Piece*,N_SQUARES> row : BoardState ){
            for (Piece* col : row) {
                col = nullptr;
            }
        }
        turn_number = 0;
    }
    
    Board(board_state_t bs) {
        for (int i = 0; i<N_SQUARES; i++) {
            for (int j=0; j<N_SQUARES; j++) {
                BoardState[i][j] = bs[i][j];
            }
        }
        turn_number = 0;
        
    };
    
    
    Board(const Board& copyBoard) {
        for (int i = 0; i<N_SQUARES; i++) {
            for (int j=0; j<N_SQUARES; j++) {
                BoardState[i][j] = copyBoard.BoardState[i][j];
            }
        }
        turn_number = copyBoard.turn_number + 1;
        
    };
    
    bool operator == (const Board &b) const
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
    
    void print_board_status(std::ostream & strm) {
            
        int row_n = 7;
            
        for(auto row = this->BoardState.rbegin(); row != this->BoardState.rend(); ++row){
            strm << row_n << " ";
            row_n --;
            for(auto col = row->begin(); col != row->end(); ++col){
                strm << ( *col == nullptr ? '.' : (*col)->get_board_identifier() ) << " ";
            }
            strm << std::endl;
        }
        strm << "  ";
        for (int x = 0; x<N_SQUARES; x++) {
            strm << x << " ";
        }
        strm << std::endl;
        
        strm << std::endl;
        strm << std::endl;
    }
    
    std::string get_status_str_repr() {
        
        constexpr int STRLEN = 1024;
        char repr[STRLEN] = {0};
        int string_pos = 0;
        
        int row_n = 7;
            
        for(auto row = this->BoardState.rbegin(); row != this->BoardState.rend(); ++row){
            
            string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"%d ",row_n);
            
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
            string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"%d ",x);
        }
        
        string_pos += snprintf(&repr[string_pos],STRLEN-string_pos,"\n\n\n");
        
        return std::string(repr);
    }
    
    void print_board_status(board_state_t bs) {
        
        int row_n = 7;
            
        for(auto row = bs.rbegin(); row != bs.rend(); ++row){
            std::cout << row_n << " ";
            row_n --;
            for(auto col = row->begin(); col != row->end(); ++col){
                std::cout << ( *col == nullptr ? '.' : (*col)->get_board_identifier() ) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "  ";
        for (int x = 0; x<N_SQUARES; x++) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
        
        std::cout << std::endl;
        std::cout << std::endl;
    }
    
    
    // Evaluate the board
    int evaluate_board()
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
    
    void move_piece(point_t pos_s,point_t pos_e) {
        
        int xs = pos_s.x;
        int ys = pos_s.y;
        int xe = pos_e.x;
        int ye = pos_e.y;
        
        this->BoardState[xe][ye] = this->BoardState[xs][ys];
        this->BoardState[xs][ys] = nullptr; // Piece has moved
    }
    
    void move_piece(move_t move) {
        
        int xs = move.start_position.x;
        int ys = move.start_position.y;
        int xe = move.end_position.x;
        int ye = move.end_position.y;
        
        this->BoardState[xe][ye] = this->BoardState[xs][ys];
        this->BoardState[xs][ys] = nullptr; // Piece has moved
    }
    
    std::forward_list<move_t> get_all_moves(team_t player)
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
    
    static constexpr int Y(char board_ID) {
        int retval;
        switch (board_ID) {
            case 'A':
                retval = 0;
                break;
            case 'B':
                retval = 1;
                break;
            case 'C':
                retval = 2;
                break;
            case 'D':
                retval = 3;
                break;
            case 'E':
                retval = 4;
                break;
            case 'F':
                retval = 5;
                break;
            case 'G':
                retval = 6;
                break;
            case 'H':
                retval = 7;
                break;
            default:
                retval = 0;
                break;
        }
        return retval;
    }

    static constexpr int X(char board_ID) {
        int retval;
        switch (board_ID) {
            case '1':
                retval = 0;
                break;
            case '2':
                retval = 1;
                break;
            case '3':
                retval = 2;
                break;
            case '4':
                retval = 3;
                break;
            case '5':
                retval = 4;
                break;
            case '6':
                retval = 5;
                break;
            case '7':
                retval = 6;
                break;
            case '8':
                retval = 7;
                break;
            default:
                retval = 0;
                break;
        }
        return retval;
    }
    
    
    
};

#endif /* board_hpp */
