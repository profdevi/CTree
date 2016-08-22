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


//v1.4 copyright Comine.com 20160821U2017
#ifndef MRelativeFileStack_h
#define MRelativeFileStack_h

////////////////////////////////////////////////
#include "MStringStack.h"
#include "MStringList.h"
#include "MString.h"
#include "MIntStack.h"
#include "MBuffer.h"

//******************************************************
//**  MRelativeFileStack class
//******************************************************
class MRelativeFileStack
	{
	////////////////////////////////////////////////
	MStringList mGlobalDirs;				// Holds collection of Global Include dirs
	MStringList mLocalDirs;					// Holds collection of local include
	MStringStack mCurrentFileStack;			// Holds Current FileName Only
	MStringStack mCurrentPathStack;			// Holds Complete Path to File
	MIntStack mCurrentLocation;				// Holds Info about current location
	
	////////////////////////////////////////////////
	void ClearObject(void);
	bool GetFileInCurrentDir(const char *filename,MString &dir
			,MString &outfile);
	bool GetFileInLocalDirs(const char *filename,MString &dir);
	bool GetFileInGlobalDirs(const char *filename,MString &dir);
	bool IsOnStackAlready(const char *dir,const char *filename);
	
	////////////////////////////////////////////////
	public:
	MRelativeFileStack(void);
	~MRelativeFileStack(void);
	bool Create(void);
	bool Destroy(void);
	bool AddGlobalDir(const char *dir);			// Add to global directories
	bool AddLocalDir(const char *dir);			// Add to local directories
	bool IsEmpty(void);							//=true if stack is empty
	bool Push(const char *filename);			// Push a new file
	bool Pop(void);								// Pop current file
	const char *GetCurrentPath(void);			// Returns Full Path of current file
	const char *GetFileName(void);				// Returns only filename
	bool IsFileInCurrentDir(void);				//=true if file is in current dir
	bool IsFileInLocalDir(void);				//=true if file is a local file
	bool IsFileInGlobalDir(void);				//=true if file is global file
	bool GetCurrentFilePath(MBuffer &buffer);	// Get Current file
	};

#endif // MRelativeFileStack_h

