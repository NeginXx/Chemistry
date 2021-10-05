#include <iostream>

int main() {
	const size_t n = 2;
	char strings[n][4] = {{'a', 'a', 'a', 'a'},
                        {'a', 'a', 'a', 'a'}};
	for (size_t i = 0; i < n; ++i) {
		scanf("%s", strings[i]);
	}
	for (size_t i = 0; i < n; ++i) {
		printf("%s\n", strings[i]);
	}
}