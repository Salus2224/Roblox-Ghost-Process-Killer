This program iterates through all windows processes, finds dead Roblox processes, and kills them.

To find dead Roblox processes, the program: 
1) Finds all processes that are using the "RobloxPlayerBeta.exe" executable file.
2) Checks to see which processes are using more than 100mb (there are meaningful background processes that use around 10mb of memory).
3) Checks to see if the process has a main window with the title "Roblox"  (to ensure that it is a background process).

Step 3 of this process would need to be changed if the title of the window was changed by an external program, but that could be easily done.
