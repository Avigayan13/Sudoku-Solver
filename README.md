# 🧩 9x9 Sudoku Solver in C (Recursive Backtracking & Empirical Benchmarking)

![Language: C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Standard: C99 / C11](https://img.shields.io/badge/Standard-C99%20%2F%20C11-blue?style=for-the-badge)
![Compiler: GCC](https://img.shields.io/badge/Compiler-GCC%2016.1%20%7C%20MinGW-41454a?style=for-the-badge&logo=gnu)
![License: MIT](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

---

## 📌 Project Overview
A modular, high-performance $9 \times 9$ Sudoku Solver written in standard C. The solver combines **recursive backtracking** with empirical complexity analysis to examine how solving time, recursive depth, and backtrack counts scale across varying difficulty levels (from 36-clue easy puzzles to 17-clue near-worst-case configurations).

Developed as part of **CSE 101: Fundamentals of Computing and Programming in C** (First Semester).


---

## 🎯 Key Highlights & Skills Tested
- **Recursion & Backtracking**: Systematic candidate exploration with automatic pruning of invalid branches.
- **2D Arrays**: Multi-dimensional indexing, coordinate subgrid mapping (`row - row % 3`), and memory manipulation in C.
- **Empirical Performance Benchmarking**: Precise CPU cycle timing using `<time.h>` and `clock()` to satisfy the analysis requirement:
  > *"Measure how the solving time changes across different puzzles."*
- **File I/O**: Parses custom puzzle files with whitespace and formatted dot notation.
- **Clean Code Standard**: Zero warnings with `-Wall -Wextra` flags.

---

## 🚀 Quick Start: Compilation & Execution

### 1. Compile with GCC
```bash
gcc -Wall -Wextra -O2 main.c -o sudoku_solver.exe
```

### 2. Run the Application
```bash
# On Windows:
.\sudoku_solver.exe

# On Linux / macOS:
./sudoku_solver.exe
```

---

## 📊 Benchmark & Performance Analysis

Running Option 4 (**Automated Comparative Analysis**) executes the solver across 5 standard puzzles:

```
===================================================================================
                      AUTOMATED COMPARATIVE ANALYSIS REPORT                         
===================================================================================
Puzzle Name                  | Difficulty | Clues  | Recursion Calls | Backtracks   | Time (ms)   
-----------------------------------------------------------------------------------
Easy Puzzle (36 Clues)       | Easy       | 30     | 4,209           | 4,157        | 1.0000      
Medium Puzzle (28 Clues)     | Medium     | 23     | 879,418         | 879,359      | 80.0000     
Hard Puzzle (24 Clues)       | Hard       | 17     | 12,207,092      | 12,207,027   | 1179.0000   
Expert / AI Escargot Style   | Expert     | 23     | 8,970           | 8,911        | < 0.5000    
Near-Worst Case Puzzle       | Extreme    | 21     | 1,114,772       | 1,114,711    | 86.0000     
-----------------------------------------------------------------------------------
TOTAL / AGGREGATE            | -          | -      | 14,214,461      | 14,214,165   | 1346.0000   
===================================================================================
```

### 🔍 Key Engineering Findings
1. **Search Tree Growth**: Fewer starting clues generally increase empty cells and widen the state-space tree.
2. **Constraint Pruning Efficiency**: Difficulty is not determined by clue count alone; geometric placement determines how early contradictory branches are cut off.
3. **Exponential vs. Practical**: While worst-case brute-force complexity is $O(9^{81})$, backtracking pruning reduces execution time to milliseconds.

---

## 📂 Repository Structure

```text
├── main.c                  # Core C engine (Solver, UI, Benchmark, File I/O)
├── PROJECT_REPORT.md       # Comprehensive documentation & Viva Q&A
├── README.md               # GitHub repository overview & usage
├── LICENSE                 # MIT Open-Source License
├── .gitignore              # Ignores executables, object files, and IDE caches
└── puzzles/                # Sample test inputs
    ├── easy.txt
    ├── medium.txt
    ├── hard.txt
    └── extreme.txt
```

---

## 📄 License
This project is licensed under the [MIT License](LICENSE).
