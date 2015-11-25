// list-test.cc

#include "list.h"
#include <logic.h>
#include <iostream>

using namespace std;
static ostream &cdebug = cerr;	// This would let me re-define debugging info to be, for example ofstream("/dev/null");
static ostream &cprompt = cout;

void copy_test()
{
	list l;
	list l2 = l;           // copies empty list
	list m = list(5, l);   // copies empty list
	list m2 = m;            // copies non-empty list
	list n = list(7, list(6, m));  // several copies here
}



  static list function (list parameter)  // call-by-value COPIES the list
  {
	cdebug << "Hello --- list " <<  (parameter.empty() ? "is" : "is not") << " empty."<< endl;
	return list(12, parameter);  // upon return, copied parameter is copied and then destroyed
  }

void function_test()
{
	list l;  // empty list
	list m = list(5, l);
	
	list l2 = function(l);
	list m2 = function(m);
}


void assign_test()
{
	list l;
	list m = list(5, l);
	list assign_me;
	assign_me = l;
	assign_me = m;
	assign_me = m;
	assign_me = l;
	assign_me = m;
	//assign_me = (assign_me = assign_me);
}



list read_a_list()
{
  int i;
  cin >> i;
  if (cin)
    return list(i, read_a_list());
  else
    return list();
}


void print_a_list(const list l)
{
  if (!l.empty())
    {
      cout << l.head() << " ";
      print_a_list(l.rest());
    }
}

void print_a_list_in_reverse(const list &l)
{
  if (!l.empty())
    {
      print_a_list_in_reverse(l.rest());
      cout << l.head() << " ";
    }
}

void still_more_test()
{
	list l;  // empty list
	list m = list(3, l);

  assert(m.head() == 3);
  assert(l.empty());
  assert(!m.empty());
  assert(m.rest().empty());

  cprompt << "Enter some numbers, please, then end-of-file" << endl;

  list n = read_a_list();

  cout << "Here are your numbers: ";
  print_a_list(n);
  cout << endl;

  cout << "Here are your numbers in reverse: ";
  print_a_list_in_reverse(n);
  cout << endl;

  cout << endl;

  list o = n;
  list p;
  p = list (1024, o);
  cout << "Here is p, which is your list with 1024 at the front: ";
  print_a_list(p);
  cout << endl;

  cout << "Now here's p again: ";
  print_a_list(p);
  cout << endl;
}


void bigger_test()
{
	cdebug << "Starting copy_test()" << endl;
	copy_test();
#if LIST_IMPLEMENTATION_STRATEGY != LIST_IMPLEMENTATION_STRATEGY_GARBAGE
	assert(list::how_many_allocated_cons_cells_are_there() == 0);
#endif

	cdebug << "Starting function_test()" << endl;
	function_test();
#if LIST_IMPLEMENTATION_STRATEGY != LIST_IMPLEMENTATION_STRATEGY_GARBAGE
	assert(list::how_many_allocated_cons_cells_are_there() == 0);
#endif

	cdebug << "Starting assign_test()" << endl;
	assign_test();
#if LIST_IMPLEMENTATION_STRATEGY != LIST_IMPLEMENTATION_STRATEGY_GARBAGE
	assert(list::how_many_allocated_cons_cells_are_there() == 0);
#endif


	cdebug << "Starting still_more_test()" << endl;
	still_more_test();
#if LIST_IMPLEMENTATION_STRATEGY != LIST_IMPLEMENTATION_STRATEGY_GARBAGE
	assert(list::how_many_allocated_cons_cells_are_there() == 0);
#endif
	
	cdebug << "If we get here, things look good" << endl;
}
