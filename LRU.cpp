#include <cstdio>
#include <cstdlib>
#include <list>
#include <utility>
#include <new>
#include <unordered_map>
using namespace std;

template <typename K,
          typename V,
          typename Hash = hash<K>,
          typename Pred = equal_to<K> >
class Lru {
 private:
  typedef typename list<pair<K, V*> >::iterator ListIterator;
  typedef typename unordered_map<K, ListIterator, Hash, Pred>::iterator
      HashIterator;
  V* cache_;
  list<pair<K, V*> > list_;
  unordered_map<K, ListIterator, Hash, Pred> hash_;
  const size_t max_size_;

 public:
  Lru(size_t max_size) : max_size_(max_size) {
    cache_ = static_cast<V*>(malloc(max_size_ * sizeof(V)));
  }

  virtual ~Lru() {
    for (size_t i = 0; i < list_.size(); ++i) {
      (cache_ + i)->~V();
    }
    free(cache_);
  }

  void Insert(const K& key, const V& value) {
    V* value_ptr = 0;
    if (list_.size() >= max_size_) {
      hash_.erase(list_.back().first);
      value_ptr = list_.back().second;
      value_ptr->~V();
      list_.pop_back();
    } else {
      value_ptr = cache_ + (list_.size());
    }
    list_.push_front(pair<K, V*>(key, new (value_ptr) V(value)));
    hash_[key] = list_.begin();
  }

  V* Get(const K& key) {
    HashIterator it = hash_.find(key);
    if (it != hash_.end()) {
      V* value_ptr = it->second->second;
      list_.erase(it->second);
      list_.push_front(pair<K, V*>(key, value_ptr));
      hash_[key] = list_.begin();
      return value_ptr;
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

int main() {
  Lru<int, long long> lru(3);
  int seq[20];
  for (int i = 0; i < 20; ++i) {
    seq[i] = rand() % 5;
  }
  for (int i = 0; i < 20; ++i) {
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
