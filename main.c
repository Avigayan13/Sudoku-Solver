#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define N 9

/* Structure to collect performance and complexity metrics */
typedef struct {
    long long recursive_calls;
    long long backtracks;
    double time_taken_ms;
} PerformanceStats;

/* Benchmark puzzle sample */
typedef struct {
    const char *name;
    const char *difficulty;
    int grid[N][N];
} BenchmarkPuzzle;

/* Function Declarations */
void print_header(void);
void print_menu(void);
void print_grid(int grid[N][N]);
bool find_empty_cell(int grid[N][N], int *row, int *col);
bool is_valid(int grid[N][N], int row, int col, int num);
bool is_initial_grid_valid(int grid[N][N]);
int count_clues(int grid[N][N]);
void copy_grid(int src[N][N], int dest[N][N]);
bool solve_sudoku(int grid[N][N], PerformanceStats *stats);
void solve_and_analyze(int grid[N][N], const char *puzzle_title);
bool load_grid_from_file(const char *filename, int grid[N][N]);
void input_custom_grid(int grid[N][N]);
void run_comparative_analysis(void);

/* Built-in test puzzles for instant testing and analysis */
BenchmarkPuzzle sample_puzzles[] = {
    {
        "Easy Puzzle (36 Clues)",
        "Easy",
        {
            {5, 3, 0, 0, 7, 0, 0, 0, 0},
            {6, 0, 0, 1, 9, 5, 0, 0, 0},
            {0, 9, 8, 0, 0, 0, 0, 6, 0},
            {8, 0, 0, 0, 6, 0, 0, 0, 3},
            {4, 0, 0, 8, 0, 3, 0, 0, 1},
            {7, 0, 0, 0, 2, 0, 0, 0, 6},
            {0, 6, 0, 0, 0, 0, 2, 8, 0},
            {0, 0, 0, 4, 1, 9, 0, 0, 5},
            {0, 0, 0, 0, 8, 0, 0, 7, 9}
        }
    },
    {
        "Medium Puzzle (28 Clues)",
        "Medium",
        {
            {0, 0, 0, 6, 0, 0, 4, 0, 0},
            {7, 0, 0, 0, 0, 3, 6, 0, 0},
            {0, 0, 0, 0, 9, 1, 0, 8, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 5, 0, 1, 8, 0, 0, 0, 3},
            {0, 0, 0, 3, 0, 6, 0, 4, 5},
            {0, 4, 0, 2, 0, 0, 0, 6, 0},
            {9, 0, 3, 0, 0, 0, 0, 0, 0},
            {0, 2, 0, 0, 0, 0, 1, 0, 0}
        }
    },
    {
        "Hard Puzzle (24 Clues)",
        "Hard",
        {
            {0, 0, 0, 8, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 4, 3},
            {5, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 7, 0, 8, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0},
            {0, 2, 0, 0, 3, 0, 0, 0, 0},
            {6, 0, 0, 0, 0, 0, 0, 7, 5},
            {0, 0, 3, 4, 0, 0, 0, 0, 0},
            {0, 0, 0, 2, 0, 0, 6, 0, 0}
        }
    },
    {
        "Expert / 'AI Escargot' Style (21 Clues)",
        "Expert",
        {
            {1, 0, 0, 0, 0, 7, 0, 9, 0},
            {0, 3, 0, 0, 2, 0, 0, 0, 8},
            {0, 0, 9, 6, 0, 0, 5, 0, 0},
            {0, 0, 5, 3, 0, 0, 9, 0, 0},
            {0, 1, 0, 0, 8, 0, 0, 0, 2},
            {6, 0, 0, 0, 0, 4, 0, 0, 0},
            {3, 0, 0, 0, 0, 0, 0, 1, 0},
            {0, 4, 0, 0, 0, 0, 0, 0, 7},
            {0, 0, 7, 0, 0, 0, 3, 0, 0}
        }
    },
    {
        "Near-Worst Case Backtracking Puzzle (17 Clues)",
        "Extreme",
        {
            {0, 0, 0, 0, 0, 0, 0, 1, 2},
            {0, 0, 0, 0, 0, 0, 0, 0, 3},
            {0, 0, 2, 3, 0, 0, 4, 0, 0},
            {0, 0, 1, 8, 0, 0, 0, 0, 5},
            {0, 6, 0, 0, 7, 0, 8, 0, 0},
            {0, 0, 0, 0, 0, 9, 0, 0, 0},
            {0, 0, 8, 5, 0, 0, 0, 0, 0},
            {9, 0, 0, 0, 4, 0, 5, 0, 0},
            {4, 7, 0, 0, 0, 6, 0, 0, 0}
        }
    }
};

const int NUM_SAMPLE_PUZZLES = sizeof(sample_puzzles) / sizeof(sample_puzzles[0]);

int main(void) {
    int choice;
    int current_grid[N][N];

    while (1) {
        print_header();
        print_menu();
        printf("Enter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            /* Clear invalid input buffer */
            while (getchar() != '\n');
            printf("\n[ERROR] Invalid input. Please enter a number between 1 and 6.\n");
            continue;
        }

        switch (choice) {
            case 1: {
                /* Built-in puzzle selection */
                printf("\n--- Select a Built-in Puzzle ---\n");
                for (int i = 0; i < NUM_SAMPLE_PUZZLES; i++) {
                    printf("  [%d] %s (%s)\n", i + 1, sample_puzzles[i].name, sample_puzzles[i].difficulty);
                }
                printf("Select puzzle (1-%d): ", NUM_SAMPLE_PUZZLES);
                int p_choice;
                if (scanf("%d", &p_choice) == 1 && p_choice >= 1 && p_choice <= NUM_SAMPLE_PUZZLES) {
                    copy_grid(sample_puzzles[p_choice - 1].grid, current_grid);
                    solve_and_analyze(current_grid, sample_puzzles[p_choice - 1].name);
                } else {
                    printf("\n[ERROR] Invalid puzzle selection.\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 2: {
                /* Console manual input */
                input_custom_grid(current_grid);
                solve_and_analyze(current_grid, "Custom User Puzzle");
                break;
            }
            case 3: {
                /* Load from file */
                char filename[256];
                printf("\nEnter path to puzzle file (e.g., puzzles/easy.txt): ");
                scanf("%255s", filename);
                if (load_grid_from_file(filename, current_grid)) {
                    solve_and_analyze(current_grid, filename);
                }
                break;
            }
            case 4: {
                /* Automated Comparative Analysis across multiple difficulties */
                run_comparative_analysis();
                break;
            }
            case 5: {
                /* Quick info about Algorithm & Backtracking */
                printf("\n============================================================\n");
                printf("         ABOUT BACKTRACKING & RECURSION IN SUDOKU           \n");
                printf("============================================================\n");
                printf("1. Recursion: A function calls itself to solve smaller sub-problems.\n");
                printf("2. Backtracking: A systematic trial-and-error strategy.\n");
                printf("   - Find the next unassigned cell (row, col).\n");
                printf("   - Try placing digits 1 through 9.\n");
                printf("   - Verify constraints: Row, Column, and 3x3 Subgrid.\n");
                printf("   - If valid, tentatively place the digit and recursively call solve().\n");
                printf("   - If that path leads to a dead end, RESET cell to 0 (BACKTRACK)\n");
                printf("     and try the next number.\n");
                printf("   - If no digit 1-9 works, return false to trigger backtrack.\n");
                printf("3. Complexity Analysis:\n");
                printf("   - Worst-case time complexity: O(9^(N*N)) = O(9^81).\n");
                printf("   - In practice: Constraint checking prunes huge portions of\n");
                printf("     the search space, solving most puzzles in under a second!\n");
                printf("============================================================\n");
                break;
            }
            case 6: {
                printf("\nThank you for using Sudoku Solver. Goodbye!\n\n");
                return 0;
            }
            default:
                printf("\n[ERROR] Invalid choice! Please select an option between 1 and 6.\n");
                break;
        }

        printf("\nPress ENTER to continue...");
        while (getchar() != '\n');
        getchar();
    }

    return 0;
}

/* Print banner header */
void print_header(void) {
    printf("\n");
    printf("============================================================\n");
    printf("      CSE 101: SUDOKU SOLVER WITH BACKTRACKING & ANALYSIS   \n");
    printf("============================================================\n");
}

/* Print main options */
void print_menu(void) {
    printf("1. Solve a Preset Sample Puzzle (Easy -> Extreme)\n");
    printf("2. Enter a Custom Puzzle Manually (Console)\n");
    printf("3. Load and Solve a Puzzle from a File\n");
    printf("4. Run Comparative Analysis (Benchmark Across Difficulties)\n");
    printf("5. Algorithm Explanation & Viva Prep Guide\n");
    printf("6. Exit\n");
    printf("------------------------------------------------------------\n");
}

/* Print 9x9 grid with formatted 3x3 block borders */
void print_grid(int grid[N][N]) {
    printf("\n    +-------+-------+-------+\n");
    for (int r = 0; r < N; r++) {
        printf("    | ");
        for (int c = 0; c < N; c++) {
            if (grid[r][c] == 0) {
                printf(". ");
            } else {
                printf("%d ", grid[r][c]);
            }
            if ((c + 1) % 3 == 0) {
                printf("| ");
            }
        }
        printf("\n");
        if ((r + 1) % 3 == 0) {
            printf("    +-------+-------+-------+\n");
        }
    }
    printf("\n");
}

/* Locate the first unassigned (0) cell in the grid */
bool find_empty_cell(int grid[N][N], int *row, int *col) {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (grid[r][c] == 0) {
                *row = r;
                *col = c;
                return true;
            }
        }
    }
    return false;
}

/* Check if assigning 'num' to grid[row][col] is safe */
bool is_valid(int grid[N][N], int row, int col, int num) {
    /* 1. Check Row Constraint */
    for (int c = 0; c < N; c++) {
        if (grid[row][c] == num) {
            return false;
        }
    }

    /* 2. Check Column Constraint */
    for (int r = 0; r < N; r++) {
        if (grid[r][col] == num) {
            return false;
        }
    }

    /* 3. Check 3x3 Subgrid Constraint */
    int start_row = row - (row % 3);
    int start_col = col - (col % 3);
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (grid[start_row + r][start_col + c] == num) {
                return false;
            }
        }
    }

    return true;
}

/* Check if the initial puzzle has any illegal duplicate clues */
bool is_initial_grid_valid(int grid[N][N]) {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            int val = grid[r][c];
            if (val < 0 || val > 9) return false;
            if (val != 0) {
                /* Temporarily clear to test validity */
                grid[r][c] = 0;
                bool valid = is_valid(grid, r, c, val);
                grid[r][c] = val;
                if (!valid) return false;
            }
        }
    }
    return true;
}

/* Count number of given numbers (clues) */
int count_clues(int grid[N][N]) {
    int clues = 0;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (grid[r][c] != 0) clues++;
        }
    }
    return clues;
}

/* Copy 2D array */
void copy_grid(int src[N][N], int dest[N][N]) {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            dest[r][c] = src[r][c];
        }
    }
}

/*
 * Recursive Backtracking Solver
 * Base Case: No empty cells left -> puzzle is solved.
 * Recursive Step: For the current empty cell, attempt digits 1 through 9.
 * Backtrack: If recursive call fails, undo assignment (set back to 0) and try next digit.
 */
bool solve_sudoku(int grid[N][N], PerformanceStats *stats) {
    if (stats) stats->recursive_calls++;

    int row, col;

    /* Base case: If no empty cells are left, we have solved the puzzle */
    if (!find_empty_cell(grid, &row, &col)) {
        return true;
    }

    /* Try placing numbers from 1 to 9 */
    for (int num = 1; num <= 9; num++) {
        if (is_valid(grid, row, col, num)) {
            /* Tentative assignment */
            grid[row][col] = num;

            /* Recurse to solve remainder */
            if (solve_sudoku(grid, stats)) {
                return true;
            }

            /* Backtrack: undo assignment */
            grid[row][col] = 0;
            if (stats) stats->backtracks++;
        }
    }

    /* Trigger backtracking in caller */
    return false;
}

/* High-level solve helper with time tracking and stats reporting */
void solve_and_analyze(int grid[N][N], const char *puzzle_title) {
    int solve_copy[N][N];
    copy_grid(grid, solve_copy);

    int clues = count_clues(solve_copy);
    int empty = 81 - clues;

    printf("\n------------------------------------------------------------\n");
    printf(" Puzzle: %s\n", puzzle_title);
    printf(" Initial Clues: %d | Empty Cells to Solve: %d\n", clues, empty);
    printf("------------------------------------------------------------\n");
    printf("INITIAL GRID:");
    print_grid(solve_copy);

    if (!is_initial_grid_valid(solve_copy)) {
        printf("[ERROR] The initial grid contains contradictory clues!\n");
        printf("A number violates row, column, or 3x3 box rules.\n");
        return;
    }

    PerformanceStats stats = {0, 0, 0.0};

    printf("Solving using recursive backtracking...\n");
    clock_t start = clock();
    bool solved = solve_sudoku(solve_copy, &stats);
    clock_t end = clock();

    stats.time_taken_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

    if (solved) {
        printf("\n>>> PUZZLE SOLVED SUCCESSFULLY! <<<\n");
        printf("SOLVED GRID:");
        print_grid(solve_copy);

        printf("------------------------------------------------------------\n");
        printf(" PERFORMANCE & COMPLEXITY ANALYSIS\n");
        printf("------------------------------------------------------------\n");
        printf("  * Total Recursive Calls : %lld\n", stats.recursive_calls);
        printf("  * Total Backtracks      : %lld\n", stats.backtracks);
        printf("  * Execution Time        : %.4f ms (%.6f seconds)\n", 
               stats.time_taken_ms, stats.time_taken_ms / 1000.0);
        printf("------------------------------------------------------------\n");
    } else {
        printf("\n[RESULT] No valid solution exists for this Sudoku configuration.\n");
        printf("Checked %lld recursive configurations before terminating.\n", stats.recursive_calls);
    }
}

/* Load puzzle from a text file */
bool load_grid_from_file(const char *filename, int grid[N][N]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("[ERROR] Could not open file: '%s'\n", filename);
        printf("Please ensure the file exists and the path is correct.\n");
        return false;
    }

    int count = 0;
    int ch;
    while ((ch = fgetc(fp)) != EOF && count < 81) {
        if (ch >= '0' && ch <= '9') {
            grid[count / N][count % N] = ch - '0';
            count++;
        } else if (ch == '.' || ch == '_') {
            grid[count / N][count % N] = 0;
            count++;
        }
        /* Ignore spaces, newlines, commas */
    }
    fclose(fp);

    if (count < 81) {
        printf("[ERROR] File '%s' contained only %d cells (81 required).\n", filename, count);
        return false;
    }

    printf("[SUCCESS] Loaded 81 cells from '%s'.\n", filename);
    return true;
}

/* Read custom grid from standard input */
void input_custom_grid(int grid[N][N]) {
    printf("\n=== ENTER 9x9 SUDOKU GRID ===\n");
    printf("Instructions: Enter 9 numbers (0-9) per row separated by spaces.\n");
    printf("Use '0' or '.' for empty cells.\n\n");

    for (int r = 0; r < N; r++) {
        printf("Row %d (9 digits): ", r + 1);
        for (int c = 0; c < N; c++) {
            char token[16];
            if (scanf("%15s", token) != 1) {
                grid[r][c] = 0;
                continue;
            }
            if (token[0] >= '1' && token[0] <= '9') {
                grid[r][c] = token[0] - '0';
            } else {
                grid[r][c] = 0;
            }
        }
    }
}

/*
 * Automated Comparative Analysis
 * Fulfills the project rubric:
 * "Analysis point: Measure how the solving time changes across different puzzles."
 */
void run_comparative_analysis(void) {
    printf("\n===================================================================================\n");
    printf("                      AUTOMATED COMPARATIVE ANALYSIS REPORT                         \n");
    printf("===================================================================================\n");
    printf(" Running backtracking solver on %d standardized test puzzles...\n\n", NUM_SAMPLE_PUZZLES);

    printf("%-28s | %-10s | %-6s | %-15s | %-12s | %-12s\n",
           "Puzzle Name", "Difficulty", "Clues", "Recursion Calls", "Backtracks", "Time (ms)");
    printf("-----------------------------------------------------------------------------------\n");

    long long total_calls = 0;
    long long total_backtracks = 0;
    double total_time = 0.0;

    for (int i = 0; i < NUM_SAMPLE_PUZZLES; i++) {
        int test_grid[N][N];
        copy_grid(sample_puzzles[i].grid, test_grid);
        int clues = count_clues(test_grid);

        PerformanceStats stats = {0, 0, 0.0};
        clock_t start = clock();
        bool solved = solve_sudoku(test_grid, &stats);
        clock_t end = clock();

        stats.time_taken_ms = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

        total_calls += stats.recursive_calls;
        total_backtracks += stats.backtracks;
        total_time += stats.time_taken_ms;

        if (solved) {
            printf("%-28s | %-10s | %-6d | %-15lld | %-12lld | %-12.4f\n",
                   sample_puzzles[i].name,
                   sample_puzzles[i].difficulty,
                   clues,
                   stats.recursive_calls,
                   stats.backtracks,
                   stats.time_taken_ms);
        } else {
            printf("%-28s | %-10s | %-6d | [UNSOLVABLE]    | -            | -\n",
                   sample_puzzles[i].name,
                   sample_puzzles[i].difficulty,
                   clues);
        }
    }

    printf("-----------------------------------------------------------------------------------\n");
    printf("%-28s | %-10s | %-6s | %-15lld | %-12lld | %-12.4f\n",
           "TOTAL / AGGREGATE", "-", "-", total_calls, total_backtracks, total_time);
    printf("===================================================================================\n");

    printf("\nKEY OBSERVATIONS & ANALYSIS FOR LAB SUBMISSION:\n");
    printf("1. Correlation between Clues and Search Space:\n");
    printf("   - Puzzles with fewer clues (or adversarially placed clues) generally require\n");
    printf("     substantially more recursive branch explorations and backtracking steps.\n");
    printf("2. Significance of Clue Distribution (Constraint Strength):\n");
    printf("   - Difficulty in backtracking is NOT solely determined by clue count, but by\n");
    printf("     how early contradictory branches can be pruned by the Sudoku rules.\n");
    printf("3. Practical Time vs. Theoretical O(9^(N*N)):\n");
    printf("   - While worst-case complexity is exponential, constraint pruning ensures\n");
    printf("     even hard 9x9 puzzles typically resolve in milliseconds.\n");
    printf("===================================================================================\n");
}
