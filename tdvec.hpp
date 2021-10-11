#define ARR_INITIAL_CAPACITY 8
template<typename T>
struct tdvec {
	T* data;
	bool* present;
	int count;
	int capacity;
	
	tdvec() {
		data = (T*)calloc(ARR_INITIAL_CAPACITY, sizeof(T));
		present = (bool*)calloc(ARR_INITIAL_CAPACITY, sizeof(bool));
		capacity = ARR_INITIAL_CAPACITY;
		count = 0;
	}
	~tdvec() {
		free(data);
		free(present);
	}
	
	void maybe_grow(int new_size) {
		if (new_size > capacity) {
			data = (T*)realloc(data, sizeof(T) * new_size);
			present = (bool*)realloc(present, sizeof(bool) * new_size);
			fox_assert(data);
			fox_assert(present);
			memset(data + capacity, 0, sizeof(T) * (new_size - capacity));
			memset(present + capacity, 0, sizeof(bool) * (new_size - capacity));
			capacity = new_size;
		}
	}
	
	T* push(T item) {
		if (count == capacity) maybe_grow(capacity * 2);
		data[count] = item;
		present[count] = true;
		
		count++;
		
		return back();
	}
	T* push_at(int i, T item) {
		maybe_grow(i + 1); // i is an index, so we need capacity = i + 1
		data[i] = item;
		return &data[i];
	}
	
	T* elem_at(int i) {
		return &data[i];
	}
	T* back() {
		return elem_at(count - 1);
	}
	
	void erase(int i) {
		if (i > capacity - 1) return;
		
		// Only have to memcpy if we're not erasing from the back
		else if (i != (capacity - 1)) {
			memcpy(data + i, data + i + 1, capacity - i - 1);
			memcpy(present + i, present + i + 1, capacity - i - 1);
		} 
		
		// Always zero out the last element
		data[capacity - 1] = 0;
		present[capacity - 1] = 0;
		count--;
	}
	
};
