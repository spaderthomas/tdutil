struct Hasher {
	static int hash(const int key) {
		return key;
	}
} hasher;

#define MAP_INITIAL_CAPACITY 8
template<typename Key, typename Value>
struct tdmap {
	tdvec<tdbyte> present;
	tdvec<Key> keys;
	tdvec<Value> values;
	
	tdmap() {
		keys.maybe_grow(MAP_INITIAL_CAPACITY);
		values.maybe_grow(MAP_INITIAL_CAPACITY);
		present.maybe_grow(MAP_INITIAL_CAPACITY);
	}
	
	void map(Key key, Value val) {
		int index = hasher.hash(key);
		while (*present.elem_at(index)) {
			index = (index + 1) % present.capacity;
		}
		keys.push_at(index, key);
		values.push_at(index, val);
		present.push_at(index, 1);
		
		if (((float)keys.count / (float)keys.capacity) > .6) {
			keys.maybe_grow(2 * keys.capacity);
			values.maybe_grow(2 * values.capacity);
			present.maybe_grow(2 * values.capacity);
		}
	}
};
