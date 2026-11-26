
#include <iostream>
#include <map>
#include <algorithm>
#include <iterator>
using namespace std;

class WarehouseIndex {
    map<long long, long long> intervals;

public:
    void addPackage(long long id) {
        auto next = intervals.upper_bound(id);

        if (next != intervals.begin()) {
            auto prev = std::prev(next);

            if (prev->second >= id) {
                cout << "Duplicate ignored: " << id << "\n";
                return;
            }
        }

        auto prev = intervals.end();

        if (next != intervals.begin())
            prev = std::prev(next);

        bool left = prev != intervals.end()
                    && prev->second + 1 == id;

        bool right = next != intervals.end()
                     && id + 1 == next->first;

        if (left && right) {
            prev->second = next->second;
            intervals.erase(next);
        }
        else if (left) {
            prev->second = id;
        }
        else if (right) {
            long long end = next->second;
            intervals.erase(next);
            intervals[id] = end;
        }
        else {
            intervals[id] = id;
        }

        cout << "Package received: " << id << "\n";
    }

    void displayIntervals() {
        cout << "\nReceived intervals: ";

        if (intervals.empty())
            cout << "None";

        for (auto p : intervals)
            cout << "[" << p.first << ", "
                 << p.second << "] ";

        cout << "\n";
    }

    void displayMissing(long long low, long long high) {
        if (low < 0 || high < low) {
            cout << "Invalid range!\n";
            return;
        }

        long long current = low;

        cout << "\nMissing ranges: ";

        for (auto p : intervals) {
            long long start = p.first;
            long long end = p.second;

            if (end < current)
                continue;

            if (start > high)
                break;

            if (start > current)
                cout << "[" << current << ", "
                     << min(high, start - 1) << "] ";

            if (end >= high) {
                current = high + 1;
                break;
            }

            current = max(current, end + 1);
        }

        if (current <= high)
            cout << "[" << current << ", " << high << "] ";

        if (current > high)
            cout << "None";

        cout << "\n";
    }

    void displayWithinRange(long long low, long long high) {
        if (low < 0 || high < low) {
            cout << "Invalid range!\n";
            return;
        }

        cout << "\nReceived intervals: ";
        bool found = false;

        for (auto p : intervals) {
            if (p.second < low)
                continue;

            if (p.first > high)
                break;

            cout << "[" << max(low, p.first)
                 << ", " << min(high, p.second) << "] ";

            found = true;
        }

        if (!found)
            cout << "None";

        cout << "\n";
    }
};

int main() {
    WarehouseIndex warehouse;
    int choice;

    cout << "WAREHOUSE ARRIVAL INDEX\n";

    while (true) {
        cout << "\n1. Add package ID\n";
        cout << "2. Display intervals\n";
        cout << "3. Display missing ranges\n";
        cout << "4. Display intervals in range\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input!\n";
            break;
        }

        if (choice == 1) {
            long long id;
            cout << "Enter package ID: ";

            if (!(cin >> id) || id < 0 || id > 1000000000LL) {
                cout << "Invalid package ID!\n";
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
                continue;
            }

            warehouse.addPackage(id);
        }
        else if (choice == 2) {
            warehouse.displayIntervals();
        }
        else if (choice == 3 || choice == 4) {
            long long low, high;

            cout << "Enter lower bound: ";
            cin >> low;
            cout << "Enter upper bound: ";
            cin >> high;

            if (!cin) {
                cout << "Invalid input!\n";
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            if (choice == 3)
                warehouse.displayMissing(low, high);
            else
                warehouse.displayWithinRange(low, high);
        }
        else if (choice == 5) {
            cout << "Thank you!\n";
            break;
        }
        else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}
