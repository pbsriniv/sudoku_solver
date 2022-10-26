#include "dimension.h"

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

result Dimn::check_solved()
{
  Vcell solv_patrn(0);
  int cell_cnt = 0;
  for(int cnt=0; cnt<9;++cnt)
  {
    if (vcell_ptr[cnt]->is_solved()) {
      solv_patrn = solv_patrn + *vcell_ptr[cnt];
      ++cell_cnt;
    }
  }

  if (cell_cnt != solv_patrn.num_entry()) return result::ERROR;
  if (solv_patrn.match(ALL_MASK)) return result::SUCESS;
  return(result::STUCK);
}

bool Dimn::rem_pattern(Vcell rem_pattern, vector <int> const & cells)
{
  // There exists some remaing pattern afer removing the unique entries..
  // Can safely remove this pattern from the in_cells
  if (!rem_pattern.match(0)) {
    for (auto incell_idx:cells) {
      *(vcell_ptr[incell_idx]) = *(vcell_ptr[incell_idx]) -rem_pattern;
      assert(!vcell_ptr[incell_idx]->match(0));
    }
    return true;
  }
  return false;
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

//Dimn::test_cnt =0;

bool Dimn::rule1_solve()
{
  static vector <vector <int>> comb_res;  // Temporary vriable
  static vector <int> compl_res;  // Temporary vriable

  int change_cnt=0;

  // Generalization of Rule 1a.. we start with looking 2 cells at
  // a time, although we could have start with 1 cell. and combined the two rules
  // "cell_group" is 'k' of 9Ck, where we are getting a combination of 'k' cells
  for (int cell_group=1; cell_group<9;++cell_group) {
    comb_res.clear();
    gen_combi(cell_group, comb_res); // Generate all possible combination of "cell_groups" number of cells

    // For each combination
    for (auto comb:comb_res) {
      // Generate the "Outcell" pattern
      compl_res.clear();
      gen_compl_cells(comb, compl_res);

      // get the incell vs outcell pattern
      Vcell in_cell = get_union(comb);
      Vcell out_cell = get_union(compl_res);


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
        // Track if there was a changes

        #if 0
        print();
        cout << " Incell " << in_cell << " Outcell " << out_cell << " Unique Pattern " <<  unique_patrn << endl;
        cout << " Cell_group " << cell_group << endl;
        cout << " Remainig Pattern " << remain_patrn << endl;
        #endif

        if (rem_pattern(remain_patrn, comb)) ++change_cnt;
      }

      // Check if incell pattern == 'k'
      if (in_cell.num_entry() == cell_group)
      {
        // Get the intersecting pattern.. these will be
        // made up the incell entrys..
        // In this case we can safely remove the pattern on the cells outside
        Vcell intersect_pattern =out_cell * in_cell;

        // Track if there was a changes
        if (rem_pattern(intersect_pattern, compl_res)) ++change_cnt;
      }
    }
  }
  return(change_cnt>0);
}


void Row::init(int row_index, Vcell sud_arr[9][9])
{
  my_index = row_index;
  for (int cnt=0; cnt<9;++cnt)
  {
    vcell_ptr[cnt]=&(sud_arr[row_index][cnt]);
  }
}



void Col::init(int col_index, Vcell sud_arr[9][9])
{
  my_index = col_index;
  for (int cnt=0; cnt<9;++cnt)
  {
    vcell_ptr[cnt]=&(sud_arr[cnt][col_index]);
  }
}


void Sqr::init(int sqr_index, Vcell sud_arr[9][9])
{
  my_index = sqr_index;
  my_start_row = 3*(sqr_index/3);
  my_start_col = 3*(sqr_index%3);
  for (int row=0; row<3;++row) {
      for (int col=0; col<3;++col) {
      vcell_ptr[3*row+col]=&(sud_arr[my_start_row+row][my_start_col+col]);
    }
  }
}

bool Sqr::get_overlap(Dimn &dimn, vector <int> &sqr_cell_in, vector <int> &oth_cell_in)
{
  sqr_cell_in.clear();
  oth_cell_in.clear();

 // Check if overlapping dimesntion is a row or col
  if (dimn.get_type()==dimn_type::ROW) {
    int row_idx=dimn.get_idx();
    if ((row_idx>=my_start_row)&&(row_idx<my_start_row+3)) {
      // Overlap exists...
      for (int cnt=0; cnt<3;++cnt) {
        // Overlapping cell ids in SQR cell
        sqr_cell_in.push_back(3*(row_idx-my_start_row)+cnt);
        // Overlapping cell ids in Other dimesnsion
        oth_cell_in.push_back(my_start_col+cnt);
      }
      return true;
    }
  }
  else if (dimn.get_type()==dimn_type::COL) {
    int col_idx=dimn.get_idx();
    if ((col_idx>=my_start_col)&&(col_idx<my_start_col+3)) {
      // Overlap exists...
      for (int cnt=0; cnt<3;++cnt) {
        // Overlapping cell ids in SQR cell
        sqr_cell_in.push_back(3*cnt +(col_idx-my_start_col));
        // Overlapping cell ids in Other dimesnsion
        oth_cell_in.push_back(my_start_row+cnt);
      }
      return true;
    }
  }
  else {
   assert(0); // We dont support overlaps of mini-square
  }
 return false;
}
