#ifndef INTERNAL_F_SYS_H
#define INTERNAL_F_SYS_H

#include "InternalDef.h"
#include "InternalErrorLogger.h"
#include "InternalSys.h"
#include "InternalLib.h"
#include "InternalStorageVecs.h"

#include <string>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <codecvt>

namespace InternalFsys { //Fsys = File system

	MDEF std::map<std::string, std::string> MyMapStor;

	std::streamsize get_flength(std::ifstream& file);

	std::string retMap(std::string value);

	std::string getFileType(std::string path);

	bool fileExitCheck(const std::string path);

	bool is_empty(std::ifstream& pFile);

	std::string readNormal(std::string path);

	void delStor(std::string value);

	tokenType splitTokens(const std::string splitKey, const std::string string,void(*fun)(const char& i, std::string& str) = [](const char& i, std::string& str){});

	void resetFiles(bool createAsNew = false);

	std::string read(std::string key, std::string path);

	std::string readNormal(std::string path);

	tokenType readVec(std::string path, std::string key);

	std::string readLine(std::string path, size_t line);

	void write(std::string path, std::string key, std::string newValue);

	void writeNormal(std::string path, std::string value, bool trunc = false);

	void writeVec(std::vector<std::string> vec,std::string path, std::string key);

	void addToVec(std::string path, std::string key, std::string val);

	
}

namespace InternalFsys {
	namespace sys { 
		bool makeFile(std::string name, std::string path, std::string message = "");

		void deleteFile(std::string name, std::string path);

		void makeDir(std::string names);

		void deleteDir(std::string path);

	}

	/*
	namespace save {
		void createSaveFile();

		void createNewSaveFile();

		void readSaveFile();
	}
	*/

}

#endif
