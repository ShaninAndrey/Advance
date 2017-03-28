#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

// Struct of user query
struct query {
    int st, fn, len, vers, pos;

    // Query "Get" value by number Pos in version Vers
    query(const int &Vers, const int &Pos) {
        vers = Vers;
        pos = Pos;
    }

    // Query "Move" in version Vers subarray [St; St + Len - 1] to [Fn; Fn + Len - 1]
    query(const int &Vers, const int &St, const int &Fn, const int &Len) {
        vers = Vers;
        st = St;
        fn = Fn;
        len = Len;
        pos = -1;
    }
};

// Struct of version
struct version {
    // Array current version
    std::vector<int> arr;

    version() { }
    version(const std::vector<int> &new_arr) {
        arr = new_arr;
    }

    /*
    * Create new version from current version
    * movement by subarray [St; St + Len - 1] to positions [Fn; Fn + Len - 1]
    */
    version* move(const int &st, const int &fn, const int &len) {
        /*
        * If start position is equal to finish position
        * result is current version
        */
        if (st == fn) {
            return new version(std::vector<int>(arr));
        }

        // Create new array
        std::vector<int> ans;

        /*
        * For better understanding
        * our array is cut into 4 pieces
        * 1, 4 - unmovable
        * 2, 3 - are swapping
        * that's why result array is 1+3+2+4 parts of current
        * where numbers is subarrays, plus - concatenation
        */

        // Copies the first unmovable numbers
        ans.insert(ans.end(), arr.begin(), arr.begin() + std::min(st, fn) - 1);

        // If subarray move to right
        if (st < fn) {
            // Copies numbers which are blocked of movable
            ans.insert(ans.end(), arr.begin() + st + len - 1, arr.begin() + fn + len - 1);
            // Copies movable numbers
            ans.insert(ans.end(), arr.begin() + st - 1, arr.begin() + st + len - 1);
        } else {
            // Copies movable numbers
            ans.insert(ans.end(), arr.begin() + st - 1, arr.begin() + st + len - 1);
            // Copies numbers which are blocked of movable
            ans.insert(ans.end(), arr.begin() + fn - 1, arr.begin() + st - 1);
        }

        // Copies last unmovable numbers
        ans.insert(ans.end(), arr.begin() + std::max(st, fn) + len - 1, arr.end());

        //return new version based array ans
        return new version(ans);
    }
};

//finish program with readed incorrect data
void finish(const int args) {
    std::cout << "Incorrect data";
    if (args > 1) {
        fclose(stdin);
    }
    if (args > 2) {
        fclose(stdout);
    }
    exit(0);
}

// read input data
void read(std::istream &input, std::vector<query> &queries, std::vector<int> &arr, const int args) {
    int n, m;
    try {
        input >> n >> m;
        if (n <= 0 || n > 100000 || m > 100000 || m < 0) throw 1;
    } catch(...) {
        finish(args);
    }
    for (int i = 0; i < n; ++i) {
        int val;
        try {
            input >> val;
            if (abs(val) > 1000000000) throw 1;
        } catch(...) {
            finish(args);
        }
        arr.push_back(val);
    }
    for (int i = 1; i <= m; ++i) {
        std::string qr;
        int vers, st, fn, len, pos;
        input >> qr;
        if (qr != "move" && qr != "get") {
            finish(args);
        }
        if (qr[0] == 'm') {
            try {
                input >> vers >> st >> fn >> len;
                if (vers < 0 || vers > m ||
                    st <= 0 || st > n ||
                    fn <= 0 || fn > n ||
                    len <= 0 || st + len > n + 1 || fn + len > n + 1) {

                    throw 1;
                }
            } catch(...) {
                finish(args);
            }
            queries.push_back(query(vers, st, fn, len));
        } else {
            try {
                input >> vers >> pos;
                if (vers < 0 || vers > m || pos <= 0 || pos > n) throw 1;
            } catch(...) {
                finish(args);
            }
            queries.push_back(query(vers, pos));
        }
    }
}

// solve the problem. Write answer
std::vector<int> solve(const std::vector<query> &queries,
                       const std::vector<int> &arr,
                       const int args) {

    std::vector<version*> versions(1, new version(arr));
    std::vector<int> ans;

    for (int i = 0; i < queries.size(); ++i) {
        if (versions[queries[i].vers]->arr.size() != arr.size()) {
            finish(args);
        }
        if (queries[i].pos == -1) {
            versions.push_back(versions[queries[i].vers]->move(queries[i].st,
                                                               queries[i].fn,
                                                               queries[i].len));
        } else {
            versions.push_back(new version());
            ans.push_back(versions[queries[i].vers]->arr[queries[i].pos - 1]);
        }
    }
    return ans;
}

// write answer
void write(std::ostream &output, const std::vector<int> &ans) {
    for (int i = 0; i < ans.size(); ++i) {
        output << ans[i] << "\n";
    }
}

int main(int args, char* argv[]) {
    if (args > 1) {
        freopen(argv[1], "r", stdin); //input file
    }
    if (args > 2) {
        freopen(argv[2], "w", stdout); // output file
    }
    std::vector<version*> versions; // array of versions
    std::vector<query> queries; // array of queries
    std::vector<int> arr; // initial array
    std::vector<int> ans; // answer
    read(std::cin, queries, arr, args); // read data
    ans = solve(queries, arr, args); // solve problem. Write answer in ans
    write(std::cout, ans); // write answer

    if (args > 1) {
        fclose(stdin);
    }
    if (args > 2) {
        fclose(stdout);
    }
    return 0;
}
