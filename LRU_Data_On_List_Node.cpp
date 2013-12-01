#include <cstdio>
#include <cstdlib>
#include <list>
#include <utility>
#include <new>
#include <tr1/unordered_map>
using namespace std;
using namespace tr1;

template <typename K,
          typename V,
          typename Hash = hash<K>,
          typename Pred = equal_to<K> >
class Lru {
 private:
  typedef typename list<pair<K, V> >::iterator ListIterator;
  typedef typename unordered_map<K, ListIterator, Hash, Pred>::iterator
      HashIterator;
  list<pair<K, V> > list_;
  unordered_map<K, ListIterator, Hash, Pred> hash_;
  const size_t max_size_;

 public:
  Lru(size_t max_size) : max_size_(max_size) {}

  virtual ~Lru() {}

  void Insert(const K& key, const V& value) {
    if (list_.size() >= max_size_) {
      ListIterator tail = list_.end(); --tail;
      hash_.erase(tail->first);
      *tail = pair<K, V>(key, value);
      list_.splice(list_.begin(), list_, tail);
    } else {
      list_.push_front(pair<K, V>(key, value));
    }
    hash_[key] = list_.begin();
  }

  V* Get(const K& key) {
    HashIterator it = hash_.find(key);
    if (it != hash_.end()) {
      list_.splice(list_.begin(), list_, it->second);
      return &(list_.begin()->second);
    } else {
      return 0;
    }
  }

  void PrintStatus() {
    for (ListIterator it = list_.begin(); it != list_.end(); ++it) {
      printf("%d->", it->first);
    }
    printf("NULL");
  }
};

const int MAX_SIZE = 10000;
int seq[MAX_SIZE];

int main() {
  Lru<int, long long> lru(MAX_SIZE / 2);
  for (int i = 0; i < MAX_SIZE; ++i) {
    seq[i] = rand() % MAX_SIZE;
  }
  for (int i = 0; i < MAX_SIZE; ++i) {
    int key = seq[i];
    long long* value_ptr = lru.Get(key);
    printf("key = %d ", key);
    if (value_ptr == 0) {
      // read the data from file
      printf("[MISS] read the data from file ");
      long long v = key;
      lru.Insert(key, v);
    } else {
      // the data is in the cache
      printf("[HIT]  In the cache, value = %lld ", *value_ptr);
    }
    lru.PrintStatus();
    printf("\n");
  }
  return 0;
}
