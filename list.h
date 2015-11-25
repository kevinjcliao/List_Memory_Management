#if ! defined LIST_LAB_H
#define LIST_LAB_H 1

//
// Linked list class using new & delete for memory management,
//	with three options for implementation of management of elements ("cons cells"):
//	1. Lists can share elements, but no effort is made to free up heap storage for elements
//		(not suitable for long-running programs)
//	2. Lists do not share elements (requiring us to copy lots of elements),
//		but then all elements of a list can be freed up when the list is destroyed
//	3. Lists share elements, and each element keeps count of how many times it has been referenced,
//		and thus we decrement this "reference count" when a list disappears,
//		and free up storage for any element that is in no lists.
//
// Note that there are no mutator operations for lists,
//  and there is no way to get a reference to the integer in the list,
//  so there should be no way to tell the implementations apart except for performance
//  or calling the "how_many_cons_cells_are_there" method.
//
// Control which implementation is used via #define LIST_IMPLEMENTATION_STRATEGY to 1, 2, or 3
//

#define LIST_IMPLEMENTATION_STRATEGY_GARBAGE  1
#define LIST_IMPLEMENTATION_STRATEGY_COPY     2
#define LIST_IMPLEMENTATION_STRATEGY_REFCOUNT 3

// UNCOMMENT _ONLY_ONE_ OF THE THREE LINES BELOW TO SET THE MEMORY MANAGEMENT STRATEGY
#define LIST_IMPLEMENTATION_STRATEGY LIST_IMPLEMENTATION_STRATEGY_GARBAGE
//#define LIST_IMPLEMENTATION_STRATEGY LIST_IMPLEMENTATION_STRATEGY_COPY
// #define LIST_IMPLEMENTATION_STRATEGY LIST_IMPLEMENTATION_STRATEGY_REFCOUNT


class list {
public:	// Nothing here should depend on LIST_IMPLEMENTATION_STRATEGY (why?)
    list();								// a list is either empty ...
    list(int h, const list &r);			// ... or an item followed by a (sub)list

    list(const list &init);
    list &operator=(const list &rhs);
    ~list();

    bool empty() const;		// list().empty() === True | list(h,r).empty() === False
    int head() const;		// list().head() === UNDEF | list(h,r).head() === h
    list rest() const;		// list().rest() === UNDEF | list(h,r).rest() === r
    
    static int how_many_allocated_cons_cells_are_there();
    static int how_many_cons_cells_were_ever_created();
    static const  bool LIST_DEBUG = true;

private:	// Things in here, and in list.cc, may depend on LIST_IMPLEMENTATION_STRATEGY
    struct cons_cell;	// This just declares that there will be a type "cons cell"

#if LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_GARBAGE
    cons_cell *my_head;
    // Representation function/invariant:
    //	 * list() has null my_head pointer
    //	 * otherwise, my_head
    //		points to a cons_cell (on the heap) with
    //		the head integer and
    //		the rest of the list
    // A single cons cell may be "in" several lists,
    //   so we can't delete the cons cells for a list
    //   just because that list is going away.
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_COPY
    cons_cell *my_head;
    // Representation function/invariant:
    //	 * list() has null my_head pointer
    //	 * otherwise, my_head
    //		points to a cons_cell (on the heap) with
    //		the head integer and
    //		the rest of the list
    // EVERY CONS CELL has EXACTLY ONE pointer to it
    //   in the "copy each list fully" representation
#elif LIST_IMPLEMENTATION_STRATEGY == LIST_IMPLEMENTATION_STRATEGY_REFCOUNT
#else

    This is an error: no valid LIST_IMPLEMENTATION_STRATEGY is defined in list.h

#endif

};

#endif

