// Copyright 2008-2009 Michigan State University. All rights reserved.

#include "dbwrite.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

#define linsiz 1000

using namespace std;

int main(int argc, char *argv[]){
    fstream theFile;
    char endck;
    char line[linsiz], line2[linsiz];
    int multiplier;
    index_type rlinelen, dnalen;
    streampos pre, post;

    if((argc < 2) || (argc > 3)){
        cerr << "ERROR: USAGE IS: " << argv[0] << " <dbfilename>\n";
        exit(1);
    }
    multiplier = 2;
    if(argc == 3){
        multiplier = atol(argv[2]);
        if(multiplier <= 0){
            cerr << "ERROR: CAN'T CREATE DB OF 0 OR NEGATIVE HASH LENGTH\n";
            exit(1);
        }
    }
    dbwrite db(argv[1], 'q', multiplier);
    if(!db.is_open()){
        cerr << "ERROR: DATABASE FILES ARE NOT OPEN\n";
        exit(1);
    }
    theFile.open(argv[1], fstream::in);
    while(!theFile.eof()){

        // Read in the data line by line and store it in the relevant
        // character array
        theFile.ignore(1);
        // A file integrity checker
        if(!theFile.good()){
        cerr << "ERROR: There is something wrong with the " <<
            "source file stream\n";
        db.close();
        exit(1);
        }
       
        // Read and write the name
        rlinelen = gline(theFile, line, linsiz);
        db.writeFirst(line, rlinelen);
        db.writeLine(line, rlinelen);

        // Read the sequence
		dnalen = gline(theFile, line, linsiz);

        // Read and write the description
		theFile.ignore(2); // Ignores the +\n characters
		rlinelen = gline(theFile, line2, linsiz);
        db.writeLine(line2, rlinelen);
		endck = theFile.peek();

        // Write the sequence
        db.writeLine(line, dnalen);

        // Make sure nothing went wrong when writing to the database
		if(db.fail() == true){
            cerr << "ERROR: Something went wrong while writing the "
                << "database.\nHave you run out of space?\n";
            db.close();
            exit(1);
        }
	}
	theFile.close();
    db.hash2Disk();
	cout << "Database saved in " << argv[1] << "_screed\n";
	cout << "Index saved in " << argv[1] << "_screed_idx\n";
	cout << "Hash saved in " << argv[1] << "_screed_hash\n";

	return 0;
}
