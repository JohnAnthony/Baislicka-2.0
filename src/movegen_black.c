#include "defs.h"

int find_moves_bP(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t to;
  
  // ep
  if(board->ep)
  {
    // Up & Right
    if((board->ep<<7) & (board->pieces[bP]) & (~U64_COL_A))
    {
      move_list[num_moves] = move_add(board, board->ep<<7, board->ep, EP, bP);
      num_moves++;
    }
    
    // Up & Left
    if((board->ep<<9) & (board->pieces[bP]) & (~U64_COL_H))
    {
      move_list[num_moves] = move_add(board, board->ep<<9, board->ep, EP, bP);
      num_moves++;
    }
  }
  
  // Down 1
  moves = (board->pieces[bP]>>8) & (~board->pieces_all);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_black(board, &move_list[num_moves], to<<8, to);
    
    // Down 2
    if((to&U64_ROW_6) && ((to>>8)&(~board->pieces_all)))
    {
      move_list[num_moves] = move_add(board, to<<8, to>>8, DOUBLE_PAWN, bP);
      num_moves++;
    }
    
    moves = moves^to;
  }
  // Down 1 Left 1
  moves = (board->pieces[bP]>>7) & (board->pieces_colour[WHITE]) & (~U64_COL_H);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_black(board, &move_list[num_moves], to<<7, to);
    moves = moves^to;
  }
  // Down 1 Right 1
  moves = (board->pieces[bP]>>9) & (board->pieces_colour[WHITE]) & (~U64_COL_A);
  while((to = (moves & ~(moves-1))) != 0)
  {
    num_moves += move_add_pawn_black(board, &move_list[num_moves], to<<9, to);
    moves = moves^to;
  }
  
  return num_moves;
}

int find_moves_bN(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->pieces[bN];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_knight(u64_to_sq(from)) & (~board->pieces_colour[BLACK]);
    
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_black(board, from, to, bN);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_bB(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->pieces[bB];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[BLACK]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_black(board, from, to, bB);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_bR(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  copy = board->pieces[bR];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[BLACK]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_black(board, from, to, bR);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_bQ(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Queen (diagonal)
  copy = board->pieces[bQ];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[BLACK]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_black(board, from, to, bQ);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (horizontal and vertical)
  copy = board->pieces[bQ];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[BLACK]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_black(board, from, to, bQ);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_bK(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t to;
  
  // castling
  if(board->castling[bKSC] &&
     !(board->pieces_all&U64_F8) &&
     !(board->pieces_all&U64_G8) &&
     calculate_attacked_white(board, U64_E8) == 0 &&
     calculate_attacked_white(board, U64_F8) == 0 &&
     calculate_attacked_white(board, U64_G8) == 0)
  {
    move_list[num_moves] = move_add(board, U64_E8, U64_G8, bKSC, bK);
    num_moves++;
  }
  if(board->castling[bQSC] &&
     !(board->pieces_all&U64_B8) &&
     !(board->pieces_all&U64_C8) &&
     !(board->pieces_all&U64_D8) &&
     calculate_attacked_white(board, U64_E8) == 0 &&
     calculate_attacked_white(board, U64_D8) == 0 &&
     calculate_attacked_white(board, U64_C8) == 0)
  {
    move_list[num_moves] = move_add(board, U64_E8, U64_C8, bQSC, bK);
    num_moves++;
  }
  
  moves  = (board->pieces[bK]<<8); // Up 1
  moves |= (board->pieces[bK]>>8); // Down 1
  moves |= (board->pieces[bK]<<1) & (~U64_COL_H); // Left 1
  moves |= (board->pieces[bK]>>1) & (~U64_COL_A); // Right 1
  moves |= (board->pieces[bK]<<7) & (~U64_COL_A); // Up 1 Right 1
  moves |= (board->pieces[bK]<<9) & (~U64_COL_H); // Up 1 Left 1
  moves |= (board->pieces[bK]>>7) & (~U64_COL_H); // Down 1 Left 1
  moves |= (board->pieces[bK]>>9) & (~U64_COL_A); // Down 1 Right 1
  moves &= ~board->pieces_colour[BLACK];
  while(moves)
  {
    to = moves & ~(moves-1);
    move_list[num_moves] = add_movecapture_black(board, board->pieces[bK], to, bK);
    num_moves++;
    moves = moves^to;
  }
  
  return num_moves;
}

int find_moves_bB_bQ(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Bishop
  copy = board->pieces[bB];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[BLACK]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_black(board, from, to, bB);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (diagonal)
  copy = board->pieces[bQ];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_diagonal(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[BLACK]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_black(board, from, to, bQ);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}

int find_moves_bR_bQ(s_board* board, s_move* move_list)
{
  assert(board != NULL);
  assert(move_list != NULL);
  
  int num_moves = 0;
  uint64_t moves;
  uint64_t from;
  uint64_t to;
  uint64_t copy;
  
  // Rook
  copy = board->pieces[bR];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[BLACK]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_black(board, from, to, bR);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  // Queen (horizontal and vertical)
  copy = board->pieces[bQ];
  while(copy)
  {
    from = copy & ~(copy-1);
    moves = magic_moves_hor_ver(board->pieces_all, u64_to_sq(from)) & (~board->pieces_colour[BLACK]);
    while(moves)
    {
      to = moves & ~(moves-1);
      move_list[num_moves] = add_movecapture_black(board, from, to, bQ);
      num_moves++;
      moves = moves^to;
    }
    copy = copy^from;
  }
  
  return num_moves;
}
