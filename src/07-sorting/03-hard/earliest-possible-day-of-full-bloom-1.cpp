#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    /*
     * GREEDY SORTING BASED ON INDEPENDENT GROWTH CYCLES ENGINE
     * ---------------------------------------------------------
     * The objective is to determine the earliest possible day when all seeds have 
     * reached full bloom. For each seed i, we are given plantTime[i] (the number of 
     * days required to plant the seed) and growTime[i] (the number of days it grows 
     * independently before blooming).
     *
     * Algorithmic & Greedy Strategy:
     * - Planting is sequential: Only one seed can be actively planted at any given time.
     * - Growth is concurrent: Multiple seeds can grow simultaneously without interfering 
     *   with one another once they are placed in the ground.
     * - To minimize the total timeline, we must maximize concurrent execution. A seed with 
     *   a long growth duration should be planted as early as possible so that its growth period 
     *   can overlap with the planting time of other seeds.
     * - Therefore, our optimal greedy rule is to sort the seeds in descending order of 
     *   their `growTime`.
     *
     * Execution Protocol:
     * - Step 1: Pair each seed's plantTime and growTime together, then sort the collection 
     *   in descending order based strictly on growTime.
     * - Step 2: Track two running variables:
     *     - `currentPlantDay`: An accumulator tracking the current day on the timeline as 
     *       we plant seeds one after another.
     *     - `maxBloomDay`: The global maximum day tracking the furthest bloom date recorded.
     * - Step 3: Iterate through the sorted seeds. Advance `currentPlantDay` by the seed's 
     *   `plantTime`. Calculate its expected bloom date (`currentPlantDay + growTime`) and 
     *   greedily maximize `maxBloomDay`.
     */
    int earliestFullBloom(vector<int>& plantTime, vector<int>& growTime) {
        size_t n = plantTime.size();
        
        // Step 1: Bind corresponding time metrics into a single sortable collection
        vector<pair<int, int>> seeds(n);
        for (size_t i = 0; i < n; ++i) {
            seeds[i] = {growTime[i], plantTime[i]};
        }

        // Sort seeds greedily in descending order based on their independent growth durations
        sort(seeds.begin(), seeds.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.first > b.first;
        });

        int currentPlantDay = 0;
        int maxBloomDay = 0;

        // Step 2 & 3: Simulate sequential planting and track concurrent growing timelines
        for (size_t i = 0; i < n; ++i) {
            int currentGrowTime = seeds[i].first;
            int currentPlantTime = seeds[i].second;

            // Advance the timeline by the sequential cost of planting the current seed
            currentPlantDay += currentPlantTime;

            // The bloom day for this specific seed is the day planting completes plus its growth time
            int currentSeedBloomDay = currentPlantDay + currentGrowTime;

            // Greedily capture the furthest timeline extension boundary encountered
            maxBloomDay = max(maxBloomDay, currentSeedBloomDay);
        }

        return maxBloomDay;
    }
};

/*
 * COMPLEXITY PROFILE METRICS:
 * ----------------------------
 * TIME COMPLEXITY: O(N log N) - Where N represents the total number of seeds. Building the 
 *                  paired array takes O(N) time. The sorting sequence based on growth durations 
 *                  dominates the execution timeline, consuming O(N log N) steps. The simulation 
 *                  pass tracks timeline limits linearly in O(N).
 *
 * SPACE COMPLEXITY: O(N) Auxiliary - The optimization strategy allocates a paired structural 
 *                   vector proportional to the total size footprint of the input datasets.
 */
int main() {
    int n;
    cout << "Enter the total number of seed items to process: ";
    if (!(cin >> n) || n <= 0) {
        cout << "Invalid parameter. Seed count must be a positive integer value." << endl;
        return 1;
    }

    vector<int> plantTime(static_cast<size_t>(n));
    cout << "Enter the sequential plantTime values separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> plantTime[static_cast<size_t>(i)];
        if (plantTime[static_cast<size_t>(i)] < 0) {
            cout << "Constraint Error: Planting timelines cannot hold negative values." << endl;
            return 1;
        }
    }

    vector<int> growTime(static_cast<size_t>(n));
    cout << "Enter the independent growTime values separated by spaces:\n";
    for (int i = 0; i < n; ++i) {
        cin >> growTime[static_cast<size_t>(i)];
        if (growTime[static_cast<size_t>(i)] < 0) {
            cout << "Constraint Error: Growth timelines cannot hold negative values." << endl;
            return 1;
        }
    }

    Solution solver;
    cout << "\nExecuting greedy sorting optimization based on independent growth cycles..." << endl;
    int earliestBloomDay = solver.earliestFullBloom(plantTime, growTime);

    cout << "Earliest possible day of absolute full cluster bloom completion: " << earliestBloomDay << endl;

    return 0;
}