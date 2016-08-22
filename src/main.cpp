/*
Copyright (C) 2011-2016, Comine.com ( profdevi@ymail.com )
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
- The the names of the contributors of this project may not be used to 
  endorse or promote products derived from this software without specific 
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
`AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


//v1.4 copyright Comine.com 20160821U2015
#include "MStdLib.h"
#include "MCommandArg.h"
#include "MBuffer.h"
#include "MCHeaderBuilder.h"


//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="CTree";	// Used in Help
static const char *GApplicationVersion="1.3";		// Used in Help

////////////////////////////////////////////////////
static void GDisplayHelp(void);

////////////////////////////////////////////////////
int main(int argn,const char *argv[])
	{
	MCommandArg args(argn,argv);

	///////////////////////////////////////////////
	if(args.GetArgCount()<=2) {  GDisplayHelp();  return 0;  }

	MCHeaderBuilder headergen;
	if(headergen.Create()==false)
		{
		return 1;
		}

	// Get Input File
	MBuffer inputfile(1000);
	const char *filename;  int index;
	if(args.GetNameValue("-in=",filename,index)==false)
		{
		MStdPrintf("**No Input File Specifield\n");
		return 1;
		}
	inputfile.SetString(filename);
	args.RemoveArg(index);

	// Get Output File
	MBuffer outputfile(1000);
	if(args.GetNameValue("-out=",filename,index)==false)
		{
		MStdPrintf("**No Input File Specifield\n");
		return 1;
		}
	outputfile.SetString(filename);
	args.RemoveArg(index);

	// Now Process each of the -include dirs
	while(args.GetNameValue("-include=",filename,index)==true)
		{
		if(headergen.AddGlobalIncludeDirectory(filename)==false)
			{
			MStdPrintf("**Unable to add include directory: %s\n",filename);
			return 1;
			}

		args.RemoveArg(index);
		}
	

	// Now Process each of the -search dirs
	while(args.GetNameValue("-search=",filename,index)==true)
		{
		if(headergen.AddLocalIncludeDirectory(filename)==false)
			{
			MStdPrintf("**Unable to add search directory: %s\n",filename);
			return 1;
			}

		args.RemoveArg(index);
		}

	// Now Check for extra arguments
	if(args.GetArgCount()!=1)
		{
		MStdPrintf("**Found unknown extra arguments\n");
		return 1;
		}

	// Now Execute the Generator
	if(headergen.MakeHeader(inputfile.GetBuffer()
			,outputfile.GetBuffer())==false)
		{
		MStdPrintf("**Failed making header file\n");
		return 1;
		}

	MStdPrintf("Done...\n");
	return 0;
	}


////////////////////////////////////////////////////
static void GDisplayHelp(void)
	{
	MStdPrintf(	"\n"
			"   usage:  %s -in=<file>  -out=<file>\n"
			"               [-search=<dir>]*  [-include=<dir>]* [-?]\n"
			"           v%s copyright Comine.com\n"
			"\n"
			"   Generates a single header file by following through #include\n"
			"   references.  Header files not found in either search or\n"
			"   include directories will be commented out.\n"
			"\n"
			"   options:\r\n"
			"           -search  :  Header files in dir will be processed to output\n"
			"           -include :  Header files in dir will not be processed\n"
			"           -in      :  Main Input File where search begins\n"
			"           -out     :  Output file which contains the results\n"
			"\n"
			,GApplicationName,GApplicationVersion);
	}



