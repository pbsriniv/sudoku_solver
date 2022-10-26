#include "vcell.h"


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
  ret.cell_info=(a.cell_info &(~b.cell_info))&ALL_MASK;
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
  set(a);
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
   out << std::hex << a.cell_info << " ";
#endif
  return(out);
}
