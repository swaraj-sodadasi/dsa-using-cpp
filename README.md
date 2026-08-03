# DSA C++ Practice Repository

Welcome to my professional Data Structures and Algorithms (DSA) practice workspace. This repository contains solutions to high-quality DSA problems optimized for placement preparation and competitive programming, built cleanly using **Modern C++** and managed with **CMake**.

---

## 📂 Repository Structure

The project is split into 24 core categories. Each category contains 30 distinct, high-quality problems divided by difficulty levels (Easy, Medium, Hard) with each and every folder and file is configured with CMake.

```text
dsa-cpp-practice/
├── .gitignore             # Filters out compilation files
├── CMakeLists.txt         # Global configuration file
├── LICENSE                # MIT License
├── list-of-problems.txt   # Lists all the problems in this repository
├── README.md              # Project documentation
└── src/
    ├── 01-introduction/
    ├── 02-mathematics/
    ├── 03-bit-manipulation/
    ├── 04-recursion/
    ├── 05-arrays/
    ├── 06-searching/
    ├── 07-sorting/
    ├── 08-matrix/
    ├── 09-hashing/
    ├── 10-strings/
    ├── 11-linkedlist/
    ├── 12-stack/
    ├── 13-queue/
    ├── 14-deque/
    ├── 15-tree/
    ├── 16-bst/
    ├── 17-heap/
    ├── 18-graph/
    ├── 19-greedy-programming/
    ├── 20-backtracking/
    ├── 21-dynamic-programming/
    ├── 22-trie/
    ├── 23-segment-tree/
    ├── 24-disjoint-set/
```

## 🚀 How to Build and Run Problems

This repository uses a **build directory isolation** approach via CMake to keep source files completely clean from compiled artifacts.

## Prerequisites or Dependencies
Ensure you have a modern C++ compiler (supporting C++17 or higher) and CMake installed on your machine.

### 1. Configure and Build
Open your terminal inside the root directory (`dsa-cpp-practice/`) and run:

```bash
# Create and enter the build folder
mkdir build && cd build

# Generate build configuration files
cmake ..

# Compile the entire project
cmake --build .
```

### 2. Execute an Individual Problem

Once compiled, you can execute individual problem files directly from your terminal inside the `build/` folder:

* **Linux / macOS:**
```bash
./src/01_introduction/01_two_sum

```

* **Windows:**
```bash
.\src\01_introduction\01_two_sum.exe
```

### 3. 📝 How to Add a New Problem

1. Drop your new `.cpp` problem file inside its corresponding category folder under `src/`.
2. Open the `CMakeLists.txt` file inside that specific category folder.
3. Append a new executable rule at the bottom:
```cmake
add_executable(problem_name problem_name.cpp)

```
4. Re-run `cmake --build .` inside your `build/` directory to compile it instantly.

<!-- Automated CI/CD & Hosting Badges -->

##  🛠️ Project Resources
> * Read the interactive code layouts: **[Live Doxygen Documentation Site](https://swaraj-sodadasi.github.io/dsa-using-cpp/)**
> * Monitor the cloud deployment status: **[GitHub Actions Workflow](https://github.com/swaraj-sodadasi/dsa-using-cpp/actions/workflows/deploy-docs.yml)**
