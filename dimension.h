#ifndef _dimension_h_
#define _dimension_h_
#include "vcell.h"
#include <vector>
#include <assert.h>

void gen_combi( int k, vector <vector <int>> &res);
void gen_compl_cells(vector <int> const &res, vector <int> &compl_res);
enum class dimn_type {ROW,COL,SQR};
enum class result {SUCESS,ERROR,STUCK};

/* could be row/column or square*/
class Dimn {

 protected:
  vector <Vcell *> vcell_ptr;
  int my_index; // 0--8

 public:
  Dimn() : vcell_ptr(9) {}
  bool clean_up(int idx);
  bool rule1_solve();
  int get_idx() { return(my_index);}
  bool rem_pattern(Vcell rem_pattern, vector <int> const & cells);
  result check_solved();

  virtual void init(int idx, Vcell sud_arr[9][9]) =0;
  virtual dimn_type get_type() =0;
  virtual void print() =0;
  Vcell get_union(vector <int> cell_idxs) {
    Vcell uni_cell(0);
    for (auto cell:cell_idxs) uni_cell = uni_cell + *(vcell_ptr[cell]);
    return(uni_cell);
    }
};

class Row:public Dimn {
private:


public:
  void init(int row_index, Vcell sud_arr[9][9]);
  dimn_type get_type() {return dimn_type::ROW;}
  void print() {
    cout << "ROW " << my_index << " : ";
    for (int cnt=0;cnt<9;++cnt) cout << *vcell_ptr[cnt] << ", ";
    cout<<endl;
  }
};


class Col:public Dimn {
private:

public:
  void init(int col_index, Vcell sud_arr[9][9]);
  dimn_type get_type() {return dimn_type::COL;}
  void print() {
    cout << "COL " << my_index << " : ";
    for (int cnt=0;cnt<9;++cnt) cout << *vcell_ptr[cnt] << ", ";
    cout<<endl;
  }
};

class Sqr:public Dimn {
private:
  int my_start_row;
  int my_start_col;

public:
  void init(int sqr_index, Vcell sud_arr[9][9]);
  bool rule2_solve(Dimn &intrsct);
  dimn_type get_type() {return dimn_type::SQR;}
  bool get_overlap(Dimn &dimn, vector <int> &sqr_cell_in, vector <int> &oth_cell_in);
  void print() {
    cout << "SQR " << my_index << " : ";
    for (int cnt=0;cnt<9;++cnt) cout << *vcell_ptr[cnt] << ", ";
    cout<<endl;
  }

};

#endif