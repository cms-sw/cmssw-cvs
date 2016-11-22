
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "Utilities/General/interface/MapVector.hh"

using std::cout;
using std::cerr;
using std::endl;
using std::atoi;

typedef int key;

// -------------------------
class thing
{
 public:
  thing():value_(0) {}
  thing(double value):value_(value){}

  double value_;
  // double morestuff[6];

  bool operator<(const thing& t) const
  { return value_<t.value_; }
  bool operator==(const thing& t) const
  { return value_==t.value_; }
  thing& operator=(const thing& t)
  { value_=t.value_; return *this; }
};

// -----------
std::ostream& operator<<(std::ostream& ost, const thing& t)
{
  ost << t.value_;
  return ost;
}

// -----------
template <class T>
struct gen
{
  gen():k(0),v(0.0) { }

  T operator()()
  {
    T i(k,v);
    v+=.1;
    k+=1;
    return i;
  }

  key k;
  double v;
};

// -----------
struct MapVectorTest
{
  typedef std::pair<key,thing> mypair;
  typedef MapVector<key,thing> things;
  typedef std::vector<mypair>  vthings;

  // need to define all map parameters because of bug in KCC 3.4
  typedef std::map<key,thing,std::less<key>,
		   std::allocator< std::pair<const key,thing> > > others;

  MapVectorTest():
    max_queries(100000),
    max_elements(100000),
    min_queries(100000),
    min_elements(100000),
    max_iters(100),
    multiplier(10)
  { }

  MapVectorTest(int maxe, int mine, int maxq, int minq, int maxit, int mult):
    max_queries(maxq),
    max_elements(maxe),
    min_queries(minq),
    min_elements(mine),
    max_iters(maxit),
    multiplier(mult)
  { }

  struct vec_adjust
  {
    vec_adjust(things& t):t_(t) {}
    void estimate(int e) { t_.reserve(e); }
    void fillDone() { t_[0]; /* access first element to cause sort */ }
    things& t_;
  };

  struct map_adjust
  {
    map_adjust(others& t):t_(t) {}
    void estimate(int e) { }
    void fillDone() { }
    others& t_;
  };

  void do_both(int elements,int queries);
  void go();


  template <class T,class adjust>
  void do_map(const std::string& name, 
	      vthings& src,
	      int elements,
	      int queries);

  int max_queries;
  int max_elements;
  int min_queries;
  int min_elements;
  int max_iters;
  int multiplier;
};

// -----------

template <class T,class adjust>
void MapVectorTest::do_map(const std::string& name,
			   vthings& src, 
			   int elements,
			   int queries)
{
  T v;
  typename T::iterator iv = v.end();
  std::insert_iterator<T> insv = std::inserter(v,iv);
  adjust a(v);
  a.estimate(elements);
  std::copy( src.begin(), src.end(), insv );
  a.fillDone();


  int bad_count = 0;
  double sum = 0.0;
  int i;
  int m = src.size();
  typename T::iterator vend = v.end();

  for(i=0;i<queries;i++)
    {
      typename T::iterator iter(v.find( src[(int)(i%m)].first ));
      if(iter==vend)
	bad_count++;
      else
	sum+=iter->second.value_;
    }

  if(bad_count > 0) cout << "*** bad count = " << bad_count << endl;

  cout << name << ',' // typeid(T).name() << ',' << elements << ','
       << queries << ',' << sum << ','
       << endl;
}

void MapVectorTest::do_both(int elements, int queries)
{
  int i;
  vthings src;
  gen<mypair> g;

  std::generate_n(std::back_inserter(src),elements,g);
  std::random_shuffle(src.begin(),src.end());

  int iters = max_iters*queries;

  // cerr << "iters = " << iters << endl;

  // cerr << "iter do_map for binmap" << endl;
  for(i=0;i<iters;i++)
    do_map<things,vec_adjust>("binmap",src,elements,queries);

  // cerr << "iter do_map for stdmap" << endl;
  for(i=0;i<iters;i++)
    do_map<others,map_adjust>("stdmap",src,elements,queries);

  cout
    << elements << ',' << queries << ','
    << endl;
}

void MapVectorTest::go()
{
  int i,j;

  cout
    << "elements,queries,"
    << "MapVecSum,"
    << "MapSum,"
    << "MapVecCopyMean,"
    << "MapCopyMean,"
    << "MapVecQueryMean,"
    << "MapQueryMean,"
    << "MapVecCopySum,"
    << "MapCopySum,"
    << "MapVecQuerySum,"
    << "MapQuerySum"
    << endl;

  for(i=min_elements;i<=max_elements;i*=10)
    {
      for(j=min_queries;j<=max_queries;j*=10)
	do_both(i,j);
    }
}

int main(int argc, char* argv[])
{
  if(argc<6)
    {
      cerr << "Usage: " << argv[0]
	   << " max_elements min_elements"
	   << " max_query min_query"
	   << " max_iters"
	   << endl;
      return -1;
    }

  int max_elements = atoi(argv[1]);
  int min_elements = atoi(argv[2]);
  int max_queries  = atoi(argv[3]);
  int min_queries  = atoi(argv[4]);
  int max_iters    = atoi(argv[5]);
	
  MapVectorTest m(max_elements,min_elements,
		  max_queries,min_queries,max_iters,10);

  m.go();
  return 0;
}

