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

namespace ccsv {
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

			void init(char *filename)
			{
				char *fileStr;
				::cat(filename, &fileStr);
				int w = 0;
				char delim = ',';
				for (int i = 0; fileStr[i] != '\n' && fileStr[i]; ++i)
					if (fileStr[i] == delim)
						++w;
				char *token;
				char delimPtr[] = {delim, '\n'};
				char *savePtr = fileStr;
				this->keys.reserve(++w);
				for (int i = 0; i < w; ++i)
					this->keys.push_back(strtok_r(savePtr, delimPtr, &savePtr));
				for (int line = 0; (token = strtok_r(savePtr, delimPtr, &savePtr)); ++line) {
					ccsv::Data::Record record;
					this->records.push_back(record);
					this->records[line].record.push_back(token);
					for (int i = 1; i < w; ++i)
						this->records[line].record.push_back(strtok_r(savePtr, delimPtr, &savePtr));
				}
				free(fileStr);
			}
	};
}

int main()
{
	ccsv::Data data;
	data.init(FILENAME);
	return 0;
}
