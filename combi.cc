#include <iostream>
#include <vector>

using namespace std;

void n_choose_k(int n, int k,  vector <int>::const_iterator itr, vector <int> &seq, vector<vector <int>> &res)
{
  for(int cnt1=0; cnt1<n-k+1; ++cnt1)
  {
    seq.push_back(*itr);
    ++itr;

    // Recurse while there are more levels
    if (k>1) n_choose_k(n-cnt1-1, k-1, itr, seq, res);
    else {
         res.push_back(seq);
    }
    seq.pop_back();
  }
}

void gen_combi( int k, vector <vector <int>> &res)
{
  vector <int> start_data = {0,1,2,3,4,5,6,7,8};
  vector <int> seq;

  vector <int>::const_iterator itr =start_data.cbegin();

  n_choose_k(start_data.size(), k, itr, seq, res);
}

void gen_compl_cells(vector <int> const &res, vector <int> &compl_res)
{
  // Generate the seq Mask
  unsigned int mask=0;
  for( auto ent:res) mask = mask|(1u<<ent);

  // Complement Mask
  const unsigned int nine_ones = ((1<<9)-1);
  unsigned int compl_mask = nine_ones & (~mask);

  compl_res.clear();
  for (int cnt=0; cnt<9;++cnt)
  {
    if (compl_mask&(1<<cnt)) compl_res.push_back(cnt);
  }
}



#if 0
int main()
{

vector <vector <int>>  res;

combi(6,res);

int idx=0;
for (auto itr:res)
{
  cout << idx << " : ";
  for (auto itr2: itr)
  cout << itr2 << " ";

  cout<<endl;
  idx++;

}


}
#endif