#include<bits/stdc++.h>
using namespace std;

int N; // Puzzle size (N x N)
vector<vector<int>> goal; // Goal state

vector<pair<int, int>> directions = {{-1,0},{1,0},{0,-1},{0,1}};
vector<char> dirChar = {'U','D','L','R'};

struct Puzzle {
    vector<vector<int>> board;
    int x, y; // position of 0 (blank)
    int g, h, f;
    string path;

    Puzzle(vector<vector<int>> b, int g = 0, string path = "") : board(b), g(g), path(path) {
        h = calcHeuristic();
        f = g + h;
        findBlank();
    }

    void findBlank() {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (board[i][j] == 0) {
                    x = i;
                    y = j;
                    return;
                }
    }

    int calcHeuristic() const {
        // Manhattan distance heuristic
        int dist = 0;
        unordered_map<int, pair<int, int>> goalPos;
        
        // Create a map of value to position in goal state
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                goalPos[goal[i][j]] = {i, j};

        // Calculate Manhattan distance for each tile
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] != 0) {
                    int gi = goalPos.at(board[i][j]).first;
                    int gj = goalPos.at(board[i][j]).second;
                    dist += abs(i - gi) + abs(j - gj);
                }
            }
        }

        return dist;
    }

    bool isGoal() const {
        return board == goal;
    }

    string boardToString() const {
        string s;
        for (const auto& row : board)
            for (int val : row)
                s += to_string(val) + ",";
        return s;
    }

    // bool operator>(const Puzzle& other) const {
    //     return f > other.f;
    // }

    void printState() const {
        cout << "\nStep #" << g << " (f=" << f << ", g=" << g << ", h=" << h << ")\n";
        cout << "Path: " << (path.empty() ? "Start" : path) << "\n";
        cout << "Board:\n";
        
        // Determine the width needed for proper alignment
        int maxDigits = to_string(N*N-1).length();
        
        for (const auto& row : board) {
            for (int val : row) {
                if (val == 0) {
                    cout << string(maxDigits, ' ') << " ";
                } else {
                    cout << setw(maxDigits) << val << " ";
                }
            }
            cout << "\n";
        }
    }
};

// Function to count inversions for solvability check
int countInversions(const vector<vector<int>>& board) {
    vector<int> linear;
    for (const auto& row : board)
        for (int val : row)
            if (val != 0) linear.push_back(val);
    
    int inv = 0;
    for (int i = 0; i < linear.size(); ++i)
        for (int j = i + 1; j < linear.size(); ++j)
            if (linear[i] > linear[j]) inv++;
    
    return inv;
}

// If N is odd, then puzzle instance is solvable if number of inversions is even in the input state.
// If N is even, puzzle instance is solvable if 
// the blank is on an even row counting from the bottom (second-last, fourth-last, etc.) and number of inversions is odd.
// the blank is on an odd row counting from the bottom (last, third-last, fifth-last, etc.) and number of inversions is even.
// For all other cases, the puzzle instance is not solvable.
bool isSolvable(const vector<vector<int>>& board) {
    int inv = countInversions(board);
    
    if (N % 2 == 1) {
        // Odd grid: solvable if inversions even
        return inv % 2 == 0;
    } else {
        // Even grid: find row of blank (from bottom)
        int blankRowFromBottom;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (board[i][j] == 0)
                    blankRowFromBottom = N - i;

        return (blankRowFromBottom % 2 == 0) ? (inv % 2 == 1) : (inv % 2 == 0);
    }
}

struct PuzzleComparator {
    bool operator()(const Puzzle& p1, const Puzzle& p2) const {
        return p1.f > p2.f;  // To prioritize lower 'f' values
    }
};


void solveAStar(const vector<vector<int>>& startBoard) {
    if (!isSolvable(startBoard) || !isSolvable(goal)) {
        cout << "\n This puzzle is not solvable!\n";
        cout << "The given initial configuration cannot be transformed into the goal configuration.\n";
        return;
    }

    //priority_queue<Puzzle, vector<Puzzle>, greater<Puzzle>> pq;
    // Use it in priority_queue
    priority_queue<Puzzle, vector<Puzzle>, PuzzleComparator> pq;
    set<string> visited;

    Puzzle start(startBoard);
    pq.push(start);
    visited.insert(start.boardToString());

    int stepCount = 0;
    const int maxSteps = 100000; // Safety limit to prevent infinite loops

    cout << "\nStarting A* Search...\n";
    cout << "Initial state:\n";
    start.printState();

    while (!pq.empty()) {
        Puzzle current = pq.top();
        pq.pop();

        if (current.isGoal()) {
            cout << "\n Puzzle Solved Successfully!\n";
            cout << "Total moves: " << current.g << "\n";
            cout << "Solution path: " << current.path << "\n";
            return;
        }

        if (++stepCount > maxSteps) {
            cout << "\n Warning: Search halted after " << maxSteps << " steps.\n";
            cout << "The puzzle might be too complex or the heuristic might not be strong enough.\n";
            return;
        }

        cout << "\nExploring state (Step " << stepCount << "):";
        current.printState();

        for (int i = 0; i < 4; ++i) {
            int newX = current.x + directions[i].first;
            int newY = current.y + directions[i].second;

            if (newX >= 0 && newX < N && newY >= 0 && newY < N) {
                auto newBoard = current.board;
                swap(newBoard[current.x][current.y], newBoard[newX][newY]);
                Puzzle next(newBoard, current.g + 1, current.path + dirChar[i]);

                if (visited.find(next.boardToString()) == visited.end()) {
                    pq.push(next);
                    visited.insert(next.boardToString());
                    
                    // Optional: Print info about this new state
                    cout << "  Generated move: " << dirChar[i] 
                         << " (f=" << next.f << ", g=" << next.g << ", h=" << next.h << ")\n";
                }
            }
        }
    }

    cout << "\nNo solution found within the explored state space.\n";
}

int main() {
    cout << "========================================\n";
    cout << "        N x N Puzzle Solver\n";
    cout << "========================================\n\n";

    cout << "Enter the puzzle size N (e.g., 3 for 3x3): ";
    cin >> N;

    if (N < 2) {
        cout << "Error: Puzzle size must be at least 2x2.\n";
        return 1;
    }

    vector<vector<int>> startBoard(N, vector<int>(N));
    goal.assign(N, vector<int>(N));

    cout << "\nEnter the initial board (use 0 for blank):\n";
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> startBoard[i][j];

    cout << "\nEnter the goal board:\n";
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> goal[i][j];

    // Validate inputs
    set<int> startValues, goalValues;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            startValues.insert(startBoard[i][j]);
            goalValues.insert(goal[i][j]);
        }
    }

    // Check if both boards contain the same set of numbers
    if (startValues != goalValues) {
        cout << "\nError: The initial and goal boards must contain the same set of numbers.\n";
        return 1;
    }

    solveAStar(startBoard);

    return 0;
}




// Your C++ program is an implementation of the **A* (A-star) algorithm*\* to solve the **N×N sliding puzzle problem** (e.g., 3×3 for 8-puzzle, 4×4 for 15-puzzle). Here's a step-by-step breakdown of what each section does:

// ---

// ### 🔹 1. **Global Variables and Setup**

// ```cpp
// int N; // Size of the puzzle (e.g., 3 for 3x3)
// vector<vector<int>> goal; // Goal state configuration
// ```

// * `N` is input by the user.
// * `goal` holds the target board configuration to reach.

// ---

// ### 🔹 2. **Movement Directions**

// ```cpp
// vector<pair<int, int>> directions = {{-1,0},{1,0},{0,-1},{0,1}};
// vector<char> dirChar = {'U','D','L','R'};
// ```

// * Represents directions: **Up, Down, Left, Right** for moving the blank (0 tile).

// ---

// ### 🔹 3. **Puzzle Structure**

// ```cpp
// struct Puzzle { ... };
// ```

// * Holds the current board state, cost values (`g`, `h`, `f`), position of blank (`x`, `y`), and path taken as a string of directions.

// **Main functions:**

// * `findBlank()`: Finds and stores position of the blank tile (0).
// * `calcHeuristic()`: Calculates **Manhattan Distance** between current state and goal.
// * `isGoal()`: Checks if current state matches the goal.
// * `boardToString()`: Serializes the board into a string for uniqueness checks.
// * `printState()`: Neatly prints the current board state and info.

// ---

// ### 🔹 4. **Inversion Count for Solvability**

// ```cpp
// int countInversions(const vector<vector<int>>& board) { ... }
// bool isSolvable(...) { ... }
// ```

// * Sliding puzzles are only solvable if inversion rules match:

//   * For odd N: solvable if number of inversions is even.
//   * For even N: blank row position from bottom and inversions together determine solvability.

// ---

// ### 🔹 5. **A* Solver*\*

// ```cpp
// void solveAStar(...) { ... }
// ```

// **Key steps inside:**

// * **Check solvability:** Rejects unsolvable puzzles early.
// * **Priority Queue:** Uses min-heap ordered by `f = g + h`.
// * **Visited Set:** Avoids reprocessing same state.
// * **Main loop:**

//   * Pick state with lowest `f`.
//   * Check for goal.
//   * Try all 4 directions of movement.
//   * Calculate new board state, heuristic, and add to queue if new.

// ---

// ### 🔹 6. **Main Function**

// ```cpp
// int main() { ... }
// ```

// **Workflow:**

// * Ask for puzzle size `N`.
// * Read initial and goal board values.
// * Validate input values (must be same numbers in both).
// * Call `solveAStar()`.

// ---

// ### ✅ Summary of Concepts Used

// | Concept            | Purpose                                    |
// | ------------------ | ------------------------------------------ |
// | A\* Algorithm      | Finds optimal path from start to goal      |
// | Manhattan Distance | Heuristic estimating distance to goal      |
// | Priority Queue     | Expands lowest `f = g + h` cost node first |
// | Set (Visited)      | Avoids revisiting states                   |
// | Inversion Count    | Validates puzzle solvability               |

// Would you like me to provide a visual or example trace for a 3x3 or 4x4 board?
