#ifndef _my_cell_
#define _my_cell_

#include <iostream>
#include <vector>

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
  bool is_solved() {  return((cell_info & (cell_info-1)==0)&&(cell_info!=0)); }
  void operator = (int a);
  void clr() {cell_info=ALL_MASK;}
  void set(int a) {cell_info= (1<<a)&ALL_MASK;}
  bool match(int a) {return(cell_info==a);}
  int num_entry() {int entry =0; for (int cnt =1; cnt<10;++cnt) {  if(cell_info&(1<<cnt)) entry++;} return(entry);}
  int get_solvd();
};

Vcell operator + (const Vcell &a, const Vcell &b)
{
  // Union
  Vcell ret;
  ret.cell_info = (a.cell_info|b.cell_info) & ALL_MASK;
  return(ret);
}

Vcell  operator - (const Vcell &a, const Vcell &b)
{
  Vcell ret;
  ret=(a &(~b))&ALL_MASK;
  return(ret);
}

Vcell operator * (const Vcell &a, const Vcell &b)
{
  // Intersection
  Vcell ret;
  ret.cell_info = (a.cell_info&b.cell_info) & ALL_MASK;
  return(ret);
}

void Vcell::operator = (int a)
{
  cell_info= ((1<<a)) & ALL_MASK;
}

int Vcell::get_solvd()
{
  int val =0;
  if (is_solved()) {
    for (int cnt =1; cnt<10;++cnt) {
      if(cell_info&(1<<cnt)) {
        val = cnt;
        break;
      }
    }
  }
  return(val);
}

ostream & operator << (ostream & out,const Vcell &a)
{
#if 0
  out <<"(";
  for (int cnt=0;cnt<8;++cnt) {
    if (a.cell_info&(1<<cnt)) out<<cnt<<",";
  }
  if (a.cell_info&(1<<cnt)) out<<cnt;
  out<<")";
#else
   out << std::hex << a.cell_info;
#endif
  return(out);
}


#endif