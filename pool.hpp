// POOL DECLS
struct pool_entry_info {
	bool available : 1;
};
#define DEFAULT_POOL_SIZE 100000

template<typename Data_Type>
struct pool_handle;

// A templated, fixed size pool of objects which stores them in continuous memory and uses handles to interface with the data elements 
template<typename Data_Type, int num_elements>
struct Pool {
	Data_Type* entries;
	pool_entry_info* info;
	
	void init();
	pool_handle<Data_Type> next_available();
	inline Data_Type* get(pool_handle<Data_Type> handle);
	inline void mark_available(pool_handle<Data_Type> handle);
	inline void mark_unavailable(pool_handle<Data_Type> handle);
};

// POOL DEFNS
template<typename Data_Type, int num_elements>
void Pool<Data_Type, num_elements>::init() {
	entries = (Data_Type*)calloc(sizeof(Data_Type), num_elements);
	fox_for(indx, num_elements) {
		Data_Type* entry = entries + indx;
		new (entry) Data_Type;
	}

	// Allocate some memory for the infos and set 
	info = (pool_entry_info*)calloc(sizeof(pool_entry_info), num_elements);
	memset(info, 1, sizeof(pool_entry_info) * num_elements);
}

// A handle to an element in a pool. Wrapper around an int with convenient operators.
template<typename Data_Type>
struct pool_handle {
	int handlecito = -1;
	Pool<Data_Type, DEFAULT_POOL_SIZE>* pool = nullptr;
	
	// These get the actual entity
	Data_Type* deref() {
		return pool->get(*this);
	}
	Data_Type* operator->() {
		return pool->get(*this);
	}
	Data_Type* operator()() {
		if (!*this) return nullptr;
		return pool->get(*this);
	}
	// This gets the handle (i.e. table index)
	int operator*() {
		return handlecito;
	}
	bool operator==(pool_handle<Data_Type> other) {
		return handlecito == other.handlecito;
	}
	bool operator==(int other) {
		return handlecito == other;
	}
	bool operator!=(pool_handle<Data_Type> other) {
		return !(*this == other);
	}
	bool operator!=(int other) {
		return !(*this == other);
	}
	operator bool() {
		return (handlecito != -1) && (pool != nullptr);
	}
};

//@slow
template<typename Data_Type, int num_elements>
pool_handle<Data_Type> Pool<Data_Type, num_elements>::next_available() {
	for (int handlecito = 0; handlecito < num_elements; handlecito++) {
		pool_entry_info entry = info[handlecito];
		if (entry.available) {
			pool_handle<Data_Type> handle = { handlecito, this };
			mark_unavailable(handle);
			return handle;
		}
	}
	
	tdns_log.write("Ran out of handles!");
	fox_assert(false);
	return { -1, nullptr }; // for the compiler
};

template<typename Data_Type, int num_elements>
inline Data_Type* Pool<Data_Type, num_elements>::get(pool_handle<Data_Type> handle) {
	fox_assert(handle);
	fox_assert(*handle >= 0);
	fox_assert(*handle < num_elements);
	fox_assert(this);
	return entries + *handle;
}

template<typename Data_Type, int num_elements>
inline void Pool<Data_Type, num_elements>::mark_available(pool_handle<Data_Type> handle) {
	info[*handle].available = true;
	memset(entries + *handle, 0, sizeof(Data_Type));
}

template<typename Data_Type, int num_elements>
void Pool<Data_Type, num_elements>::mark_unavailable(pool_handle<Data_Type> handle) {
	info[*handle].available = false;
}
