#ifndef _dimension_h_
#define _dimension_h_
#include "vcell.h"
#include <vector>

void gen_combi_mask(int k, vector <unsigned int> &combi_masks);

/* could be row/column or square*/
class Dimn {

 protected:
  vector <Vcell *> vcell_ptr;

 public:
  bool check_solved();
  bool clean_up(int idx);
  bool rule1_solve();
};

bool Dimn::clean_up(int idx)
{
  if (vcell_ptr[idx]->is_solved())
  {
    // remove the number from  all the other neighbors
    for (int i=0;i<idx-1;++i)
    {
      (*vcell_ptr[i]) = (*vcell_ptr[i]) - (*vcell_ptr[idx]);
    }

    for (int i=idx+1;i<9;++i)
    {
      (*vcell_ptr[i]) = (*vcell_ptr[i]) - (*vcell_ptr[idx]);
    }
    return true;
  }
  return false;
}

bool Dimn::check_solved()
{
  Vcell solv_patrn(0);
  for(int cnt=0; cnt<9;++cnt)
  {
    if (clean_up(cnt)) solv_patrn = solv_patrn + *vcell_ptr[cnt];
  }

  return(solv_patrn.match(ALL_MASK));
}

#if 0
bool Dimn::rule1a_solve()
{
  // Look for a unique number in this dimension
  int num_solved = 0;
  for(int cnt=0; cnt<9;++cnt)
  {
    if (!vcell_ptr[cnt]->is_solved())
    {
      Vcell solv_patrn(0);

      // Get the pattern outside the current cell
      for (int i=0;i<cnt-1;++cnt)
      {
        solv_patrn = solv_patrn + *(vcell_ptr[i]);
      }

      for (int i=cnt+1;i<9;++cnt)
      {
        solv_patrn = solv_patrn + *(vcell_ptr[i]);
      }

      // Remove all the surronding cell patterns
      solv_patrn =  (*(vcell_ptr[cnt])) - solv_patrn;

      // Check if there is a unique value
      if (auto soln = solv_patrn.get_solvd())
      {
        // Set the solution
        vcell_ptr[cnt]->set(soln);

        // Clean up surrounding entrys
        clean_up(cnt);

        // total count of number of entrys solved
        ++num_solved;
      }
    }
  }
  return(num_solved>0);
}
#endif

bool Dimn::rule1_solve()
{
  static vector <unsigned int> comb_mask;  // Temporary vriable
  int change_cnt=0;

  // Generalization of Rule 1a.. we start with looking 2 cells at
  // a time, although we could have start with 1 cell. and combined the two rules
  // "cell_group" is 'k' of 9Ck, where we are getting a combination of 'k' cells
  for (int cell_group=1; cell_group<9;++cell_group) {
    comb_mask.clear();
    gen_combi_mask(cell_group, comb_mask);

    Vcell in_cell(0), out_cell(0);
    for (auto mask:comb_mask) {
      // get the incell vs outcell pattern
      for (int cell_cnt=0; cell_cnt<9;++cell_cnt) {
        // Incell
        if (mask&(1<<cell_cnt)) in_cell = in_cell + *(vcell_ptr[cell_cnt]);
        else out_cell = out_cell + *(vcell_ptr[cell_cnt]);
      }

      // Get any unique patrn that lies only in the cell
      Vcell unique_patrn = in_cell - out_cell;

      // check if the unique pattern has 'k' number of elements..
      if (unique_patrn.num_entry() == cell_group)
      {
        // Then we can remove all other entries from these cells except the pattern
        // of intrest
        Vcell remain_patrn = in_cell - unique_patrn;

        // There exists some remaing pattern afer removing the unique entries..
        // Can safely remove this pattern from the in_cells
        if (!remain_patrn.match(0)) {
          for (int cell_cnt=0; cell_cnt<9;++cell_cnt) {
            // Incell
            if (mask&(1<<cell_cnt)) *(vcell_ptr[cell_cnt]) = *(vcell_ptr[cell_cnt]) -remain_patrn;
          }

          // Track if there was a changes
          ++change_cnt;
        }
      }

        // Check if incell pattern == 'k'
      if (in_cell.num_entry() == cell_group)
      {
        // In this case we can safely remove the pattern on the cells outside
        Vcell intersect_pattern =out_cell * in_cell;

        if (!intersect_pattern.match(0)) {

        for (int cell_cnt=0; cell_cnt<9;++cell_cnt) {
          // Outcell
          if (!(mask&(1<<cell_cnt))) *(vcell_ptr[cell_cnt]) = *(vcell_ptr[cell_cnt]) -intersect_pattern;
        }

        // Track if there was a changes
        ++change_cnt;
        }
      }
    }
  }
  return(change_cnt>0);
}

class Row:public Dimn {
private:
  int my_index; // 0--8

public:
  void init(int row_index, Vcell sud_arr[9][9]);
};

void Row::init(int row_index, Vcell sud_arr[9][9])
{
  my_index = row_index;
  for (int cnt=0; cnt<9;++cnt)
  {
    vcell_ptr[cnt]=&(sud_arr[row_index][cnt]);
  }
}


class Col:public Dimn {
private:
  int my_index; // 0--8

public:
  void init(int col_index, Vcell sud_arr[9][9]);
};

void Row::init(int col_index, Vcell sud_arr[9][9])
{
  my_index = col_index;
  for (int cnt=0; cnt<9;++cnt)
  {
    vcell_ptr[cnt]=&(sud_arr[col_index][cnt]);
  }
}


class Sqr:public Dimn {
private:
  int my_index; // 0--
  int my_start_row;
  int my_start_col;

public:
  void init(int sqr_index, Vcell sud_arr[9][9]);
};

void Row::init(int sqr_index, Vcell sud_arr[9][9])
{
  my_index = sqr_index;

  int my_start_row = 3*(sqr_index/3);
  int my_start_col = 3*(sqr_index%3);
  for (int row=0; row<3;++row) {
      for (int col=0; col<3;++col) {
      vcell_ptr[3*row+col]=&(sud_arr[my_start_row+row][my_start_col+col]);
    }
  }
}

