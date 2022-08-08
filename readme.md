This program will iterate through your roblox processes, then check to see if the main window for one of the processes is open, and if there are no main windows, it kills them all.  

Warning: This will not work if you have multiple instances open, and one dies but not another.  For me to add support for that, I would need to check the memory usage of each process, and only kill processes with no windows that have over 100mb (example) of memory used.
