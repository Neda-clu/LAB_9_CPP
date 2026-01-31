#ifndef GROUP_STATS_HPP
#define GROUP_STATS_HPP

#include <vector>
#include <map>
#include <utility>

template < typename Key, typename Val, typename Elem >
struct GroupInfo {
    Key key;
    long gsize = 0;
    Elem minEl;
    Elem maxEl;
    Val sum;
    double avg = 0.0;
};

template < typename Container, typename KeyFunc, typename ValueFunc >
auto groupStats(const Container & arr, KeyFunc keyFunc, ValueFunc valueFunc) {
    using T = typename Container::value_type;
    using Key = decltype(keyFunc(std::declval<T>()));
    using Val = decltype(valueFunc(std::declval<T>()));

    struct GroupData {
        long gsize = 0;
        T minEl{};
        T maxEl{};
        Val sum{};
        bool initialized = false;
    };

    std::map<Key, GroupData> groups;
    std::vector<GroupInfo<Key, Val, T>> result;

    for (const T& x : arr) { /*T-тип контейнера ,описанный в using выше.*/
        Key key = keyFunc(x);
        Val val = valueFunc(x);
        GroupData &data = groups[key]; /*GroupData-структура,которая хранится в groups*/

        if (!data.initialized) {
            data.minEl = x;
            data.maxEl = x;
            data.sum = val;
            data.gsize = 1;
            data.initialized = true;
        } else {
            if (x > data.maxEl) data.maxEl = x;
            if (x < data.minEl) data.minEl = x;
            data.sum += val;
            data.gsize++;
        }
    }

    result.reserve(groups.size());

    for (const std::pair<const Key,GroupData> & : groups) { /*pair-шаблонный класс из из utity> */
        GroupInfo<Key, Val, T> group;
        group.key = key;
        group.gsize = data.gsize;
        group.minEl = data.minEl;
        group.maxEl = data.maxEl;
        group.sum = data.sum;
        group.avg = (data.gsize != 0) ? double(data.sum) / data.gsize : 0.0;
        result.push_back(group);
    }

    return result;
}

#endif // GROUP_STATS_HPP
