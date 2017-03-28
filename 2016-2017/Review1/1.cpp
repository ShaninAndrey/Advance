#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

void read(std::istream &input,
          std::vector<int> &people,
          int &passengers,
          int &buses,
          int &size_bus) {

    input >> buses >> size_bus >> passengers;
    people.resize(passengers);
    for (int cPeople = 0; cPeople < passengers; ++cPeople) {
        input >> people[cPeople];
    }
}

void fill_dynamic(std::vector<std::vector<std::vector<int> > > &dynamic,
                  const std::vector<int> &people,
                  const int &passengers,
                  const int &buses,
                  const int &size_bus) {

    dynamic.resize(passengers);
    for (int i = 0; i < passengers; ++i) {
        dynamic[i].resize(buses);
        for (int q = 0; q < buses; ++q) {
            dynamic[i][q].resize(size_bus + 1);
        }
    }
    for (int cBus = 0; cBus < buses; ++cBus) {
        for (int cSize = 0; cSize <= size_bus; ++cSize) {
            if (cBus > 0 || cSize >= people[0]) {
                dynamic[0][cBus][cSize] = 1;
            }
        }
    }

    for (int cPeople = 1; cPeople < passengers; ++cPeople) {
        for (int cBus = 0; cBus < buses; ++cBus) {
            for (int cSize = 0; cSize <= size_bus; ++cSize) {
                dynamic[cPeople][cBus][cSize] = dynamic[cPeople - 1][cBus][cSize];
                if (cBus > 0 || cSize >= people[cPeople]) {
                    if (cSize >= people[cPeople]) {
                        dynamic[cPeople][cBus][cSize] =
                        std::max(dynamic[cPeople][cBus][cSize],
                                 dynamic[cPeople - 1][cBus][cSize - people[cPeople]] + 1);
                    } else {
                        dynamic[cPeople][cBus][cSize] =
                        std::max(dynamic[cPeople][cBus][cSize],
                                 dynamic[cPeople - 1][cBus - 1][size_bus - people[cPeople]] + 1);
                    }
                }
            }
        }
    }
}

int solve(const std::vector<int> &people,
          const int &passengers,
          const int &buses,
          const int &size_bus) {

    std::vector<std::vector<std::vector<int> > > dynamic;
    fill_dynamic(dynamic, people, passengers, buses, size_bus);
    return dynamic[passengers - 1][buses - 1][size_bus];
}

void write(std::ostream &output,
           const int ans) {

    output << ans;
}

int main()
{
    int passengers, buses, size_bus;
    std::vector<int> people;
    read(std::cin, people, passengers, buses, size_bus);
    const int answer = solve(people, passengers, buses, size_bus);
    write(std::cout, answer);
}
