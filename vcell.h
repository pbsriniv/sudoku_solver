#include <iostream>
#include <vector>


#ifndef _VCELL_H_
#define _VCELL_H_

using namespace std;

static unsigned int ALL_MASK=0x3fe;

class Vcell {

  friend Vcell  operator + (const Vcell &a, const Vcell &b);
  friend Vcell  operator - (const Vcell &a, const Vcell &b);
  friend Vcell  operator * (const Vcell &a, const Vcell &b);
  friend ostream & operator << (ostream & out,const Vcell &a);

  private:
  unsigned int  cell_info;

  public :
  Vcell(): cell_info(ALL_MASK) {}
  Vcell(unsigned int mask): cell_info(mask) {}
  bool is_solved() {  return(((cell_info & (cell_info-1))==0)&&(cell_info!=0)); }
  void operator = (int a);
  bool operator == (Vcell a) { return(cell_info == a.cell_info);}
  void clr() {cell_info=ALL_MASK;}
  void set(int a) { cell_info = ((a>0)&&(a<10)) ? ((1<<a)&ALL_MASK) : ALL_MASK;}
  bool match(int a) {return(cell_info==a);}
  int num_entry() {int entry =0; for (int cnt =1; cnt<10;++cnt) {  if(cell_info&(1<<cnt)) entry++;} return(entry);}
  int get_solvd();
  int num() {
    if(is_solved()) {
      int cnt=1;
      for (; cnt<10;++cnt) { if (cell_info & (1<<cnt)) break;}
      return(cnt);
    }
    return 0;
  }
};

#endif