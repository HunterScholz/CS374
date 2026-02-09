What could go wrong if we didn't lock the header?

What could go wrong if we didn't lock the record? (This is harder because in this program, as described, nothing will go wrong if we left the record locks out. But consider what might happen if there were a second external program that was reading records at the same time that your program was writing them. What might that second program see that wasn't right if the record lock wasn't in place?)
