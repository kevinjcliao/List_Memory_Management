// list-test.cc

#include "list.h"
#include <logic.h>
#include <iostream>
#include "list-test-bigger.h"

using namespace std;
static ostream &cdebug = cerr;	// This would let me re-define debugging info to be, for example ofstream("/dev/null");


static list function (list parameter)  // call-by-value semantics; 'list & parameter' would be call-by-reference
{
	// note that call-by-value semantics means that the call 'function(n)'
	//  will set up 'parameter' here exactly as would have been done via
	// list parameter = n;
	cdebug << "Hello --- list " <<  (parameter.empty() ? "is" : "is not") << " empty."<< endl;
	parameter = list(15, parameter);
	return list(50, parameter);
	// returning a value semantics: result is copied (as parameters were) and then local value destroyed
}



void small_test()
{
	list l;  // empty list --- empty constructor
	cdebug << "After creation of   l, expecting 0 cons_cells, there are " << list::how_many_allocated_cons_cells_are_there() << endl;
	list m = list(5, l);  // create one cons_cell
	cdebug << "After creation of   m, expecting 1 cons_cells, there are " << list::how_many_allocated_cons_cells_are_there() << endl;
	list n = m;
	cdebug << "After creation of   n, expecting 2 cons_cells, there are " << list::how_many_allocated_cons_cells_are_there() << endl;
	list o = list(50, list(500, m));
	cdebug << "After creation of   o, expecting 3 cons_cells, there are " << list::how_many_allocated_cons_cells_are_there() << endl;
	o=n;  /// assignment
	cdebug << "After assignment to n, expecting 4 cons_cells, there are " << list::how_many_allocated_cons_cells_are_there() << endl;
	list p = function(n);
	cdebug << "After creation of   p, expecting 5 cons_cells, there are " << list::how_many_allocated_cons_cells_are_there() << endl;
}


int main()
{
	bool just_do_small_test = true;
	
	cdebug << "Starting small_test()" << endl;
	small_test();
	cdebug << "Done with small_test(), hoping for 0 cons_cells, there are  " << list::how_many_allocated_cons_cells_are_there() << endl;
	cdebug << "Done with small_test(), expecting number of cons_cell allocations was __FILL_THIS_IN___; the actual number is " << list::how_many_cons_cells_were_ever_created() << endl;
	cdebug << "\n";
#if LIST_IMPLEMENTATION_STRATEGY != LIST_IMPLEMENTATION_STRATEGY_GARBAGE
	assert(list::how_many_allocated_cons_cells_are_there() == 0);
#endif

	if (! just_do_small_test) {
		cdebug << "Starting bigger_test()" << endl;
		bigger_test();
		cdebug << "Done with bigger_test(), hoping for 0 cons_cells, there are " << list::how_many_allocated_cons_cells_are_there() << endl;
		cdebug << "\n";
#if LIST_IMPLEMENTATION_STRATEGY != LIST_IMPLEMENTATION_STRATEGY_GARBAGE
		assert(list::how_many_allocated_cons_cells_are_there() == 0);
#endif
	}
}
