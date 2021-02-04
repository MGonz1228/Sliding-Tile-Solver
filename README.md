# Sliding-Tile-Solver
Command line application that uses the A* search algorithm to solve sliding tile puzzles of any given size.

## Usage
### Generate a random puzzle (optional)
* #### Define puzzle size
  * Edit line 6: ```#define N 8``` of generate_puzzle.c to your desired size.
* #### Compile and run with desired puzzle difficulty
  * ```gcc generate_puzzle.c -o generate && ./generate N```
  * Note: replace N with the number of random steps from the goal state to scramble 
    * ./generate 10 will be an easy puzzle; the generator only moves 10 times from the goal state
  
### Solve an input puzzle
* #### Define puzzle size
  * Edit line 4: ```#define N 8``` of sliding_tile.c to match the size of your puzzle.
* ### Compile and run with puzzle as input
  * ```gcc sliding_tile.c -o solve && ./solve [PUZZLE]```
  * Note: replace [PUZZLE] with the initial state of your puzzle, on one line
    * an 8x8 puzzle may look something like: 1  2  3  4  5  6  7  8  9 10 0 ... 61 62 55 63

## Known Issues
* The solver output ```No solution found in 20000 iterations```
  * The solver gives up after a while. Edit line 277 ```if(iter %20000 == 0)``` and change 20000 to some larger number.
  
## Possible Future Functionality
* GUI!
