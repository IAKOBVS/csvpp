#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <string.h>
/* #include <stdlib.h> */
extern "C" {
#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/jArray/jarr.h"
#include "/home/james/c/jString/jstr.h"
}

#define FILENAME (char *)"/home/james/.local/bin/nix-db/db/upah-tukang_pryk-ns-lk_1.tsv"
#define deb(THING) std::cout << THING << '\n'

namespace jcsv {
	class Data {
		public:
			class Record {
				public:
					std::vector<std::string> record;
			};

			char *title;
			std::vector<std::string> keys;
			std::vector<Record> records;

			int findKey(char *toFind)
			{
				int i = 0;
				for (int j = this->keys.size(); i < j; ++i)
					if (this->keys[i].find(toFind))
						return i;
				perror("");
				return 0;
			}

			int findRecords(char *toFind)
			{
				return 0;
			}
	};
}

int main()
{
	char *fileStr;
	::cat(FILENAME, &fileStr);
	int w = 0;
	char delim = ',';
	for (int i = 0; fileStr[i] != '\n' && fileStr[i]; ++i)
		if (fileStr[i] == delim)
			++w;
	char *token;
	char delimPtr[] = {delim, '\n'};
	jcsv::Data data;
	data.keys.reserve(++w);
	for (int i = 0; i < w; ++i)
		data.keys.push_back(strtok_r(fileStr, delimPtr, &fileStr));
	for (int line = 0; (token = strtok_r(fileStr, delimPtr, &fileStr)); ++line) {
		jcsv::Data::Record record;
		data.records.push_back(record);
		data.records[line].record.push_back(token);
		for (int i = 1; i < w; ++i) {
			data.records[line].record.push_back(strtok_r(fileStr, delimPtr, &fileStr));
		}
	}
	return 0;
}
