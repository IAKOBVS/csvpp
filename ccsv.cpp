#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

#include <string.h>
#include <stdlib.h>
#include "/home/james/c/nix.c/nix.h"

#define FILENAME "/home/james/.local/bin/nix-db/db/upah-tukang_pryk-ns-lk_1.tsv"
#define FILE_STOCKS "/tmp/stocks-nix/stocks/Balancepos20221230.txt"
#define deb(THING) std::cout << THING << '\n'

namespace ccsv {
	class Data {
		public:
			class Record {
				public:
					std::vector<std::string> values;
			};

			const char *title;
			std::vector<std::string> keys;
			std::vector<Record> records;

			size_t recordsLen()
			{
				return this->records.size();
			}

			size_t valuesLen()
			{
				return this->records[0].values.size();
			}

			size_t keysLen()
			{
				return this->keys.size();
			}

			size_t keysIter(const char *toFind)
			{
				for (size_t i =0, j = this->keys.size(); i < j; ++i)
					if (this->keys[i].find(toFind) != std::string::npos)
						return i;
				perror("");
				return 0;
			}

			size_t valueIter(const char *value)
			{
				for (size_t i = 0, vecLen = this->records.size(); i < vecLen; ++i)
					for (size_t j = 0, vecLen = this->records[i].values.size(); j < vecLen; ++j)
						if (this->records[i].values[j].find(value) != std::string::npos)
							return j;
				perror("");
				return 0;
			}

			void keyPrint(const char *key)
			{
				for (size_t i = 0, at = this->keysIter(key), vecLen = this->records[at].values.size(); i < vecLen; ++i)
					std::cout << this->records[i].values[at] << '\n';
			}

			void keysPrint(char delim)
			{
				for (auto key : this->keys)
					std::cout << key << delim; 
				std::cout << '\n';
			}

			void getRecord(const char *value)
			{
				for (size_t i = 0, j = this->valueIter(value), vecLen = this->records.size(); i < vecLen; ++i)
					if (this->records[i].values.data()->find(value) != std::string::npos)
						std::cout << this->records[i].values[j] << '\n';
			}

			void print(char delim)
			{
				for (size_t i = 0, iLen = this->records.size(); i < iLen; ++i) {
					for (size_t j = 0, jLen = this->records[i].values.size(); j < jLen; ++j)
						std::cout << this->records[i].values[j] << delim;
					std::cout << '\n';
				}
			}

			int init(const char *filename, int delim)
			{
				do {
					char *fileStr;
					if (!nixCat((char *)filename, &fileStr))
						break;
					char *token;
					char delimPtr[] = {(char)delim, '\n'};
					char *savePtr = fileStr;
					size_t w = nixWcwnl(fileStr, delim);
					this->keys.reserve(w);
					for (size_t i = 0, j = w; i < j; ++i)
						this->keys.push_back(strtok_r(savePtr, delimPtr, &savePtr));
					this->keysPrint(',');
					for (size_t line = 0; (token = strtok_r(savePtr, delimPtr, &savePtr)); ++line) {
						ccsv::Data::Record record;
						this->records.push_back(record);
						this->records[line].values.push_back(token);
						for (size_t i = 1; i < w && (token = strtok_r(savePtr, delimPtr, &savePtr)); ++i)
							this->records[line].values.push_back(token);
					}
					free(fileStr);
					return 1;
				} while (0);
				perror("");
				return 0;
			}

	};
}
int main(int argc, char **argv)
{
	ccsv::Data data;
	data.init("/tmp/stocks", ',');
	for (size_t i = 0, j = data.recordsLen(); i < j; ++i)
		if (data.records[i].values.data()->find("UNVR") != std::string::npos)
			for (int x = 0, len = data.valuesLen(); x < len; ++x)
				std::cout << data.records[i].values[x] << '\n';
	return 0;
}
