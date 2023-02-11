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

			const char *title;
			std::vector<std::string> keys;
			std::vector<Record> records;

			size_t keyIs(const char *toFind)
			{
				for (int i =0, j = this->keys.size(); i < j; ++i)
					if (!this->keys[i].find(toFind))
						return i;
				perror("");
				return 0;
			}

			int recordIs(const char *recordName)
			{
				for (size_t i = 0, vecLen = this->records.size(); i < vecLen; ++i)
					for (size_t j = 0, vecLen = this->records[i].record.size(); j < vecLen; ++j)
						if (!this->records[i].record[j].find("recordName"))
							return j;
				return 0;
			}

			void recordPrint(const char *recordName)
			{
				for (size_t i = 0, j = this->recordIs(recordName), vecLen = this->records.size(); i < vecLen; ++i)
					std::cout << this->records[i].record[j] << '\n';
			}

			void keyPrint(const char *keyName)
			{
				for (size_t i = 0, at = this->keyIs(keyName), vecLen = this->records[at].record.size(); i < vecLen; ++i)
					std::cout << this->records[i].record[at] << '\n';
			}

			int findRecords(const char *toFind)
			{
				return 1;
			}

			int init(const char *filename)
			{
				do {
					char *fileStr;
					if (!::cat((char *)filename, &fileStr))
						break;
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
	return 0;
}
