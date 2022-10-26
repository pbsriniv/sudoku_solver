#include <iostream>
#include "dimension.h"

using namespace std;


class Sudoku{

private :
  Row sud_rows[9];
  Col sud_cols[9];
  Sqr sud_sqrs[9];
  Vcell puzzle[9][9];

public :
  void init(int puzz[9][9]);
  result solve();
  bool rule2_solve(Sqr &min_sq, Dimn &row_col);
  result check();
  void print();
  void print2();
};

void Sudoku::init(int puzz[9][9])
{
  for (int row=0; row<9;++row) {
    for (int col=0; col<9;++col) {
      puzzle[row][col].set(puzz[row][col]);
    }
  }

  for (int row=0; row<9;++row) sud_rows[row].init(row,puzzle);
  for (int col=0; col<9;++col) sud_cols[col].init(col,puzzle);
  for (int sqr=0; sqr<9;++sqr) sud_sqrs[sqr].init(sqr,puzzle);
}

void Sudoku::print()
{
  cout << "{" << endl;
  for (int row=0;row<9;++row) {
    for (int col=0;col<8;++col) {
      cout <<puzzle[row][col].num() <<", ";
    }
    cout <<puzzle[row][8].num() << endl;
  }
  cout << "}" << endl;
}

void Sudoku::print2()
{
  cout << "{" << endl;
  for (int row=0;row<9;++row) {
    sud_rows[row].print();
  }
  cout << "}" << endl;
}


bool Sudoku::rule2_solve(Sqr &min_sq, Dimn &row_col)
{
  int change_cnt = 0;
  vector <int> sq_in_cell, rc_in_cell;
  vector <int> sq_out_cell, rc_out_cell;

  if (min_sq.get_overlap(row_col,sq_in_cell, rc_in_cell))
  {

  #if 0
    cout << " - - - - - - - - -- - - - - - - - -- - - -- - - - - -- - - - - " <<endl;
    cout << " Mini Square Indx " << min_sq.get_idx() << "  " << ((row_col.get_type()==dimn_type::ROW) ? "ROW | " : "COL | ") <<  "Row/Col Index  " << row_col.get_idx() << endl;
    cout << "Mini Square Indxes ";
    for(auto itr:sq_in_cell) cout << itr << ", ";
    cout << endl;
    cout << "Row Col Indxes ";
    for(auto itr:rc_in_cell) cout << itr << ", ";
    cout <<endl;
    cout << " - - - - - - - - -- - - - - - - - -- - - -- - - - - -- - - - - " <<endl;
  #endif

    #if 0
    min_sq.print();
    row_col.print();
    #endif

    gen_compl_cells(sq_in_cell,sq_out_cell);  // Get the out cell list
    gen_compl_cells(rc_in_cell,rc_out_cell);  // Get the out cell list

    Vcell sq_in_cell_patrn =min_sq.get_union(sq_in_cell);
    Vcell rc_in_cell_patrn =row_col.get_union(rc_in_cell);

    // check if the two pattern match.. Fix me
    assert(sq_in_cell_patrn == rc_in_cell_patrn);

    // Get the Outcell pattern
    Vcell rc_out_cell_patrn=row_col.get_union(rc_out_cell);
    Vcell sq_out_cell_patrn=min_sq.get_union(sq_out_cell);

    // 1. Remove Unique pattern from other cells of the mini square
    // check if there are any unique cells in the incell pattern
    Vcell uniq_seq1 = sq_in_cell_patrn - rc_out_cell_patrn;

    // If these cells are set in the row_col out_cells then it is safe to remove
    uniq_seq1 = uniq_seq1 * sq_out_cell_patrn;

    // Pattern can only be in the incell, remove it from the outcells
    change_cnt += min_sq.rem_pattern(uniq_seq1,sq_out_cell);

    // 2. Remove Unique pattern from other cells of the Row/Col

    // check if there are any unique cells in the incell pattern
    Vcell uniq_seq2 = sq_in_cell_patrn - sq_out_cell_patrn;

    // If these cells are set in the row_col out_cells then it is safe to remove
    uniq_seq2 = uniq_seq2 * rc_out_cell_patrn;

    // Pattern can only be in the incell, remove it from the outcells
    change_cnt+=row_col.rem_pattern(uniq_seq2,rc_out_cell);

    #if 0
    cout << "change cnt : " << change_cnt << "Unique Seq1 : " << std::hex << uniq_seq1 <<  "Unique Seq2 " << std::hex << uniq_seq2 <<endl;
    min_sq.print();
    row_col.print();
    #endif

  }
  return(change_cnt>0);
}

// Figure Solved, Unsolved, Error
result Sudoku::check()
{
  result final_result = result::SUCESS;
  for (int row=0; row<9; ++row)
  {
    result dimn_result = sud_rows[row].check_solved();
    if (dimn_result == result::STUCK) { return(result::STUCK);}
    if (dimn_result != result::SUCESS) {final_result = result::ERROR;}
  }

  for (int col=0; col<9; ++col)
  {
    result dimn_result = sud_cols[col].check_solved();
    if (dimn_result == result::STUCK) { return(result::STUCK);}
    if (dimn_result != result::SUCESS) {final_result = result::ERROR;}
  }

  for (int sqr=0; sqr<9; ++sqr)
  {
    result dimn_result = sud_sqrs[sqr].check_solved();
    if (dimn_result == result::STUCK) { return(result::STUCK);}
    if (dimn_result != result::SUCESS) {final_result = result::ERROR;}
  }


  return(final_result);
}

result Sudoku::solve()
{
  // Keep Running the rules until we have no more changes, then check
  bool converged = false;
  while (!converged)
  {
    int n_solved =0;
    for (int row=0;row<9;++row) n_solved+=sud_rows[row].rule1_solve();
    for (int col=0;col<9;++col) n_solved+=sud_cols[col].rule1_solve();
    for (int sqr=0;sqr<9;++sqr) n_solved+=sud_sqrs[sqr].rule1_solve();

    #if 1
    for (int sqr=0;sqr<9;++sqr) {
      for (int row=0;row<9;++row) n_solved+=rule2_solve(sud_sqrs[sqr],sud_rows[row]);
      for (int col=0;col<9;++col) n_solved+=rule2_solve(sud_sqrs[sqr],sud_cols[col]);
    }
    #endif
    #if 0
    cout << "N_solved " << n_solved << endl << endl;
    print();
    #endif

    if (n_solved == 0) converged = true;
  }
return(check());
}

