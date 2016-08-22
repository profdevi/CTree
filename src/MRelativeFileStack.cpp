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
#include "MStdLib.h"
#include "MStringStack.h"
#include "MStringList.h"
#include "MPathNameList.h"
#include "MFileOps.h"
#include "MString.h"
#include "MIntStack.h"
#include "MBuffer.h"
#include "MDirOps.h"
#include "MRelativeFileStack.h"

//******************************************************
//**  Module Elements
//******************************************************
static const int GMaxFileDepth=1000;			// Maximum depth of stacks
enum GFileLocation
	{
	GFL_CURRENT_DIR=0,
	GFL_LOCAL_DIR=1,
	GFL_GLOBAL_DIR=2
	};


//////////////////////////////////////////////////////
static bool GNormalizeFilePath(const char *dir,const char *filename
		,MBuffer &normaldir,MBuffer &normalfile)
	{
	MPathNameList dirlist;  dirlist.Create(dir);
	MPathNameList filelist;	filelist.Create(filename);
	dirlist.Push(filelist);

	normalfile.Create(1000);
	normalfile.SetString(filelist.GetTop());
	
	dirlist.Pop();
	dirlist.GetFullPath(normaldir);
	return true;
	}

//******************************************************
//**  MRelativeFileStack class
//******************************************************
void MRelativeFileStack::ClearObject(void)
	{
	}

////////////////////////////////////////////////
bool MRelativeFileStack::GetFileInCurrentDir(const char *filename,MString &dir
			,MString &outfile)
	{
	if(mCurrentPathStack.IsEmpty()==true) { return false; }

	MPathNameList fileseq;
	if(fileseq.Create(filename)==false)
		{
		return false;
		}

	MPathNameList pathseq;
	if(pathseq.Create(mCurrentPathStack.GetTop())==false)
		{
		return false;
		}
	
	MPathNameList currentfile;
	if(currentfile.Create(mCurrentFileStack.GetTop())==false)
		{
		return false;
		}

	// Add File Sequence to pathseq
	if(pathseq.Push(currentfile)==false)
		{
		return false;		
		}

	// Pop the filename to get sequence of current dir
	if(pathseq.Pop()==false)
		{
		return false;
		}

	if(pathseq.Push(fileseq)==false)
		{
		return false;
		}

	outfile.Create(fileseq.GetTop());

	MBuffer pathbuffer;
	if(pathseq.GetFullPath(pathbuffer)==false)
		{
		return false;
		}

	MFileOps fileops;
	if(fileops.Create()==false)
		{
		return false;
		}

	if(fileops.Exists(pathbuffer.GetBuffer())==false)
		{ return false; }

	//=File does exist

	pathseq.Pop();  // Pop the filename
	pathseq.GetFullPath(pathbuffer);
	dir.Create(pathbuffer);

	return true;
	}


////////////////////////////////////////////////
bool MRelativeFileStack::GetFileInLocalDirs(const char *filename
			,MString &founddir)
	{
	MPathNameList fileseq;
	if(fileseq.Create(filename)==false)
		{
		return false;
		}

	if(mLocalDirs.ReadReset()==false)
		{
		return false;
		}

	// Create FileOps Object
	MFileOps fileops;
	if(fileops.Create()==false)
		{
		return false;
		}

	MPathNameList pathseq;  const char *dir;  MBuffer pathbuffer;
	while((dir=mLocalDirs.ReadString())!=NULL)
		{
		MString dirfullpath;
		if(fileops.GetAbsolutePath(dir,dirfullpath)==false)
			{
			return false;
			}

		if(pathseq.Create(dirfullpath.Get() )==false)
			{
			return false;
			}

		// Add file seq to path seq
		if(pathseq.Push(fileseq)==false)
			{
			return false;
			}

		MBuffer pathbuffer;
		if(pathseq.GetFullPath(pathbuffer)==false)
			{
			return false;
			}

		if(fileops.Exists(pathbuffer.GetBuffer())==true)
			{
			//=We have found this file in the directory
			if(founddir.Create(dirfullpath.Get() )==false)
				{
				return false;
				}

			return true;
			}
		}

	return false;
	}


////////////////////////////////////////////////
bool MRelativeFileStack::GetFileInGlobalDirs(const char *filename
			,MString &founddir)
	{
	MPathNameList fileseq;
	if(fileseq.Create(filename)==false)
		{
		return false;
		}

	if(mGlobalDirs.ReadReset()==false)
		{
		return false;
		}

	// Create FileOps Object
	MFileOps fileops;
	if(fileops.Create()==false)
		{
		return false;
		}

	MPathNameList pathseq;  const char *dir;  MBuffer pathbuffer;
	while((dir=mGlobalDirs.ReadString())!=NULL)
		{
		if(pathseq.Create(dir)==false)
			{
			return false;
			}

		// Add file seq to path seq
		if(pathseq.Push(fileseq)==false)
			{
			return false;
			}

		MBuffer pathbuffer;
		if(pathseq.GetFullPath(pathbuffer)==false)
			{
			return false;
			}

		if(fileops.Exists(pathbuffer.GetBuffer())==true)
			{
			//=We have found this file in the directory
			if(founddir.Create(dir)==false)
				{
				return false;
				}

			return true;
			}
		}

	return false;
	}


////////////////////////////////////////////////
bool MRelativeFileStack::IsOnStackAlready(const char *directory
		,const char *filename)
	{
	MStringStackReader dirreader;
	MStringStackReader filereader;

	if(mCurrentFileStack.GetReader(filereader)==false
			|| mCurrentPathStack.GetReader(dirreader)==false)
		{
		return true;  // so that file will not be added
		}

	//Construct File Name Sequence
	MPathNameList dirlist;  dirlist.Create(directory);
	MPathNameList filelist;  filelist.Create(filename);
	dirlist.Push(filelist);

	for(;;)
		{
		const char *file=filereader.Read();
		const char *dir=dirreader.Read();
		if(file==NULL) { return false; }
		if(dir==NULL)
			{
			return true;
			}

		MPathNameList tmpdirlist;  tmpdirlist.Create(dir);
		MPathNameList tmpfilelist;  tmpfilelist.Create(file);
		tmpdirlist.Push(tmpfilelist);
		
		if(dirlist.IsSamePath(tmpdirlist)==true) { return true; }
		}
	}


////////////////////////////////////////////////
MRelativeFileStack::MRelativeFileStack(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MRelativeFileStack::~MRelativeFileStack(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MRelativeFileStack::Create(void)
	{
	Destroy();

	if(mGlobalDirs.Create()==false)
		{
		Destroy();  return false;
		}

	if(mLocalDirs.Create()==false)
		{
		Destroy();  return false;
		}

	if(mCurrentFileStack.Create()==false)
		{
		Destroy();  return false;
		}

	if(mCurrentPathStack.Create()==false)
		{
		Destroy();  return false;
		}

	if(mCurrentLocation.Create(GMaxFileDepth)==false)
		{
		Destroy(); return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MRelativeFileStack::Destroy(void)
	{
	mGlobalDirs.Destroy();
	mLocalDirs.Destroy();
	mCurrentFileStack.Destroy();
	mCurrentPathStack.Destroy();
	mCurrentLocation.Destroy();
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MRelativeFileStack::AddGlobalDir(const char *dir)
	{
	if(dir==NULL || *dir==0)
		{
		return false;
		}

	if(mGlobalDirs.AddString(dir)==false)
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MRelativeFileStack::AddLocalDir(const char *dir)
	{
	if(dir==NULL || *dir==0)
		{
		return false;
		}

	if(mLocalDirs.AddString(dir)==false)
		{
		return false;
		}

	return true;
	}


//////////////////////////////////////////////////
bool MRelativeFileStack::IsEmpty(void)					//=true if stack is empty
	{
	
	return mCurrentFileStack.IsEmpty();
	}


////////////////////////////////////////////////
//  Check if file exists in order: 1. CurrentDir 2:local dirs
//     3: Global Dirs
bool MRelativeFileStack::Push(const char *filename)		// Push a new file
	{
	MString retdir,retfile;

	// Check current directory
	if(GetFileInCurrentDir(filename,retdir,retfile)==true)
		{
		//= We have found the file in the current dir

		// Check if file is being processed now
		if(IsOnStackAlready(retdir.Get(),retfile.Get())==true)
			{  return false; }

		if(mCurrentFileStack.Push(retfile.Get())==false)
			{
			return false;
			}

		if(mCurrentPathStack.Push(retdir.Get())==false)
			{
			mCurrentFileStack.Pop();
			return false;
			}

		if(mCurrentLocation.Push(GFL_CURRENT_DIR)==false)
			{
			mCurrentPathStack.Pop();
			mCurrentFileStack.Pop();
			return false;
			}

		return true;
		}

	// Check Local Directories
	if(GetFileInLocalDirs(filename,retdir)==true)
		{
		//= We have found the file in the local directories

		// Check if file is being processed now
		if(IsOnStackAlready(retdir.Get(),filename)==true)
			{  return false; }

		if(mCurrentFileStack.Push(filename)==false)
			{
			return false;
			}

		if(mCurrentPathStack.Push(retdir.Get())==false)
			{
			mCurrentFileStack.Pop();
			return false;
			}

		if(mCurrentLocation.Push(GFL_LOCAL_DIR)==false)
			{
			mCurrentPathStack.Pop();
			mCurrentFileStack.Pop();
			return false;
			}

		return true;
		}

	
	// Check Global Directories
	if(GetFileInGlobalDirs(filename,retdir)==true)
		{
		//= We have found the file in the global directories

		// Check if file is being processed now
		if(IsOnStackAlready(retdir.Get(),filename)==true)
			{  return false; }

		if(mCurrentFileStack.Push(filename)==false)
			{
			return false;
			}

		if(mCurrentPathStack.Push(retdir.Get())==false)
			{
			mCurrentFileStack.Pop();
			return false;
			}

		if(mCurrentLocation.Push(GFL_GLOBAL_DIR)==false)
			{
			mCurrentPathStack.Pop();
			mCurrentFileStack.Pop();
			return false;
			}

		return true;
		}

	//=We have not found the file in the three locations
	return false;
	}


////////////////////////////////////////////////
bool MRelativeFileStack::Pop(void)						// Pop current file
	{
	if(mCurrentLocation.Pop()==false)
		{
		return false;
		}
	if(mCurrentPathStack.Pop()==false)
		{
		return false;
		}

	if(mCurrentFileStack.Pop()==false)
		{
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
const char *MRelativeFileStack::GetCurrentPath(void)	// Returns Full Path of current file
	{
	if(mCurrentFileStack.IsEmpty()==true)
		{
		return NULL;
		}

	return mCurrentPathStack.GetTop();
	}


//////////////////////////////////////////////
const char *MRelativeFileStack::GetFileName(void)			// Returns only filename
	{
	if(mCurrentFileStack.IsEmpty()==true)
		{
		return NULL;
		}

	return mCurrentFileStack.GetTop();
	}


////////////////////////////////////////////////
bool MRelativeFileStack::IsFileInCurrentDir(void)	//=true if file is in current dir
	{

	if(IsEmpty()==true)
		{
		return false;
		}

	if(mCurrentLocation.GetTop()==GFL_CURRENT_DIR) { return true; }

	return false;
	}

////////////////////////////////////////////////
bool MRelativeFileStack::IsFileInLocalDir(void)			//=Check if file is a local file
	{

	if(IsEmpty()==true)
		{
		return false;
		}

	if(mCurrentLocation.GetTop()==GFL_LOCAL_DIR) { return true; }

	return false;
	}


////////////////////////////////////////////////
bool MRelativeFileStack::IsFileInGlobalDir(void)		//=true if file is global file
	{

	if(IsEmpty()==true)
		{
		return false;
		}

	if(mCurrentLocation.GetTop()==GFL_GLOBAL_DIR) { return true; }

	return false;
	}


//////////////////////////////////////////////
bool MRelativeFileStack::GetCurrentFilePath(MBuffer &buffer)	// Get Current file
	{
	if(mCurrentFileStack.IsEmpty()==true) { return false; }

	MPathNameList filenamelist;
	if(filenamelist.Create(mCurrentFileStack.GetTop())==false)
		{
		return false;
		}

	MPathNameList pathnamelist;
	if(pathnamelist.Create(mCurrentPathStack.GetTop() )==false)
		{
		return false;
		}

	// Add File Name to path list
	if(pathnamelist.Push(filenamelist)==false)
		{
		return false;
		}

	if(pathnamelist.GetFullPath(buffer)==false)
		{
		return false;
		}

	return true;
	}



