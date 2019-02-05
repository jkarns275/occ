The Program
-----------@
                                                                        
This program is pretty simple: it performs a breadth  first search on the
specified graph. The graph can be any ascii text file  that has an '@'
character, which is going to be the starting location, and a '!' character
which will be the destination.                                            
                                         
				       !
The graph should be supplied to the program through standard in, e.g.
    cat remarks.md | ./tbfs


Issues
-----------

- Breadth first search is slow (and very slow on certain maps), but my A*
version of this program is too big.
- Lots of extra whitespace may be needed if you want to feed in an arbitrary
file.
- It fails to compile on GCC sometimes (according to a friend of mine, I've
not encountered this myself.
- The program could be obfuscated much further.
