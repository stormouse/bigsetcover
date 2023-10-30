#include <algorithm>
#include <iostream>
#include <bitset>
#include <memory>
#include <unordered_map>
#include <vector>
#include <list>
#include <optional>
#include <dbs/dbs.h>

class SetCover {
private:
    struct ItemSet {
        uint32_t key;
        uint64_t importance;
        const dbs_lib::dbs& value;
        
        ItemSet(uint32_t key, const dbs_lib::dbs& value) : key{key}, value{value} {};

        void updateImportance(const dbs_lib::dbs& target) {
            importance = 0;
            auto n = target & value;
            while (n.first() != dbs_lib::dbs::npos) { 
                n = n.flip(n.first());
                importance++;
            }
        }
        
        const dbs_lib::dbs kZero{};
    };

public:
    std::optional<dbs_lib::dbs> getSetCover(const dbs_lib::dbs& target, const std::unordered_map<uint32_t, dbs_lib::dbs>& candidates) {
        auto selected = dbs_lib::dbs{};
        auto itemSets = buildItemSets(candidates);
        auto state = target;

        while (state != kZero) {
            // update importance
            std::for_each(itemSets.begin(), itemSets.end(), [&state](auto& x) { x.updateImportance(state); });

            // find the best candidate
            auto best = std::max_element(itemSets.begin(), itemSets.end(), cmpByImportance);

            // check for no solution
            if (best->importance == 0) {
                return {};
            }

            // update
            selected = selected.set(best->key);
            state = (state ^ best->value) & state;
        }

        return selected;
    }

private:
    std::list<ItemSet> buildItemSets(const std::unordered_map<uint32_t, dbs_lib::dbs>& candidates) const {
        auto result = std::list<ItemSet>{};
        for (auto& p : candidates) {
            result.push_back(ItemSet{p.first, p.second});
        }
        return result;
    }

    static bool cmpByImportance(const ItemSet& a, const ItemSet& b)
    {
        return a.importance < b.importance;
    }

    const dbs_lib::dbs kZero{};
};

constexpr int NumKeys = 500;
constexpr int NumValues = 80;

int main(int argc, char** argv) {
    auto algo = SetCover{};
    auto candidates = std::unordered_map<uint32_t, dbs_lib::dbs>{};

    for (int i = 0; i < NumKeys; i++) {
        auto v = dbs_lib::dbs{};
        for (int j = 0; j < NumValues; j++) {
            if ((i - j) % 7 == 0) {
                v = v.set(j);
            }
        }
        candidates.insert({i, v});
    }

    std::cout << "Data prepared.\n";

    auto target = dbs_lib::dbs{};
    for (int i = 0; i < 8; i++) {
        target = target.set(i);
    }

    std::cout << "Target: " << target << std::endl;
    std::cout << "Target Check: " << target.first() << std::endl;
    std::cout << "Set cover: " << *algo.getSetCover(target, candidates) << std::endl;

    return 0;
}
