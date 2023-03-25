//
//  main.cpp
//  chess_engine
//
//  Created by Francesco Maio on 11/03/23.
//

#include <iostream>
#include <fstream>
#include <algorithm>
			
#include "engine_algorithm.hpp"
#include "piece.hpp"
#include "board.hpp"

typedef enum{ TEST_SINGLE_PIECE,
              TEST_ALL_MOVES,
              TEST_MOVES_TREE,
              PLAY
}test_id_t;


int main(int argc, const char * argv[]) {
    
    // Init this structure. And check that the pawns have legal moves.
    /*
       0 1 2 3 4 5 6 7
     0 o o o o o o o o
     1 o P P o o o o o
     2 p p o o o o o o
     3 o o o o o o o o
     4 o o o o o o o o
     5 o o o o o o o o
     6 o o o o o o o o
     7 o o o o o o o o
     
     */
    
    // White pieces
    auto wp = Pawn(WHITE_TEAM);
    auto wr = Rook(WHITE_TEAM);
    auto wb = Bishop(WHITE_TEAM);
    auto wn = Knight(WHITE_TEAM);
    auto wq = Queen(WHITE_TEAM);
    auto wk = King(WHITE_TEAM);
    
    // Black pieces at their start square, then move them artificially to the position to sim a moved state.
    auto bp = Pawn(BLACK_TEAM);
    auto br = Rook(BLACK_TEAM);
    auto bb = Bishop(BLACK_TEAM);
    auto bn = Knight(BLACK_TEAM);
    auto bq = Queen(BLACK_TEAM);
    auto bk = King(BLACK_TEAM);
       
    // Create board state
    board_state_t board_state;
    
    // Init all pointers as null
    for(auto row = board_state.begin(); row != board_state.end(); ++row){
        for(auto col = row->begin(); col != row->end(); ++col){
            *col = nullptr;
        }
    }
    
    // Initialize chess board
    board_state[Board::X('1')][Board::Y('A')] = &wr;
    board_state[Board::X('1')][Board::Y('B')] = &wn;
    board_state[Board::X('1')][Board::Y('C')] = &wb;
    board_state[Board::X('1')][Board::Y('D')] = &wq;
    board_state[Board::X('1')][Board::Y('E')] = &wk;
    board_state[Board::X('1')][Board::Y('F')] = &wb;
    board_state[Board::X('1')][Board::Y('G')] = &wn;
    board_state[Board::X('1')][Board::Y('H')] = &wr;

    board_state[Board::X('2')][Board::Y('A')] = &wp;
    board_state[Board::X('2')][Board::Y('B')] = &wp;
    board_state[Board::X('2')][Board::Y('C')] = &wp;
    board_state[Board::X('2')][Board::Y('D')] = &wp;
    board_state[Board::X('2')][Board::Y('E')] = &wp;
    board_state[Board::X('2')][Board::Y('F')] = &wp;
    board_state[Board::X('2')][Board::Y('G')] = &wp;
    board_state[Board::X('2')][Board::Y('H')] = &wp;

    board_state[Board::X('8')][Board::Y('A')] = &br;
    board_state[Board::X('8')][Board::Y('B')] = &bn;
    board_state[Board::X('8')][Board::Y('C')] = &bb;
    board_state[Board::X('8')][Board::Y('D')] = &bk;
    board_state[Board::X('8')][Board::Y('E')] = &bq;
    board_state[Board::X('8')][Board::Y('F')] = &bb;
    board_state[Board::X('8')][Board::Y('G')] = &bn;
    board_state[Board::X('8')][Board::Y('H')] = &br;

    board_state[Board::X('7')][Board::Y('A')] = &bp;
    board_state[Board::X('7')][Board::Y('B')] = &bp;
    board_state[Board::X('7')][Board::Y('C')] = &bp;
    board_state[Board::X('7')][Board::Y('D')] = &bp;
    board_state[Board::X('7')][Board::Y('E')] = &bp;
    board_state[Board::X('7')][Board::Y('F')] = &bp;
    board_state[Board::X('7')][Board::Y('G')] = &bp;
    board_state[Board::X('7')][Board::Y('H')] = &bp;
    
    // Create the board object with this initialized state
    Board board1(board_state);
    
    board1.print_board_status(std::cout);
    
    
    
    test_id_t test_id = PLAY;
    
    if (test_id == TEST_SINGLE_PIECE ) {
        
        char file = 'B';
        char row  = '8';
        
        point_t pos_start = {Board::X(row),Board::Y(file)};
        
        auto moves = board1.BoardState[pos_start.x][pos_start.y]->get_AllMoves(board1.BoardState,pos_start);
        for(auto move_iterator = moves.begin(); move_iterator != moves.end(); ++move_iterator){
            // Move piece
            Board temp_board(board1.BoardState);
            temp_board.move_piece( (*move_iterator).start_position , (*move_iterator).end_position  );
            temp_board.print_board_status(std::cout);
            std::cout << "Evaluation is: " << temp_board.evaluate_board() << std::endl << std::endl;
        }
        
    }
    
    if (test_id == TEST_ALL_MOVES ) {
        
        team_t player = WHITE_TEAM;
        board_state_t board_state_test;
        
        // Init all pointers as null
        for(auto row = board_state_test.begin(); row != board_state_test.end(); ++row){
            for(auto col = row->begin(); col != row->end(); ++col){
                *col = nullptr;
            }
        }
        
        // Init to wanted position
        board_state_test[Board::X('1')][Board::Y('A')] = &wr;
        board_state_test[Board::X('2')][Board::Y('C')] = &wn;
        board_state_test[Board::X('1')][Board::Y('C')] = &wb;
        board_state_test[Board::X('1')][Board::Y('D')] = &wq;
        board_state_test[Board::X('1')][Board::Y('E')] = &wk;
        board_state_test[Board::X('1')][Board::Y('F')] = &wb;
        board_state_test[Board::X('1')][Board::Y('G')] = &wn;
        board_state_test[Board::X('3')][Board::Y('H')] = &wr;

        board_state_test[Board::X('2')][Board::Y('A')] = &wp;
        board_state_test[Board::X('2')][Board::Y('B')] = &wp;
        board_state_test[Board::X('2')][Board::Y('C')] = &wp;
        board_state_test[Board::X('2')][Board::Y('D')] = &wp;
        board_state_test[Board::X('2')][Board::Y('E')] = &wp;
        board_state_test[Board::X('2')][Board::Y('F')] = &wp;
        board_state_test[Board::X('2')][Board::Y('G')] = &wp;
        board_state_test[Board::X('4')][Board::Y('H')] = &wp;

        board_state_test[Board::X('8')][Board::Y('A')] = &br;
        board_state_test[Board::X('8')][Board::Y('A')] = &bn;
        board_state_test[Board::X('8')][Board::Y('C')] = &bb;
        board_state_test[Board::X('8')][Board::Y('D')] = &bk;
        board_state_test[Board::X('8')][Board::Y('E')] = &bq;
        board_state_test[Board::X('8')][Board::Y('F')] = &bb;
        board_state_test[Board::X('6')][Board::Y('F')] = &bn;
        board_state_test[Board::X('8')][Board::Y('H')] = &br;

        board_state_test[Board::X('7')][Board::Y('A')] = &bp;
        board_state_test[Board::X('7')][Board::Y('B')] = &bp;
        board_state_test[Board::X('7')][Board::Y('C')] = &bp;
        board_state_test[Board::X('7')][Board::Y('D')] = &bp;
        board_state_test[Board::X('7')][Board::Y('E')] = &bp;
        board_state_test[Board::X('7')][Board::Y('F')] = &bp;
        board_state_test[Board::X('7')][Board::Y('G')] = &bp;
        board_state_test[Board::X('7')][Board::Y('H')] = &bp;
        
        Board board_test = Board(board_state_test);
        
        auto moves = board_test.get_all_moves(WHITE_TEAM);
        for(auto move_iterator = moves.begin(); move_iterator != moves.end(); ++move_iterator){
            // Move piece
            Board temp_board(board_test.BoardState);
            temp_board.move_piece( (*move_iterator).start_position , (*move_iterator).end_position  );
            temp_board.print_board_status(std::cout);
            std::cout << "Evaluation is: " << temp_board.evaluate_board() << " with move " << repr_move(*move_iterator) << std::endl << std::endl;
        }
        
        
    }
    
    if (test_id == TEST_MOVES_TREE) {
        printf("Calling ..\n");
        select_best_move(BLACK_TEAM,board1);
    }

    
    
    if (test_id == PLAY) {
        
        
        Board temp_board = Board(board1);
        
        while(1) {
            
            char sx,sy,ex,ey;
            printf("Select your move: ");
            scanf(" %c%c %c%c",&sy,&sx,&ey,&ex);
            move_t mv = {{Board::X(sx),Board::Y(sy)},
                         {Board::X(ex),Board::Y(ey)}};
            
            temp_board.move_piece(mv);
            temp_board.print_board_status(std::cout);
            
            auto opponent_move = select_best_move(BLACK_TEAM,temp_board);
            
            temp_board.move_piece(opponent_move);
            temp_board.print_board_status(std::cout);
            
        }
    }
    
    
    return 0;
}

