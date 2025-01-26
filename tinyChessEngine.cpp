#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

class Piece;
struct coords { int x; int y; };
struct a_move {
  int weight=0;
  int potential=0;
  char piece ;
  coords from;
  coords to;
};
class Square {
  public :
    Piece* p = nullptr; 
    std::vector<Piece*> attacks;
    std::vector<Piece*> pawn_threats;
    std::vector<Piece*> defenders;
    void clear() {
      p = nullptr;
      attacks.clear();
      pawn_threats.clear();
      defenders.clear();
    }
    void set_attack(Piece *attacker) {
      if ( std::count(attacks.begin(), attacks.end(), attacker) == 0) {
      attacks.push_back(attacker);
      }
    }
    void set_pawn_threat(Piece *attacker) {
      if ( std::count(attacks.begin(), attacks.end(), attacker) == 0) {
      pawn_threats.push_back(attacker);
      }
    }
    void set_defender(Piece *attacker) {
      if ( std::count(attacks.begin(), attacks.end(), attacker) == 0) {
      defenders.push_back(attacker);
      }
    }
};

class Piece {
  public:
    int x;
    int y;
    bool white;
    int value = 0;
    char type;
    virtual void set_moves(Square board[8][8], int gx, int gy) {}
    virtual void check_skewers(Square board[8][8], Piece * checks[3], int ckeck_count) { }
};

class Pawn : public Piece {
  int value=10;
  public:
    void set_moves(Square board [8][8], int gx, int gy) override {
      int forwards = -1;
      if (white){forwards = 1;};
      int ny = gy + forwards;
      int nx;
      if (ny <8 && ny >= 0) {
        if ( board[gx][ny].p == nullptr){
            board[gx][ny].set_attack(this);
        }
        nx = gx +1;
        if (nx <8) {
          if ( board[nx][ny].p != nullptr){
            if ( board[nx][ny].p->white != white){
                board[nx][ny].set_attack(this);
            } else {
                board[nx][ny].set_defender(this);
            }
          }
            else {
                board[nx][ny].set_pawn_threat(this);
            }
        }

        nx = gx -1;
        if (nx >=0) {
          if ( board[nx][ny].p != nullptr){
            if ( board[nx][ny].p->white != white){
                board[nx][ny].set_attack(this);
            } else {
                board[nx][ny].set_defender(this);
            }
          }
            else {
                board[nx][ny].set_pawn_threat(this);
            }
        }
      }
    }
};
std::string recursive_seek_attack(Piece *p, Square board[][8], a_move sv) {
  sv.from.x = sv.from.x + sv.to.x;
  sv.from.y = sv.from.y + sv.to.y;
  if ( sv.from.x >7 || sv.from.x <0
      || sv.from.y >7 || sv.from.y <0 ) {
  return std::to_string(sv.from.x) + " "+ std::to_string(sv.from.y) + " out of bounds" ;
  } 
  if ( board[sv.from.x][sv.from.y].p != nullptr) {
    if (p->white != board[sv.from.x][sv.from.y].p->white ) {
        board[sv.from.x][sv.from.y].set_attack(p);
    } else {
        board[sv.from.x][sv.from.y].set_defender(p);
    }
      return std::to_string(sv.from.x) + " x "+ std::to_string(sv.from.y);
  } else {
        board[sv.from.x][sv.from.y].set_attack(p);
          return recursive_seek_attack(p, board, sv);
        }
  return "didn't think of thise";
}

class Rook : public Piece {
  int value=50;
  void set_moves(Square board[8][8], int gx, int gy) override {
    a_move attack_dir;
    attack_dir.from.x = gx;
    attack_dir.from.y = gy;
    attack_dir.to.x = 0;
    attack_dir.to.y = 1;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = 0;
    attack_dir.to.y = -1;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = 1;
    attack_dir.to.y = 0;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = -1;
    attack_dir.to.y = 0;
    recursive_seek_attack(this, board, attack_dir);
   }
};
class Bishop : public Piece {
  int value=30;
void set_moves(Square board[8][8], int gx, int gy) override {
    a_move attack_dir;
    std::string out = "uindefined";
    attack_dir.from.x = gx;
    attack_dir.from.y = gy;
    attack_dir.to.x = 1;
    attack_dir.to.y = 1;
    out =recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = 1;
    attack_dir.to.y = -1;
    out = recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = -1;
    attack_dir.to.y = -1;
    out = recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = -1;
    attack_dir.to.y = 1;
    out = recursive_seek_attack(this, board, attack_dir);
   }
};
class Knight : public Piece {
  int value=30;
void set_moves(Square board[8][8], int gx, int gy) override {
  int dx[] = {-2, -1, 1, 2, 2, 1, -1, -2};
  int dy[] = { 1, 2, 2, 1, -1, -2, -2, -1};
  for (int i = 0; i<8; i++) {
    int newX = x + dx[i];
    int newY = y + dy[i];
    if(newX >=0 && newX <8 && newY >=0 && newY <8) {
      if (board[newX][newY].p == nullptr) {
            board[newX][newY].set_attack(this);
      }else{
        if ( board[newX][newY].p->white != white){
            board[newX][newY].set_attack(this);
        } else {
            board[newX][newY].set_defender(this);
        }
      }
    }
  }
   }
};
class Queen : public Piece {
  int value=90;
void set_moves(Square board[8][8], int gx, int gy) override {
    a_move attack_dir;
    attack_dir.from.x = gx;
    attack_dir.from.y = gy;
    attack_dir.to.x = 0;
    attack_dir.to.y = 1;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = 0;
    attack_dir.to.y = -1;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = 1;
    attack_dir.to.y = 0;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = -1;
    attack_dir.to.y = 0;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = 1;
    attack_dir.to.y = 1;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = 1;
    attack_dir.to.y = -1;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = -1;
    attack_dir.to.y = -1;
    recursive_seek_attack(this, board, attack_dir);
    attack_dir.to.x = -1;
    attack_dir.to.y = 1;
    recursive_seek_attack(this, board, attack_dir);
   }
};
class King : public Piece {
  int value=255;
  void set_moves(Square board[8][8], int gx, int gy) override {
    for (int ix=-1; ix<2; ix++) {
      for (int iy=-1; iy<2; iy++) {
        if( ix == 0 && iy == 0) {
        }else {
          int nx = gx+ix;
          int ny = gy+iy;
          if(nx>=0 && nx<8 && ny>=0 && ny<8) {

            if (board[nx][ny].p == nullptr) {
              bool attacked = false;
              for (auto & attacker : board[nx][ny].attacks) {
                if (attacker->white != white && attacker->type != 'p'){
                  attacked = true;
                }
              }
              for (auto & attacker : board[nx][ny].pawn_threats) {
                if (attacker->white != white){
                  attacked = true;
                }
              }
              if ( attacked == false) {
                board[nx][ny].set_attack(this);
              }
            } else if (board[nx][ny].p->white != white) {
              if (board[nx][ny].defenders.size() == 0) {
                board[nx][ny].set_attack(this);
              }
            }
          }
        }
      }
    }
  }
  void check_skewers(Square board[8][8],Piece * checks[3], int check_count) override {
    for (int i =0; i<check_count; i++ ){
      if ( checks[i]->type == 'q' || checks[i]->type == 'r' || checks[i]->type == 'b') {
        int gx =   x-checks[i]->x;
        int gy =  y-checks[i]->y ;
        if (gx < 0) { gx = -1; }
        else if (gx > 0) { gx = 1;}
        if (gy < 0) { gy = -1; }
        else if (gy > 0) { gy = 1;}
        int nx = x+gx;
        int ny = y+gy;
        if ( nx >7 || nx <0
            || ny >7 || ny <0 ) {} else{
      std::erase_if(board[nx][ny].attacks, [this ](Piece *x) {
          return this == x;
          });
        }
      }
    }
  }
};

Piece* make_piece(char& c, Square board[8][8], int gx, int gy) {
  //takes a character representing the piece
                          bool white = std::isupper(c);
                          if(white){c=std::tolower(c);}
                          switch(c) {
                            case 'k':
                              board[gx][gy].p= new King;
                              board[gx][gy].p->value = 255;
                              break;
                            case 'q':
                              board[gx][gy].p= new Queen;
                              board[gx][gy].p->value = 90;
                              break;
                            case 'b':
                              board[gx][gy].p= new Bishop;
                              board[gx][gy].p->value = 30;
                              break;
                            case 'n':
                              board[gx][gy].p= new Knight;
                              board[gx][gy].p->value = 30;
                              break;
                            case 'r':
                              board[gx][gy].p= new Rook;
                              board[gx][gy].p->value = 50;
                              break;
                            case 'p':
                              board[gx][gy].p= new Pawn;
                              board[gx][gy].p->value = 10;
                              break;
                            default:
                              std::cout << c << "  what the hell happened?" <<std::endl;
                          }
                          board[gx][gy].p->white = white;
                          board[gx][gy].p->x = gx;
                          board[gx][gy].p->y = gy;
                          board[gx][gy].p->type = c;
                          return board[gx][gy].p;
    }

void clear_moves(Square board[8][8], Square allowed[8][8], bool colour) {
  // we're in check, remove all moves that don't prevent check
  for (int gx=0; gx<8; gx++) {
    for (int gy=0; gy<8; gy++) {
      auto allowed_attacks = allowed[gx][gy].attacks;
      std::erase_if(board[gx][gy].attacks, [allowed_attacks, colour](Piece *x) {
          return x->white == colour && std::count(allowed_attacks.begin(), allowed_attacks.end(), x) == 0;
          });
      }
    }
  }

void recursive_pin_search(int kx,int ky,bool kw, Square board[8][8], a_move sv) {
  sv.from.x = sv.from.x + sv.to.x;
  sv.from.y = sv.from.y + sv.to.y;
  if ( sv.from.x >7 || sv.from.x <0
      || sv.from.y >7 || sv.from.y <0 ) {
  return ;
  } 
  if ( board[sv.from.x][sv.from.y].p != nullptr) {
    if (kw == board[sv.from.x][sv.from.y].p->white ) {
      // found piece, check if pinned
      bool pinned = false;
      for (auto & attacker : board[sv.from.x][sv.from.y].attacks) {
          if (attacker->white != kw){
            if ( attacker->type == 'r' || attacker->type == 'q' ) {
              if ( attacker->x == kx || attacker->y == ky) {
                pinned = true;
              }
            }
            if ( attacker->type == 'b' || attacker->type == 'q' ) {
              if ( abs(kx - attacker->x) == abs(ky - attacker->y) ){
                pinned = true;
              }
            }
          }
            
          if (pinned == true) {
            Piece *p = board[sv.from.x][sv.from.y].p;

                for (int gx=0; gx<8; gx++) {
                  for (int gy=0; gy<8; gy++) {
      std::erase_if(board[gx][gy].attacks, [p](Piece *x) {
          return  x == p;
          });
                  }
                }
                return;
          }
      }

      return ;
    }
    else {
      return ;
    }
  } else {
          recursive_pin_search(kx, ky, kw, board, sv);
          return ;
        }
}

void set_moves(Square board[8][8]) {
  Piece* kings[2];
  int king_count = 0;
  for (int gx=0; gx<8; gx++) {
    for (int gy=0; gy<8; gy++) {
      if (board[gx][gy].p != nullptr) {
        if (board[gx][gy].p->type == 'k') {
          kings[king_count] = board[gx][gy].p;
          king_count += 1;
        }else {
          board[gx][gy].p->set_moves(board, gx, gy);
        }
      }
    }
  }

  for( int i=0; i <king_count; i++){

    int gx = kings[i]->x;
    int gy = kings[i]->y;
    bool king_col = kings[i]->white;
    Square allowed_moves[8][8];

    Piece* all_attackers[3];// I presume no more than two pieces can check a king. Otherwise we'll have a segmentation fault
    int attacker_count = 0;
    for (auto & attacker : board[gx][gy].attacks) {
        if (attacker->white != king_col){
          //we are in check
          all_attackers[attacker_count] = attacker;
          attacker_count +=1;
        }
    }
    if (attacker_count == 0) {
      //all good
      int dx[] = { 0,  0, 1, -1, 1,  1, -1, -1};
      int dy[] = { 1, -1, 0,  0, 1, -1,  1, -1};
      a_move sv;
      sv.from.x = gx;
      sv.from.y = gy;
      for (int i = 0; i<8; i++) {
        sv.to.x = dx[i];
        sv.to.y = dy[i];
        recursive_pin_search(gx, gy, king_col, board, sv);
          }
      kings[i]->set_moves(board, gx,gy);
    }
    else if (attacker_count == 1) {
      //single piece attacking king, blocks are possible
      auto attacker = all_attackers[0];
        if (attacker->type == 'n'){
          int ax = attacker->x;
          int ay = attacker->y;
          for (auto & remover : board[ax][ay].attacks) {
            if (remover->white == king_col) {
              allowed_moves[ax][ay].set_attack(remover);
            }
          }
          clear_moves(board, allowed_moves, king_col);
          kings[i]->set_moves(board, gx,gy);
          kings[i]->check_skewers(board, all_attackers,attacker_count);
        } else {
          clear_moves(board, allowed_moves, king_col);
          kings[i]->set_moves(board, gx,gy);
          kings[i]->check_skewers(board, all_attackers,attacker_count);
        }
    } else {
          clear_moves(board, allowed_moves, king_col);
          kings[i]->set_moves(board, gx,gy);
          kings[i]->check_skewers(board, all_attackers,attacker_count);
    }
  }
}


std::vector<a_move> get_moves(Square board[8][8], bool white_move) {
  int n =0;
  std::vector<a_move> all_moves;
  for (int gx=0; gx<8; gx++) {
    for (int gy=0; gy<8; gy++) {
      for (auto & attacker : board[gx][gy].attacks) {
        if (attacker->white == white_move){
          a_move add_move;
          add_move.from.x = attacker->x;
          add_move.from.y = attacker->y;
          add_move.to.x = gx;
          add_move.to.y = gy;
          add_move.weight = 0;
          add_move.piece = attacker->type;

          all_moves.push_back(add_move);
        }
      }
    }
  }
  std::sort(all_moves.begin(), all_moves.end(), [](a_move a, a_move b) {
      return a.weight > b.weight;
      });

  return all_moves;
}

std::string transcribe_move(a_move got_move) {
  std::string result = "";
  result += 'a' + got_move.from.x;
  result += std::to_string( got_move.from.y+1 );
  result += 'a' + got_move.to.x;
  result += std::to_string(got_move.to.y+1);
  if( got_move.piece== 'p')
  { if(got_move.to.y ==0 || got_move.to.y == 7) {
    result += "q"; //queen a pawn
                          }}
  return result;
}

std::string transcribe_moves( std::vector<a_move> all_moves) {
  std::string result = "";
  int top_value = -1000;
  int previous_value = -100;
  for (a_move got_move : all_moves ) { 
    if ( true ){
      //( got_move.weight > top_value || got_move.weight == previous_value) {
          result += transcribe_move(got_move);
          result += " ";
          previous_value = got_move.weight;
          if ( top_value < 0) {
            top_value = got_move.weight / 2;
          }
    }
  }
  return result;
}

void boards_from_moves(Square board[8][8], bool white_move, std::vector<a_move> all_moves, std::vector<a_move> second_moves[]) {
  Square out_board[size(all_moves)][8][8];
  int i = 0;
  //std::vector<a_move> second_moves[size(all_moves)];
  for (a_move got_move : all_moves ) { 
    //int map_key = got_move.from.x*8 + got_move.from.y;
    //if ( cleaned_boards.contains(map_to_key) ) {
    //}
    for (int gx=0; gx<8; gx++) {
      for (int gy=0; gy<8; gy++) {
        if (gx == got_move.from.x && gy == got_move.from.y ) {
          out_board[i][gx][gy].p = nullptr;
        } else if (gx == got_move.to.x && gy == got_move.to.y )
        {
          out_board[i][gx][gy].p = board[got_move.from.x][got_move.from.y].p;
        }
        else {
          out_board[i][gx][gy].p = board[gx][gy].p;
        }
        set_moves(out_board[i]);
        second_moves[i] = get_moves(out_board[i], ! white_move);
      }
    }
    i += 1;
  }
}

void board_from_fen( Square board[8][8], std::string board_chars) {
  bool go_search = 0;
  int x =0;
  int y =7;
  for (int gx=0; gx<8; gx++) {
    for (int gy=0; gy<8; gy++) {
      board[gx][gy].clear() ;
    }
  }

    for (char& c: board_chars) {
      if (std::isdigit(c)) {
        x += c - '0';
      }
      else if (c == '/'){
        y=y-1;
        x=0;
      }
      else  {

        make_piece(c, board, x, y);
        x=x+1;
      }
    }
}



class Node {
  public:
    Square board[8][8];
    std::vector<a_move> all_moves;
    Node * all_boards;
    int * move_indices;
    Node * parent;
    int num_children;
    bool white;
    bool boards_built =false;

    void clear_board() {
      for (int gx=0; gx<8; gx++) {
        for (int gy=0; gy<8; gy++) {
          board[gx][gy].clear() ;
        }
      }
    }
    void print_tree( std::string padding ) {
      for (int i =0; i<num_children; i++) { 
        std::cout << padding << transcribe_move(all_moves[i]) <<" " << all_moves[i].weight << std::endl;
        if ( all_boards[i].boards_built) {
          all_boards[i].print_tree(padding + "  ");
        }
      }
    }

    void from_fen(std::string board_chars , bool white_to_move) {
      white = white_to_move;
      clear_board();
      board_from_fen(board, board_chars);
      set_moves(board);
      std::vector<a_move> moves_vector = get_moves(board, white);
      num_children = moves_vector.size();
      all_moves = moves_vector;
      all_boards = new Node[num_children];
      move_indices = new int[num_children] ;
      for (int i =0; i<num_children; i++) { 
        move_indices[i] = i;
      }

      boards_built =false;
    }

    void from_move( a_move got_move, Square parent_board[8][8], bool white_to_move) {
      white = white_to_move;
      clear_board();
        for (int gx=0; gx<8; gx++) {
          for (int gy=0; gy<8; gy++) {
            if (gx == got_move.from.x && gy == got_move.from.y ) {
              board[gx][gy].p = nullptr;
            } else if (gx == got_move.to.x && gy == got_move.to.y )
            {
              board[gx][gy].p = parent_board[got_move.from.x][got_move.from.y].p;
            }
            else {
              board[gx][gy].p = parent_board[gx][gy].p;
            }
          }
        }
            set_moves(board);
            all_moves = get_moves(board,  white_to_move);
            num_children = all_moves.size();
            all_boards = new Node[num_children];
            move_indices = new int[num_children] ;
            for (int i =0; i<num_children; i++) { 
              move_indices[i] = i;
            }

            boards_built =false;
    }

    a_move search_with_pruning( int depth, bool player) {

      if ( num_children ==1 ) {
        return all_moves[0];
      }

      int denominator = depth +1;
      if (denominator <1) {denominator =2;}

            int num_candidates = num_children/denominator;
            if ( num_candidates <2 ){
              num_candidates =2;
            }
            std::vector<int> pruned_moves; 
            pruned_moves.resize(num_children);

            for (int i =0; i<num_children; i++) { 
                pruned_moves[i] = i;
            }
            //pruned_moves = all_moves;

                if (white) {
                  std::sort(pruned_moves.begin(), pruned_moves.end(), [&](int a, int b) {
                        return all_moves[a].weight > all_moves[b].weight;
                        });
                } else {
                    std::sort(pruned_moves.begin(), pruned_moves.end(), [&](int a, int b) {
                        return all_moves[a].weight < all_moves[b].weight;
                        });
                  }

            pruned_moves.resize(num_candidates);
      std::vector<a_move> new_moves; 
      new_moves.resize(num_candidates);
      int ii=0;
      for ( int index : pruned_moves ) {

        a_move temp_move = all_moves[index];

        if ( all_boards[index].num_children > 0 ) {
          temp_move =  all_boards[index].search(depth-1,!player);
        }

        new_moves[ii] = all_moves[index];
        new_moves[ii].weight = temp_move.weight;
        ii +=1;
      }
      std::sort(new_moves.begin(), new_moves.end(), [](a_move a, a_move b) {
                        return a.weight < b.weight;
                        });
      return new_moves[0];
    }

    a_move search( int depth, bool player) {

      a_move chosen_move = all_moves[0];
        if (boards_built == false) {
          for ( int i=0 ;i< num_children; i++ ) {
            //all_moves[i];
            all_boards[i] = Node();
            all_boards[i].from_move( all_moves[i], board,!white); 
            if (depth >1 ) {
            }
          }
          boards_built = true;
        }
      if (depth > 1) {
          for ( int i=0 ;i< num_children; i++ ) {
            a_move check_move;
            check_move.weight = 0 ;
            if ( all_boards[i].num_children > 0 )
            {
              check_move = all_boards[i].search(depth-1,!player);
            } else {
              if (white ) {
                check_move.weight = 1024;
              } else {
                check_move.weight = -1024;
              }
            }
              all_moves[i].weight = check_move.weight;
            if ( white && check_move.weight > chosen_move.weight ) {
              chosen_move = all_moves[i];
            }
            if ( !white && check_move.weight < chosen_move.weight ) {
              chosen_move = all_moves[i];
            }
          }
      } else {
        if (white) { chosen_move.weight = -999999 ; }
        else {chosen_move.weight = 999999 ;}
          for ( int i=0 ;i< num_children; i++ ) {
             all_moves[i].weight =all_boards[i].evaluate();
             if ( white && all_moves[i].weight > chosen_move.weight ) {
              chosen_move = all_moves[i];
             }
             if ( !white && all_moves[i].weight < chosen_move.weight ) {
              chosen_move = all_moves[i];
             }
          }
      }

      return chosen_move;
    }

    int evaluate() {
      int total_score = 0;

      for (int gx=0; gx<8; gx++) {
        for (int gy=0; gy<8; gy++) {
          int multiplier =0;
          bool colour = false;
          if ( board[gx][gy].p != nullptr ) {
            multiplier =board[gx][gy].p->value /2;
            colour = board[gx][gy].p->white;
            if ( colour ) {
              total_score += board[gx][gy].p->value*2;
            } else {
              total_score -= board[gx][gy].p->value*2;
            }
          }
          for (auto & attacker : board[gx][gy].attacks ) {
            if (attacker->white ) {
              total_score +=1 +multiplier;
              if (attacker->type == 'k' ) 
              {total_score +=2;}
            } else {
              total_score -=1 +multiplier;
              if (attacker->type == 'k' ) 
              {total_score -=2;}
            }
          }
          for (auto & defender : board[gx][gy].defenders ) {
            if (defender->white ) {
              total_score +=1 +multiplier;
            } else {
              total_score -=1 +multiplier;
            }
          }
          for (auto & pawn : board[gx][gy].pawn_threats ) {
            if (pawn->white ) {
              total_score += 1+multiplier;
            } else {
              total_score -=1 +multiplier;
            }
          }
        }
      }
      return total_score;
    }
};

void deleteTree(Node *root) {
  if (root == nullptr) {
    return ; }
  for (int i =0; i<root->num_children; i++) { 
  
    deleteTree(&root->all_boards[i]);
  }
  delete root;
}

int main() {
  std::string fen = "";
  int depth = 2;
  while (fen != "end"){
    std::getline(std::cin , fen);

    std::size_t pos = fen.find(" ");
    std::string board_chars = fen.substr(0, pos);
    bool white=fen[pos+1] == 'w';
    if ( pos < size(fen) ) {
      Node *root = new Node;
      root->from_fen( board_chars , white );
      
      a_move out_move =root->search( depth, true );
      out_move = root->search_with_pruning( depth +1, true );
      std::string moves = transcribe_move(out_move);
      std::cout << moves << std::endl;
      delete root;
    }
  }
  return 0;
}

