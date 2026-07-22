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
    │   ├── CMakeLists.txt # Group build script
    ├── 02-mathematics/
    │   ├── CMakeLists.txt # Group build script
    ├── 03-bit-manipulation/
    │   ├── CMakeLists.txt # Group build script
    ├── 04-recursion/
    │   ├── CMakeLists.txt # Group build script
    ├── 05-arrays/
    │   ├── CMakeLists.txt # Group build script
    ├── 06-searching/
    │   ├── CMakeLists.txt # Group build script
    ├── 07-sorting/
    │   ├── CMakeLists.txt # Group build script
    ├── 08-matrix/
    │   ├── CMakeLists.txt # Group build script
    ├── 09-hashing/
    │   ├── CMakeLists.txt # Group build script
    ├── 10-strings/
    │   ├── CMakeLists.txt # Group build script
    ├── 11-linkedlist/
    │   ├── CMakeLists.txt # Group build script
    ├── 12-stack/
    │   ├── CMakeLists.txt # Group build script
    ├── 13-queue/
    │   ├── CMakeLists.txt # Group build script
    ├── 14-deque/
    │   ├── CMakeLists.txt # Group build script
    ├── 15-tree/
    │   ├── CMakeLists.txt # Group build script
    ├── 16-bst/
    │   ├── CMakeLists.txt # Group build script
    ├── 17-heap/
    │   ├── CMakeLists.txt # Group build script
    ├── 18-graph/
    │   ├── CMakeLists.txt # Group build script
    ├── 19-greedy-programming/
    │   ├── CMakeLists.txt # Group build script
    ├── 20-backtracking/
    │   ├── CMakeLists.txt # Group build script
    ├── 21-dynamic-programming/
    │   ├── CMakeLists.txt # Group build script
    ├── 22-trie/
    │   ├── CMakeLists.txt # Group build script
    ├── 23-segment-tree/
    │   ├── CMakeLists.txt # Group build script
    ├── 24-disjoint-set/
    │   ├── CMakeLists.txt # Group build script

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
>
