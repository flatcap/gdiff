In the first, there is no other way. Imagine a recursive walk of a binary tree: 

void Walk(NODE *Node)
{
	// Do whatever we came here to do...

	if(Node->Left)				// Shall we recurse left?
		Walk(Node->Left);

	if(Node->Right)				// Shall we recurse right?
		Walk(Node->Right);
}

So as we walk the tree, we start by making calls that lead us left, left, left, left... until the bottom. We then wander
about in the tree, visiting every combination of left, left, right, right, left... and so on, until we finish our walk by
doing a whole bunch of returns from our final visit that was right, right, right, right... 

Every step left or right entails opening a new stack frame, copying the argument into the stack frame, performing
the call and returning. On some jobs with a lot of navigation but not a lot of per node processing, this overhead
can mount up. But look at this powerful idiom, known as tail recursion elimination: 

void 
Walk (NODE * Node)
{
Label:						// Do whatever we came here to do...
	if (Node->Left)				// Shall we recurse left?
		Walk (Node->Left);

	if (Node->Right)			// Shall we recurse right?
	{
		Node = Node->Right;		// Tail recursion elimination used for efficiency
		goto Label;
	}
}

We use the stack to keep track of where we are to the left, but after we have explored left, the right walk doesn't
need to keep position. So we eliminate a full 50% of the call and return overhead. This can make the difference
between staying in C and having to add an assembly language module to the source. For an awesome example of
this kind of thing, see Duff's Device in Stroustrup's The C++ Programming Language. 
