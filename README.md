<pre>
Python dependencies:
   numpy
   pygame

Usage:
   ./maze_generator rows cols [--stack] > maze.txt
         -> entry and exit are always (1,1) and (n,n)
         -> without --stack the maximum maze size it can generate is about 500 500
            with --stack it can go over 10000 10000 but the generated mazes dont look as good. 
            Too lazy to get the exact behaviour.
   
   python visualize_map.py maze.txt [path.txt] [resolution (default 800)]
         -> output is in maze.png
   
</pre>
