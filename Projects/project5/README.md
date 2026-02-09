1. If we didn't lock the header, than the counts would be constantly overwritten. When we were dumping the log into stdout, the counts would be read all wrong and the data wouldn't be found.

2. If the record wasn't locked, than during writing it would be visible that the records were being overwritten as the next process starts. Though this doesn't change the code from running, it helps unclutter the log data and keeps processes from stepping on each others toes while writing. 
