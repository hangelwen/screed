#include "dbwrite.h"
#include <string>
#include <fstream>
#include <iostream>
#include <new>
#include <stdlib.h>

using namespace std;

/*-------------------------------------------------
 * Constructor
 * Opens .seqdb2 and .seqdb2.idx files in the same
 * directory as the filename passed in using the
 * dbFile and idxFile file handles
-------------------------------------------------*/
dbwrite::dbwrite(string fname){
	open = true;
	fname.append(".seqdb2");
	dbFile.open(fname.c_str(), ios::out | ios::binary);
	fname.append(".idx");
	idxFile.open(fname.c_str(), ios::out | ios::binary);

	if((!dbFile.is_open()) || (!idxFile.is_open())){
			open = false;
	}
}

/*-----------------------------------------------
 * Destructor
 * Closes the database and index file handles
-----------------------------------------------*/
dbwrite::~dbwrite(){
	dbFile.close();
	idxFile.close();
}

/*------------------------------------------
 * Name: close
 * Explicitly closes the file handles
------------------------------------------*/
void dbwrite::close(){
	dbFile.close();
	idxFile.close();
}

/*-----------------------------------------
 * writeFirst
 * Write the put pointer's location to the
 * index file
----------------------------------------*/
bool dbwrite::writeFirst(){
	idxFile.write((char*)&dbFile.tellp(), sizeof(dbFile.tellp()));
	idxFile.write(&newline, 1);
	return !(idxFile.fail());
}

/*---------------------------------------
 * writeLine
 * Writes the given line to the database
 * file. First write the size of the
 * line and then the line itself
---------------------------------------*/
bool dbwrite::writeLine(string theLine){
	long long lsize;
	lsize = theLine.size() + 1;
	dbFile.write((char*)&(lsize), sizeof(lsize));
	dbFile.write(&space, 1);
	dbFile.write(theLine.c_str(), theLine.size());
	dbFile.write(&newline, 1);
	return !(dbFile.fail());
}

/*-------------------------------------
 * writeDelim
 * Write the record deliminator to the
 * database file
-------------------------------------*/
bool dbwrite::writeDelim(){
	dbFile.write(&delim, 1);
	return !(dbFile.fail());	
}
