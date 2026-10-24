# CSE 101: Fundamentals of Computing and Programming in C
## Project Report & Technical Documentation

---

### Project Title:
**Design and Implementation of an Automated Sudoku Solver Using Recursive Backtracking and Performance Analysis**

### Team Information:
- **Course**: CSE 101 - Fundamentals of Computing and Programming in C
- **Class / Section**: Sem: I | CSE : CS : D
- **Student Roll Numbers**:
  - `AP26110090269`
  - `AP26110090265`
  - `AP26110090216`

---

## 1. Executive Summary & Objective

Sudoku is a logic-based combinatorial number-placement puzzle. The objective is to fill a $9 \times 9$ grid with digits from 1 to 9 such that each column, each row, and each of the nine $3 \times 3$ subgrids (also called "boxes" or "blocks") contains all digits from 1 to 9 without repetition.

The objective of this project is three-fold:
1. **Recursion & Backtracking Implementation**: Formulate a robust algorithm in standard C that systematically explores candidate values and backtracks when constraints are violated.
2. **2D Array Manipulation**: Utilize multi-dimensional array constructs (`int grid[9][9]`) for grid representation, state modification, and coordinate mapping.
3. **Empirical Performance Analysis**: Measure, record, and analyze how the execution time, recursive calls, and backtracking counts vary across different puzzle difficulties and clue densities.

---

## 2. Core Concepts Tested

### A. 2D Arrays in C
In C, a two-dimensional array `int grid[9][9]` is stored in **row-major order** as a contiguous block of $9 \times 9 \times \text{sizeof}(int) = 81 \times 4 = 324\text{ bytes}$ of memory.
- Cell $(r, c)$ is mapped to offset $(r \times 9 + c)$.
- We pass `int grid[9][9]` to helper functions by reference (array decaying to a pointer to an array of 9 ints), enabling in-place puzzle modification.

### B. Recursion
Recursion is a programming technique where a function calls itself to solve a smaller sub-instance of the same problem.
- **Base Case**: All 81 cells are filled validly (no unassigned cell remains). The function returns `true`.
- **Recursive Step**: Identify an unassigned cell $(r, c)$, place a valid candidate $k \in \{1, \dots, 9\}$, and call `solve_sudoku(grid)` to solve the remaining $(81 - m)$ cells.

### C. Backtracking
Backtracking is a refined brute-force technique for solving constraint satisfaction problems (CSPs). Instead of generating all $9^{81}$ possible full grids, it builds candidates incrementally:
1. Try placing a candidate number in the current empty cell.
2. If safe, advance to the next empty cell recursively.
3. If no candidate leads to a valid complete solution (a dead end is encountered), **undo** the choice (reset cell back to `0`) and backtrack to the previous decision node to try the next alternative.

---

## 3. Mathematical Constraints & Algorithm Design

### 3.1 Constraint Checking (`is_valid`)
For any assignment of digit $k$ at cell $(row, col)$:
1. **Row Constraint**:
   $$\forall c \in [0, 8]: \text{grid}[row][c] \neq k$$
2. **Column Constraint**:
   $$\forall r \in [0, 8]: \text{grid}[r][col] \neq k$$
3. **$3 \times 3$ Subgrid Constraint**:
   The starting coordinate of the $3 \times 3$ block containing $(row, col)$ is:
   $$\text{start\_row} = row - (row \pmod 3)$$
   $$\text{start\_col} = col - (col \pmod 3)$$
   Constraint:
   $$\forall \Delta r, \Delta c \in \{0, 1, 2\}: \text{grid}[\text{start\_row} + \Delta r][\text{start\_col} + \Delta c] \neq k$$

### 3.2 Pseudocode

```text
Function SolveSudoku(grid):
    If find_empty_cell(grid, row, col) is False:
        Return True   // Base Case: All cells assigned

    For num from 1 to 9:
        If is_valid(grid, row, col, num) is True:
            grid[row][col] = num
            
            If SolveSudoku(grid) is True:
                Return True
                
            grid[row][col] = 0  // Backtrack step

    Return False  // Triggers backtracking to parent call
```

---

## 4. Empirical Performance Analysis (Rubric Requirement)

Using the `<time.h>` library, CPU execution time was measured via `clock()` alongside counters for recursive function calls and backtracking actions.

### 4.1 Benchmark Results Table

| Puzzle Title | Difficulty | Given Clues | Empty Cells | Recursive Calls | Backtrack Steps | CPU Time (ms) |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| **Puzzle 1 (Easy)** | Easy | 30 | 51 | 4,209 | 4,157 | 1.0000 ms |
| **Puzzle 2 (Medium)** | Medium | 23 | 58 | 879,418 | 879,359 | 80.0000 ms |
| **Puzzle 3 (Hard)** | Hard | 17 | 64 | 12,207,092 | 12,207,027 | 1,179.0000 ms |
| **Puzzle 4 (Expert)** | Expert | 23 | 58 | 8,970 | 8,911 | < 0.5000 ms |
| **Puzzle 5 (Extreme)**| Extreme | 21 | 60 | 1,114,772 | 1,114,711 | 86.0000 ms |

### 4.2 Analytical Observations & Conclusions
1. **Search Space vs. Clue Count**:
   - Generally, fewer clues mean more unassigned cells, exponentially enlarging the state space tree. Puzzle 3 (Hard, 17 clues) required over 12 million recursive calls.
2. **Constraint Propagation & Clue Placement**:
   - Puzzle 4 (Expert) has 23 clues, yet solved in fewer than 9,000 calls. This demonstrates that **the geometric distribution of clues matters as much as the total count**. Strategic placement enforces early constraint violations, cutting off massive invalid subtrees near the root.
3. **Worst-Case vs. Practical Complexity**:
   - Theoretical worst-case complexity is $O(9^m)$ where $m$ is the count of empty cells. In practice, constraint pruning eliminates $>99.999\%$ of branches, enabling typical puzzles to be solved within milliseconds.

---

## 5. Viva / Presentation Preparation Questions & Answers

1. **Q: Why is backtracking preferred over simple brute force for Sudoku?**
   - **A**: Pure brute force tests all combinations of unassigned cells ($9^{81} \approx 1.96 \times 10^{77}$ combinations), which is astronomically infeasible. Backtracking evaluates constraints at each step and abandons invalid branches as early as possible (pruning), reducing the search tree to manageable levels.

2. **Q: How does the algorithm avoid infinite recursion?**
   - **A**: The base case triggers when `find_empty_cell()` returns `false` (all 81 cells are validly filled). Furthermore, the number of empty cells strictly decreases with each deeper recursive call, ensuring termination.

3. **Q: What is the significance of resetting `grid[row][col] = 0`?**
   - **A**: That is the fundamental **backtrack step**. If a tentatively chosen number does not lead to a complete valid solution, resetting it to 0 restores the state of the board so subsequent candidate numbers or parent calls can explore alternative paths.

4. **Q: How is execution time measured in C?**
   - **A**: We use the `clock()` function from `<time.h>`. By recording `start = clock()` before solving and `end = clock()` after solving:
     $$\text{Time (ms)} = \frac{(\text{end} - \text{start})}{\text{CLOCKS\_PER\_SEC}} \times 1000.0$$
