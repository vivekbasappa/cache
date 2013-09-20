/**
 * filename: Cache.h
 *
 *
 * Original Author: Vivek Basappa Reddy
 * Date: 23 September, 2013
 *
 * Description: Simple Cache mechanism to handle LRU
 *
 * $Id$
 */
#ifndef CACHE_H
#define CACHE_H

#include <map>
#include <list>
#include <ostream>

// cache will work for any key value types
/**
 * @file Cache.h
 *      This describes the implimenation of simple cacheing mechanism which
 * supports LRU following operations are supported in this class
 *  creation and initialization
 *      we'd like to specify the cache size upon its creation- that is the
 *      maximal number of kesy it stores
 *  lookup
 *      we'd like to ask the cache for a key and get the value, or an
 *      indication that this key doesn't exist in the cache
 *  insertion
 *      we'd like to add keys to the cache. If the key already exist in the
 *      cache, its value will be updated with the latest value. If there's
 *      no key in the cache, it will be added to the cache. If the cache
 *      is full, the LRU key will be removed to make space for the new key
 */
template <typename key_type, typename value_type>
class Cache
{
private:
    // as for C++ standards, typename is mandatory before the qualified,
    // dependent name which referes to a type
    //typedef typename list<key_type>::iterator list_iter;
    typedef typename std::list<key_type>::iterator list_iter;
    struct cached_value
    {
            cached_value(value_type value_, list_iter cache_i_)
                : value(value_), cache_i(cache_i_)
            {

            }
            value_type value;
            list_iter cache_i;
    };

    struct cache_statistics {
        private:
            int found_hits;
            int found;
            int removed;
            int miss;

        public:
            friend class Cache;
            cache_statistics():found_hits(0),
                    found(0), miss(0), removed(0) {}
    };

    typedef typename std::map<key_type, cached_value>::iterator table_iter;
    unsigned maxsize;                      //!< maximum size given to the
                                           //!< cache at creation
    std::list<key_type>              lru_list;  //!< queue manage MRU data
    std::map<key_type, cached_value> table;     //!< cache storage
    cache_statistics            stats;     //!< statistical information

public:
    /**
     *@ctor
     */
     Cache(int size);

    /**
     *@dtor
     */
    ~Cache() {}

    /**
     * @brief
     * The key is looked up in the table which has efficient lookups, if the
     * key wasn't found, we simply return 0. If the key was found,
     * we have to splice the accessed key out of its place in the queue
     * and place it in front - since now this key is the most recently used.
     * Then, we return the value of the key
     *
     * @param[in] key
     *  key which need to be found.
     *
     * @return
     *  value saved in the cache else 0.
     */
    value_type* find(const key_type& key);

    /**
     * @brief
     *  First we look for the key in the table. Note that the local,
     *  cache's function find() is used here, becouse if we do find the
     *  element we want to marked as MRU.
     *      If the key was found, we just update its value and return.
     *  More interesting is what happens when the key is not found - here
     *  insertion takes place. After adding the key to the cache, we check
     *  if the cache size is exceeded. If it is, we throw out the key that's
     *  in the back of lru_list which is, if you recall, the LRU key - just
     *  what we need!
     *
     * @param[in] key
     *      key to find the bucket.
     *
     *  @param[in] value
     *      value that need to be saved .
     *
     * @return
     *      none.
     */
    void insert(const key_type& key, const value_type& value);

    /**
     * @brief
     * display the statistics of the cache usage
     *      found_hits
     *      found
     *      removed
     *
     * @param[in]
     *      none
     *
     *  @return
     *      none
     *
     */
    void statistics(std::ostream& out) const;
};

template <typename key_type, typename value_type>
Cache<key_type, value_type>::Cache(int size) :maxsize(size)  { }

template <typename key_type, typename value_type>
value_type*
Cache<key_type, value_type>::find(const key_type& key)
{
    table_iter ti = table.find(key);

    stats.found_hits++;

    if (ti == table.end())
        return 0;

    stats.found++;
    // if we found the element then mark it as MRU
    list_iter li = ti->second.cache_i;
    lru_list.splice(lru_list.begin(), lru_list, li);
    return &(ti->second.value);
}

template <typename key_type, typename value_type>
void
Cache<key_type, value_type>::insert(const key_type& key,
                                    const value_type& value)
{
    value_type* valptr = find(key);

    if (valptr) {
        *valptr = value;
    } else  {
        // add value to the list
        lru_list.push_front(key);
        cached_value cv(value, lru_list.begin());
        table.insert(make_pair(key, cv));

        if (lru_list.size() > maxsize) {
            key_type lru_key = lru_list.back();
            table.erase(lru_key);
            lru_list.pop_back();
            stats.removed++;
        }
    }
    return;
}

template <typename key_type, typename value_type>
void
Cache<key_type, value_type>::statistics(std::ostream& out) const
{
     out << "cache found hits:" << stats.found_hits << std::endl;
     out << "cache found     :" << stats.found << std::endl;
     out << "cache removed   :" << stats.removed << std::endl;
     out << "cache missed    :" << stats.found_hits - stats.found  << std::endl;

     return;
}

#endif // CHACHE_H

