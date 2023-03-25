//
//  piece.hpp
//  chess_engine
//
//  Created by Francesco Maio on 11/03/23.
//

#ifndef piece_hpp
#define piece_hpp

#include <stdio.h>
#include <string>
#include <forward_list>
#include <array>

#define N_SQUARES 8

#define BLACK_FWD -1
#define WHITE_FWD 1

typedef enum{
    BLACK_TEAM = 1,
    WHITE_TEAM = 0,
}team_t;

class point_t{
public:
    int x;
    int y;
    
    bool operator == (const point_t &p)
    {
       if (x == p.x && y == p.y)
          return true;
      return false;
    }
    
};

typedef struct{
    point_t start_position;
    point_t end_position;
}move_t;

inline std::string repr_move(move_t m){
    char s[256] = {0};
    snprintf(s, 256, "{%d,%d} -> {%d,%d}",m.start_position.x,m.start_position.y,m.end_position.x,m.end_position.y);
    return std::string(s);
}

// This is used to pass around the state of the board. Type is a const, even if the object is not const, so that we don't accidentally modify it.
// Forward declaration just to let the compiler know that Piece will exist
class Piece;
typedef std::array<std::array< Piece*,N_SQUARES>, N_SQUARES> board_state_t;

typedef void (*update_pos_fptr)(point_t&, int);

class Piece {
    
    // Define as protected so base classes can access the variables.
protected:
    std::string str_name;
    std::string str_boardID;
    team_t      str_teamID;
    
    int i_value;
    int i_fwd;
    
    // The piece has no knowledge of the rest of the board, so checks only that a move doesn't go outside the board
    bool check_pos_within_borders(const move_t &test);
    bool check_square_is_occupied(const board_state_t & board_state,const point_t new_pos);
    bool check_square_is_friend(const board_state_t & board_state,const point_t new_pos);
    
    auto remove_outbound_positions(std::forward_list<move_t> & all_positions);
    
    // Each piece has it's own way of updating its position, however the boilerplate code that
    // checks if the square is occupied and so on it's repeated all over the place. It's nice
    // if we have a generic function that takes as input the logic on how to update the position
    void append_position_with_logic(
            std::forward_list<move_t> &moves_list,
            const board_state_t &board_state,
            const point_t& curr_pos,
            update_pos_fptr update_pos_logic,
            const int limit = N_SQUARES,
            const bool stop_if_occupied=1);
    
public:
    // Construtctor - ID for the piece
    Piece(const std::string &name,const team_t &team, const std::string boardID, const int value) :
    str_name(name),
    str_teamID(team),
    str_boardID(boardID){
        i_fwd   = ( team == WHITE_TEAM ? WHITE_FWD : BLACK_FWD);
        i_value = i_fwd*value;
    };
    
    
    inline std::string get_name() const {
        return this->str_name;
    }
    
    inline team_t get_teamID() const {
        return this->str_teamID;
    }
    
    inline char get_board_identifier() const {
        if ( this->str_teamID == WHITE_TEAM ){
            return this->str_boardID.c_str()[0];
        } else {
            return std::tolower(this->str_boardID.c_str()[0]);
        }
    }
    
    inline int get_value() const {
        return this->i_value;
    }
    
    // Virtual method that returns a list of all the possible positions that the piece could go
    virtual std::forward_list<move_t> get_AllMoves(const board_state_t & board_state, const point_t curr_pos) {
        return std::forward_list<move_t>();
    };

};

// Define pieces which have different move and capture methods
class Pawn: public Piece{
    
public:
    Pawn(const team_t & ID) : Piece("PAWN",ID,"P",1) {};
    
    std::forward_list<move_t> get_AllMoves(const board_state_t & board_state, const point_t curr_pos);
};

class Rook: public Piece{
    
public:
    Rook(const team_t & ID) : Piece("ROOK",ID,"R",4) {};
    
    std::forward_list<move_t> get_AllMoves(const board_state_t & board_state, const point_t curr_pos);
};

class Bishop: public Piece{
    
public:
    Bishop(const team_t & ID) : Piece("BISHOP",ID,"B",3) {};
    
    std::forward_list<move_t> get_AllMoves(const board_state_t & board_state, const point_t curr_pos);
};

class Knight: public Piece{
    
public:
    Knight(const team_t & ID) : Piece("KNIGHT",ID,"N",3) {};
    
    std::forward_list<move_t> get_AllMoves(const board_state_t & board_state, const point_t curr_pos);
};

class Queen: public Piece{
    
public:
    Queen(const team_t & ID) : Piece("QUEEN",ID,"Q",9) {};
    
    std::forward_list<move_t> get_AllMoves(const board_state_t & board_state, const point_t curr_pos);
};

class King: public Piece{
    
public:
    King(const team_t & ID) : Piece("KING",ID,"K",10000) {};
    
    std::forward_list<move_t> get_AllMoves(const board_state_t & board_state, const point_t curr_pos);
};



/***
 Example
 #include <iostream>

 using namespace std;

 class A {
     protected:
     int x;
     public:
     A(int x) : x(x) {};
     virtual int get_x() { return this->x; };
 };

 class B : public A{
     public:
     B(int x) : A(x) {};
     int get_x() { return 2*x; };
 };

 int main()
 {
     A myA(4);
     B myB(4);
     cout << myA.get_x() <<endl;
     cout << myB.get_x() <<endl;
     return 0;
 }
 
 This prints
 4
 8
 ***/
#endif /* piece_hpp */
