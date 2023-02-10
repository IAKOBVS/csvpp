#include <iostream>
#include <string>
#include <vector>
#include <assert.h>


extern "C" {
#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/jArray/jarr.h"
#include "/home/james/c/jString/jstr.h"
}
#include <string.h>

#define FILENAME (char *)"/home/james/.local/bin/nix-db/db/upah-tukang_pryk-ns-lk_1.tsv"
#define deb(THING) \
	std::cout << THING << '\n'

namespace jcsv {
	class Data {
		public:
			char *title;
			std::vector<std::string> keys;

			class Record {
				public:
					std::vector<std::string> record;
			};
			std::vector<Record> records;
	};
}

int main()
{
	char *dest;
	::cat(FILENAME, &dest);
	int w = 0;
	char delim = ',';
	for (int i = 0; dest[i] != '\n' && dest[i]; ++i)
		if (dest[i] == delim)
			++w;
	char *token;
	char *savePtr = dest;
	char delimPtr[] = {delim, '\n'};
	jcsv::Data data;
	data.keys.reserve(++w);
	for (int i = 0; i < w; ++i)
		data.keys.push_back(strtok_r(savePtr, delimPtr, &savePtr));
	for (int line = 0; (token = strtok_r(savePtr, delimPtr, &savePtr)); ++line) {
		jcsv::Data::Record record;
		data.records.push_back(record);
		data.records[line].record.push_back(token);
		for (int i = 1; i < w && (token = strtok_r(savePtr, delimPtr, &savePtr)); ++i)
			data.records[line].record.push_back(token);
	}
	return 0;
}
