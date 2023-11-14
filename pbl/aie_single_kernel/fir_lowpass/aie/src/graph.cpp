// Copyright (C) 2023 Advanced Micro Devices, Inc
//
// SPDX-License-Identifier: MIT


#include "graph.h"
#include <iostream>
#include <iomanip>
#include <fstream>

#include <unistd.h>

using namespace std;
using namespace adf;

Fir_Lowpass16 F2;

#if defined(__AIESIM__) || defined(__X86SIM__)

int main(){
	F2.init();
	F2.run(NITERATIONS);
	F2.end();

#if defined(__X86SIM__)

#define OUTFILE "./x86simulator_output/data/output.txt"
#define REF_OUTFILE "../data/output_golden.txt"

	ifstream fin1,fin2;
	char l1[256],l2[256];

//	char cwd[256];
//	if (getcwd(cwd, sizeof(cwd)) == NULL)
//	      perror("getcwd() error");
//	    else
//	      printf("current working directory is: %s\n", cwd);

	fin1.open(REF_OUTFILE);
	if(!fin1.is_open())
	{
		cerr << "Cannot open '" << REF_OUTFILE << "'" << endl;
		cin >> l1;
		return(1);
	}

	fin2.open(OUTFILE);
	if(!fin2.is_open())
	{
		cerr << "Cannot open '" << OUTFILE <<  "'" << endl;
		cin >> l1;
		return(1);
	}

	int ntot=0;
	int nerr=0;

	while(!fin1.eof() && !fin2.eof())
	{
		fin1.getline(l1,256);
		fin2.getline(l2,256);
		if(strlen(l1)>0 && strlen(l2)>0)
		{
			if(!strcmp(l1,l2))
				cout << "Line " << setw(4) << ntot << "     OK" << endl;
			else
			{
				cout << "Line " << setw(4) << ntot << "          ERROR     Ref: " << l1 << "     Simulation: " << l2 << endl;
				nerr++;
			}
			ntot++;
		}
	}

	fin1.close();
	fin2.close();

	cout << endl << endl;
	cout << "Number of Output: " << ntot << endl;
	cout << "Number of CORRECT output: " << ntot-nerr << endl;
	cout << "Number of WRONG   output: " << nerr << endl;
#endif
	return(0);
}

#endif
