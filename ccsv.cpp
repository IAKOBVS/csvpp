#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <string.h>
/* #include <stdlib.h> */
#include "/home/james/c/nix.c/nix.h"
/* #include "/home/james/c/jArray/jarr.h" */
/* #include "/home/james/c/jString/jstr.h" */

#define FILENAME "/home/james/.local/bin/nix-db/db/upah-tukang_pryk-ns-lk_1.tsv"
#define deb(THING) std::cout << THING << '\n'

namespace ccsv {
	class Data {
		public:
			struct Record {
					std::vector<std::string> values;
			};

			const char *title;
			std::vector<std::string> keys;
			std::vector<Record> records;

			size_t keysGetIter(const char *toFind)
			{
				for (size_t i =0, j = this->keys.size(); i < j; ++i)
					if (this->keys[i].find(toFind) != std::string::npos)
						return i;
				perror("");
				return 0;
			}

			void keyPrint(const char *keyName)
			{
				for (size_t i = 0, at = this->keysGetIter(keyName), vecLen = this->records[at].values.size(); i < vecLen; ++i)
					std::cout << this->records[i].values[at] << '\n';
			}

			int recordGetIterator(const char *recordName)
			{
				for (size_t i = 0, vecLen = this->records.size(); i < vecLen; ++i)
					for (size_t j = 0, vecLen = this->records[i].values.size(); j < vecLen; ++j)
						if (this->records[i].values[j].find(recordName) != std::string::npos)
							return j;
				perror("");
				return 0;
			}

			void recordPrint(const char *recordName)
			{
				for (size_t i = 0, j = this->recordGetIterator(recordName), vecLen = this->records.size(); i < vecLen; ++i)
					std::cout << this->records[i].values[j] << '\n';
			}

			void print()
			{
				for (size_t i = 0, iLen = this->records.size(); i < iLen; ++i) {
					for (size_t j = 0, jLen = this->records[i].values.size(); j < jLen; ++j)
						std::cout << this->records[i].values[j] << ' ';
					std::cout << '\n';
				}
			}

			int init(const char *filename)
			{
				do {
					char *fileStr;
					if (!nix::cat((char *)filename, &fileStr))
						break;
					size_t w = 0;
					char delim = ',';
					for (int i = 0; fileStr[i] != '\n' && fileStr[i]; ++i)
						if (fileStr[i] == delim)
							++w;
					char *token;
					char delimPtr[] = {delim, '\n'};
					char *savePtr = fileStr;
					this->keys.reserve(++w);
					for (size_t i = 0; i < w; ++i)
						this->keys.push_back(strtok_r(savePtr, delimPtr, &savePtr));
					for (size_t line = 0; (token = strtok_r(savePtr, delimPtr, &savePtr)); ++line) {
						ccsv::Data::Record record;
						this->records.push_back(record);
						this->records[line].values.push_back(token);
						for (size_t i = 1; i < w; ++i)
							this->records[line].values.push_back(strtok_r(savePtr, delimPtr, &savePtr));
					}
					free(fileStr);
					return 1;
				} while (0);
				perror("");
				return 0;
			}
	};
}

int main()
{
	ccsv::Data data;
	data.init(FILENAME);
	data.print();
	return 0;
}
