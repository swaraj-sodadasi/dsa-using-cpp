#include <iostream>
#include <cmath>

using namespace std;

/*
CORE LOGIC (INFORMATION THEORY AND COMBINATORIAL DIMENSIONALITY):
- The problem asks for the minimum number of pigs required to find the single poisonous bucket out of `buckets` total buckets within a given time limit.
- Let `minutesToTest` be the total available testing time and `minutesToDie` be the time it takes for a pig to die after drinking poison.
- The maximum number of testing rounds a single pig can participate in is calculated as:
  $$ rounds = \frac{minutesToTest}{minutesToDie} $$
- Each round a pig drinks from a dynamic selection of buckets. If it dies, we know the poison is in that batch. If it survives, we move to the next round. 
- A critical insight is that a pig has exactly `rounds + 1` possible states at the end of the entire testing process:
  - It could die in round 1.
  - It could die in round 2.
  - ...
  - It could die in the final round.
  - It could survive all rounds.
- If we have `p` pigs, each pig acts as an independent informational dimension. The total number of unique states (or combinations) we can distinctively measure across `p` pigs is:
  $$ \text{Total States} = (rounds + 1)^p $$
- To successfully locate the single poisonous bucket, the number of unique informational states must be greater than or equal to the total number of buckets:
  $$ (rounds + 1)^p \ge buckets $$
- Solving for `p` using logarithms gives:
  $$ p \ge \frac{\log(\text{buckets})}{\log(rounds + 1)} $$
- We take the ceiling of this value (`std::ceil`) to get the minimum whole number of pigs required.

TIME COMPLEXITY: O(1) - The answer is computed directly in constant time using standard logarithmic operations.
SPACE COMPLEXITY: O(1) - Runs completely in-place using scalar variables.
*/
int poorPigs(int buckets, int minutesToDie, int minutesToTest) {
    if (buckets <= 1) return 0; // If there is only 1 bucket, it must be the poisonous one; no testing needed.

    // Calculate how many rounds of tests can be executed sequentially
    int rounds = minutesToTest / minutesToDie;
    int base = rounds + 1;

    // Use logarithms to solve the equation: base^p >= buckets
    // p = ceil( log(buckets) / log(base) )
    double pigsRequired = log(buckets) / log(base);
    
    // Using a tiny epsilon value helps prevent floating-point precision issues from dropping the ceiling lower
    return static_cast<int>(ceil(pigsRequired - 1e-9));
}

int main() {
    int buckets, minutesToDie, minutesToTest;
    
    cout << "Enter the total number of buckets: ";
    cin >> buckets;
    cout << "Enter the minutes to die after drinking: ";
    cin >> minutesToDie;
    cout << "Enter the total minutes to test: ";
    cin >> minutesToTest;

    if (buckets < 1 || minutesToDie <= 0 || minutesToTest <= 0) {
        cout << "Error: Invalid inputs. Buckets must be >= 1, and times must be positive values." << endl;
        return 1;
    }

    int result = poorPigs(buckets, minutesToDie, minutesToTest);
    cout << "\nThe minimum number of pigs required to find the poisonous bucket is: " << result << endl;

    return 0;
}