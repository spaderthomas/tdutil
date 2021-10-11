struct Circle_Buffer {
	int* data = nullptr;
	int head = 0;
	int capacity = 0;
	int len = 0;
	
	void push_back(int elem);
	std::optional<int> pop_front();
	void clear();
};

void Circle_Buffer::push_back(int elem) {
	fox_assert(len <= capacity);
	fox_assert(head >= 0 && head < capacity);
	if (len == capacity) { return; }
	data[(head + len) % capacity] = elem;
	len++;
}
std::optional<int> Circle_Buffer::pop_front() {
	fox_assert(len <= capacity);
	fox_assert(head >= 0 && head < capacity);
	if (len) {
		int ret = data[head];
		head = (head + 1) % capacity;
		len--;
		return ret;
	}
	return {};
}
void Circle_Buffer::clear() {
	memset(data, 0, capacity * sizeof(data));
	head = 0;
	len = 0;
}
