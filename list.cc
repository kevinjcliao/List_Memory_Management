#include "list.h"
#include <logic.h>

// *********************************************************************************
//
// First, we define the structure of a "cons cell",
//  which holds the integer value and the rest of the list
//  (the rest of the list is, of course, a list object represented with
//   a pointer to a cons cell ... but when that list object is created
//   or destroyed, C++ knows to call constructors or destructors for it!)
//
// Another line of asterisks will indicate the end of the cons cell operations,
//  and thus the start of the class list functions.
// Class cons_cell is in this .cc file, rather than its own .h file,
//  since it is needed *only* for the implementation of the list class.
//
// Also, in variance with our usual practice, comments will discuss
//  usage of C++ as well as the algorithm/data-structure being defined.
//

	struct list::cons_cell {
		int i;
		list rest_of_the_list;

		static int current_number_of_allocated_cons_cells;  // allocated once = shared by all objects
		static int total_number_of_allocations_of_cons_cells;  // allocated once = shared by all objects
		cons_cell(int val, const list &rest);
		~cons_cell();  // for updating total_number_of_allocated_cons_cells
	};

int list::cons_cell::current_number_of_allocated_cons_cells = 0;  // initialize
int list::cons_cell::total_number_of_allocations_of_cons_cells = 0;  // initialize

#include <iostream>
using namespace std;

static ostream &cdebug = cerr;	// This would let me re-define debugging info to be, for example ofstream("/dev/null");

//
// To create a cons cell,
//  we initialize it's "i" field with the value,
//  and its "rest_of_the_list" field with the "rest" parameter;
//  since the latter is a list, list constructor will be used for it
//  (specifically, the "copy constructor" that creates a list from a list).
//
// This works for both the GARBAGE and the COPY implementations,
//  since the list class' constructor will either copy the cons cells or not.
//
list::cons_cell::cons_cell(int val, const list &rest) : i(val), rest_of_the_list(rest)
{
#if LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_REFCOUNT
#endif
	current_number_of_allocated_cons_cells++;
	total_number_of_allocations_of_cons_cells++;

	if (LIST_DEBUG) {
		cdebug << "Finished running list::cons_cell::cons_cell to create cons_cell at memory location " << this
			<< " with value " << val;
		if (rest_of_the_list.my_head == 0) {
			cdebug << " and rest as a null pointer (0)" << endl;
		} else {
			cdebug << " and rest pointing to cons_cell at location " << rest_of_the_list.my_head << endl;
		}
	}
}

//
// The cons_cell destructor will be called when a cons_cell goes away;
//  note that, since the cons_cell contains a list object,
//  the list destructor will automatically be called as well,
//  and no real action is needed in the cons_cell destructor
//  in any implementation strategy (since the list destructor
//  will destroy the subsequent cons cell(s) if necessary).
//
list::cons_cell::~cons_cell()
{
	current_number_of_allocated_cons_cells--;
	if (LIST_DEBUG) {
		cdebug << "Finished running list::cons_cell::~cons_cell to cons_cell at memory location " << this << endl;
	}
}


// *********************************************************************************
//
//    that's all we need for the cons cell class ... next up, the list operations
//
// *********************************************************************************


//
// First, the easy functions that just return some simple answers:
//

// "empty" and "head" don't create lists or cons cells,
//  so they are the same in all three implementations:
bool list::empty() const
{
	return my_head == 0;
}

int list::head() const
{
	precondition(!empty());
	return my_head->i;  // i.e., (*my_head).i;
}


//
// "rest" creates a list by returning one;
// This will trigger the list "copy constructor" ...
//  in the GARBAGE implementation, the original and returned list share cons cells;
//  in the COPY implementation, the cons cells of the original will be copied
//                              automatically by that list copy constructor,
//                              and nothing else is needed here
//
list list::rest() const
{
	precondition(!empty());
	return my_head->rest_of_the_list;
}


int list::how_many_allocated_cons_cells_are_there()
{
	return cons_cell::current_number_of_allocated_cons_cells;
}

int list::how_many_cons_cells_were_ever_created()
{
	return cons_cell::total_number_of_allocations_of_cons_cells;
}


//
// Creation of an empty list is the same in all implementations,
//  since no cons cells are involved
//
// Note that this is used not only when we write "list()", but also
//	for any uninitialized list variable in a function,
//	and for a list field (or superclass) in some other class type, UNLESS
//		the initializer list notation is used in that class' constructor (as it is in cons_cell)
//

list::list()
{
	my_head = 0;  // Null pointer
	if (LIST_DEBUG) {
		cdebug << "Finished running list::list() to create a null list (0 pointer) in memory location " << this << endl;
	}
}


//
// Creation of a list varies considerably between the implementations
//
list::list(int h, const list &r)
{
#if LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_GARBAGE
	my_head = new cons_cell(h, r);	// "this" and "r" both contain r's cons cells
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_COPY
	my_head = new cons_cell(h, r);	// recall that the cons_cell constructor
	                                // uses the list copy constructor,
                                    // which will make "this" get a copy of r's cons cells.
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_REFCOUNT
#endif

	if (LIST_DEBUG) {
		cdebug << "Finished running list::list(int h, const list &r) to create a new list in memory location " << this
			<< " with head cons_cell in location " << my_head << " (which holds the value " << my_head->i;
		if (my_head->rest_of_the_list.my_head == 0) {
			cdebug << " and rest as a null pointer (0))" << endl;
		} else {
			cdebug << " and rest pointing to cons_cell at location " << my_head->rest_of_the_list.my_head << ")" << endl;
		}
	}
}


//
// This is the list "copy constructor",
//  used when we create a new list that's a copy of an old one, i.e. for
//		* list l2 = l1;	// initialization of a list l2 from a list l1
//		* list l2 = list(l1);	// as above, different notation
//		* passing a list parameter by value in a function call
//		* returning a list value as a return from a function
//		* creating an object of a class containing a list,
//			with the initializer list notation as used in cons_cell
//

list::list(const list &init)
{
#if LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_GARBAGE
	this->my_head = init.my_head;	// after "list x = y", x and y have the same head.
	                                // note there was no old "x", since x is being created
	if (LIST_DEBUG) {
		cdebug << "Copied cons cell pointer " << my_head << " from " << &init << " to " << this << endl;
	}
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_COPY
	if (init.my_head == 0) {
		my_head = 0;
		if (LIST_DEBUG) {
			cdebug << "Copied null list (0 pointer) from " << &init << " to " << this << endl;
		}
	} else {
		// In the COPY implementation,
		//   all we need to do is copy the cons cell;
		//   since the cons cell includes a list,
		//   it will trigger the list copy constructor, etc.
		my_head = new cons_cell(init.my_head->i, init.my_head->rest_of_the_list);
		if (LIST_DEBUG) {
			cdebug << "Finished running list::list(const list &init) to copy list from " << &init << " to " << this << ", head = " << my_head->i << endl;
		}
	}
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_REFCOUNT
#endif
}

list &list::operator=(const list &rhs)
{
	if (LIST_DEBUG) {
		cdebug << "[ About to assign from " << &rhs << " to " << this << endl;
	}
#if LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_GARBAGE
	this->my_head = rhs.my_head;	// after "x = y", x and y share cons cells,
	                                //  old cons cells of "x" may be garbage now,
	                                //  or they may be part of some other list... who knows.
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_COPY
	if (this != &rhs) {	// don't do anything if somebody writes "x=x"
		if (my_head != 0) {
			delete my_head;
		}

		if (rhs.my_head == 0) {
			my_head = 0;
		} else {
			my_head = new cons_cell(rhs.my_head->i, rhs.my_head->rest_of_the_list);
			// note that that will copy "rest of the list" thanks to list copy constructor
		}
	}
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_REFCOUNT
#endif

	if (LIST_DEBUG) {
		cdebug << "Just assigned list from " << &rhs << " to " << this
			<< " (except for building the return value...) ]" << endl;
	}
	return *this;	// C++ semantics: assignment returns the object assigned to.
}


//
// list destructor is called when a list is destroyed, including
//	* local variables and parameters disappearing at the end of their scope/function
//	* list objects allocated on the heap with "new" being destroyed with "delete"
//	* objects containing (or subclassed from) list being destroyed for those reasons,
//		including the destruction of a "cons cell" on the heap.
//

list::~list()
{
#if LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_GARBAGE
	// Do nothing ... let the garbage pile up ... google "Sarah Cynthia Sylvia Stout" while it does...
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_COPY
	if (my_head != 0) {
		delete my_head;	// note that this will trigger the cons_cell destructor,
						// and automatically the list destructor for its contained list
	}
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_REFCOUNT
#endif
	if (LIST_DEBUG) {
		cdebug << "Finished running list::~list() on list at " << this << endl;
	}
}

