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


//v1.4 copyright Comine.com 20160821U2027
#include "MStdLib.h"
#include "MStringList.h"
#include "MCTokenReaderStack.h"
#include "MBuffer.h"
#include "MStringStack.h"
#include "MRelativeFileStack.h"
#include "MPathNameList.h"
#include "MFileOps.h"
#include "MCHeaderBuilder.h"


//******************************************************
//**  Module Elements
//******************************************************
static const int GMaxFileDepth=200;


////////////////////////////////////////////////
enum GState
	{
	GS_START,GS_POUND,GS_POUND_INCLUDE,GS_POUND_INCLUDE_BRA,
	GS_POUND_INCLUDE_BRA_KET,GS_ERROR
	};


////////////////////////////////////////////////
// Remove all extra double quotes
static void GClearUpHeaderFile(MBuffer &nextheaderfile)
	{	char *src,*dst;
	for(src=dst=nextheaderfile.GetBuffer(); ; ++src)
		{
		if((*dst=*src)!='"') { dst++; }
		if(*src==0) { break; }
		}
	}


/////////////////////////////////////////////////
static int GCompareStrings(const char *src,const char *dst)
	{	int i;
	for(i=0;  ;++i)
		{
		if(src[i]!=dst[i])
			{ return src[i]-dst[i]; }
		if(src[i]==0)
			{ return 0; }
		}
	}


//////////////////////////////////////////////////
static bool GAddInitialFile(const char *filename
		,MRelativeFileStack &stack)
	{	MFileOps fileops;
	if(fileops.Create()==false)
		{
		return false;
		}

	MString fullpathname;
	if(fileops.GetAbsolutePath(filename,fullpathname)==false)
		{
		return false;
		}

	
	MPathNameList namelist;
	if(namelist.Create(fullpathname.Get())==false)
		{
		return false;
		}

	// Add the path first
	if(namelist.Pop()==false)
		{
		return false;
		}

	MBuffer filedir;
	if(namelist.GetFullPath(filedir)==false)
		{
		return false;
		}

	if(stack.AddLocalDir(filedir)==false)
		{
		return false;
		}


	if(namelist.Create(fullpathname.Get())==false)
		{
		return false;
		}

	if(stack.Push(namelist.GetTop())==false)
		{
		return false;
		}

	return true;	
	}


///////////////////////////////////////////////////////
static bool AddFileToVisitedFiles(const char *filename,
		const char *pathdir,MStringStack &stack)
	{
	if(filename==NULL || *filename==0)
		{
		}

	MPathNameList filenamelist;
	if(filenamelist.Create(filename)==false)
		{
		return false;
		}

	MPathNameList pathnamelist;
	if(pathnamelist.Create(pathdir)==false)
		{
		return false;
		}

	// Add File Name to path list
	if(pathnamelist.Push(filenamelist)==false)
		{
		return false;
		}

	MBuffer fullpathname;
	if(pathnamelist.GetFullPath(fullpathname)==false)
		{
		return false;
		}

	// Check if file already exists as visited
	if(stack.IsElement(fullpathname)==true)
		{  return false; }
	
	//=File has never been visited

	if(stack.Push(fullpathname)==false)
		{
		return false;
		}

	return true; // Success
	}



//******************************************************
//**  MCHeaderBuilder class
//******************************************************
void MCHeaderBuilder::ClearObject(void)
	{
	}

////////////////////////////////////////////////
MCHeaderBuilder::MCHeaderBuilder(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MCHeaderBuilder::~MCHeaderBuilder(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MCHeaderBuilder::Create(void)
	{
	Destroy();
	// Set up global include list
	if(mGlobalInclude.Create()==false)
		{
		Destroy();  return false;
		}

	// Set up Local include list
	if(mLocalInclude.Create()==false)
		{
		Destroy();  return false;
		}
	return true;
	}


////////////////////////////////////////////////
bool MCHeaderBuilder::Destroy(void)
	{
	mGlobalInclude.Destroy();
	mLocalInclude.Destroy();
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MCHeaderBuilder::AddGlobalIncludeDirectory(const char *mainincludedir)
	{
	if(mGlobalInclude.AddString(mainincludedir)==false)
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MCHeaderBuilder::AddLocalIncludeDirectory(const char *searchincludedir)
	{
	if(mLocalInclude.AddString(searchincludedir)==false)
		{
		return false;
		}

	return true;
	}


/////////////////////////////////////////////////
bool MCHeaderBuilder::MakeHeader(const char *srcfile,const char *dstfile)
	{
	// Create the lexer
	MCTokenReaderStack lexerstack;
	if(lexerstack.Create(GMaxFileDepth)==false)
		{
		return false;
		}

	MRelativeFileStack filestack;
	if(filestack.Create()==false)
		{
		return false;
		}	
	
	//****Add Global directories***

	//Reset global include dir reader
	if(mGlobalInclude.ReadReset()==false)
		{
		return false;
		}

	const char *str;
	while((str=mGlobalInclude.ReadString())!=NULL)
		{
		if(filestack.AddGlobalDir(str)==false)
			{
			return false;
			}
		}

	//**** Add Local Include Directories ****

	//Reset local include dir reader
	if(mLocalInclude.ReadReset()==false)
		{
		return false;
		}

	while((str=mLocalInclude.ReadString())!=NULL)
		{
		if(filestack.AddLocalDir(str)==false)
			{
			return false;
			}
		}

	// Now Find and add the local file
	if(GAddInitialFile(srcfile,filestack)==false)
		{
		}



	// Create visited file set
	MStringStack visitedfiles;
	if(visitedfiles.Create()==false)
		{
		return false;
		}

	if(AddFileToVisitedFiles(filestack.GetFileName()
			,filestack.GetCurrentPath(),visitedfiles)==false)
		{
		return false;
		}
	

	// Create FileOutput
	MFileOutput fileoutput;
	if(fileoutput.Create(dstfile)==false)
		{
		return false;
		}
	
	// Buffer for next header file
	MBuffer nextheaderfile;
	if(nextheaderfile.Create(1000)==false)
		{
		return false;
		}

	// Now add the file to lexerstack
	MBuffer currentfilepath;
	if(filestack.GetCurrentFilePath(currentfilepath)==false)
		{
		return false;
		}


	// Start new lexer
	if(lexerstack.Push(currentfilepath)==false)
		{
		return false;
		}
	
	// Start the state machine
	GState state=GS_START; 
	while(lexerstack.IsEmpty()==false)
		{
		if(lexerstack.Read()==false)
			{
			lexerstack.Pop();
			filestack.Pop();
			continue;
			}

		// Get the next token data
		const char *lexeme=lexerstack.GetLexeme();
		MCTokenReaderType lexemetype=lexerstack.GetLexemeType();

		// State GS_START
		if(state==GS_START)
			{
			if(lexemetype==MCTRT_OPERATOR && MStdStrCmp(lexeme,"#")==0)
				{  state=GS_POUND;  continue;  }
	
			if(lexemetype==MCTRT_NEWLINE)
				{
				fileoutput.WriteChars("\r\n");	// Force new dos new line
				state=GS_START;
				continue;
				}

			fileoutput.WriteChars(lexeme);  state=GS_START;
			continue;
			}

		// State GS_POUND
		if(state==GS_POUND)
			{
			if(lexemetype==MCTRT_SYMBOL && MStdStrCmp(lexeme,"include")==0)
				{  state=GS_POUND_INCLUDE;  continue;  }

			if(lexemetype==MCTRT_SPACE){  state=GS_POUND;  continue;  }

			// Reached another preprocessing statement
			fileoutput.WriteChars("#");
			fileoutput.WriteChars(lexeme);
			state=GS_START;  continue;
			}

		// State GS_POUND_INCLUDE
		if(state==GS_POUND_INCLUDE)
			{
			if(lexemetype==MCTRT_SPACE){ state=GS_POUND_INCLUDE;  continue; }

			if(lexemetype==MCTRT_OPERATOR && MStdStrCmp(lexeme,"<")==0)
				{
				state=GS_POUND_INCLUDE_BRA;
				nextheaderfile.SetString(""); // clear the buffer
				continue;
				}

			if(lexemetype==MCTRT_DQSTRING)
				{
				state=GS_POUND_INCLUDE_BRA_KET;
				nextheaderfile.SetString(lexeme);
				continue;
				}

			state=GS_START; continue;
			}

		// State GS_POUND_INCLUDE_BRA
		if(state==GS_POUND_INCLUDE_BRA)
			{
			if(lexemetype==MCTRT_OPERATOR && MStdStrCmp(lexeme,">")==0)
				{  state=GS_POUND_INCLUDE_BRA_KET; continue; }

			if(lexemetype==MCTRT_NEWLINE)
				{
				return false;
				}

			nextheaderfile.StringAppend(lexeme);
			state=GS_POUND_INCLUDE_BRA; continue;
			}

		// State GS_POUND_INCLUDE_BRA_KET
		if(state==GS_POUND_INCLUDE_BRA_KET &&
				lexemetype!=MCTRT_NEWLINE)
			{ continue; }

		//=We can now process the the header file
		state=GS_START;  // Reset State for next turn
		
		// Clean up all extra arguments
		GClearUpHeaderFile(nextheaderfile);

		// Now Push the file onto the filestack
		if(filestack.Push(nextheaderfile)==false)
			{
			/* SKIPPED: #include \"0000.h\" */
			fileoutput.WriteChars("/* SKIPPED: #include \"");
			fileoutput.WriteChars(nextheaderfile);
			fileoutput.WriteChars("\" */\r\n");
			state=GS_START;  continue;
			}

		if(filestack.IsFileInGlobalDir()==true)
			{
			//= The file is a global include file
			//  Pop the file off the stack
			if(filestack.Pop()==false)
				{
				return false;
				}
			
			// Write proper header
			fileoutput.WriteChars("#include <");
			fileoutput.WriteChars(nextheaderfile);
			fileoutput.WriteChars(">\r\n");
			continue;
			}

		//=File must be in current dir or local include

		// Check if file has already been visited

		MBuffer fullfilepath;
		if(filestack.GetCurrentFilePath(fullfilepath)==false)
			{
			return false;
			}

		if(visitedfiles.IsElement(fullfilepath)==true)
			{
			//=File has already been visited
			if(filestack.Pop()==false)
				{
				return false;
				}
			// We are skipping repeated files
			continue;
			}

		// Add File to visited list
		if(visitedfiles.Push(fullfilepath)==false)
			{
			return false;
			}

		// Start Processing the file
		if(lexerstack.Push(fullfilepath)==false)
			{
			return false;
			}
		}
	
	return true;
	}


