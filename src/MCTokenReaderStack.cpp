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


//v1.4 copyright Comine.com 20150813R1310
#include "MStdLib.h"
#include "MFile.h"
#include "MCTokenReader.h"
#include "MFileOps.h"
#include "MCTokenReaderStack.h"


//******************************************************
//**  Module Elements
//******************************************************
static bool GCharInput(int &ch,void *extra)
	{
	MFileInput *fileinput=(MFileInput *)extra;
	if(fileinput==NULL)
		{
		return false;
		}

	char inpch=0;
	for(;;)
		{
		if(fileinput->Read(inpch)==false) { ch=0;  return false; }

		// Skip '\r'
		if(inpch=='\r') { continue; }

		ch=inpch;  return true;
		}
	}

//******************************************************
//**  MCTokenReaderStack class
//******************************************************
void MCTokenReaderStack::ClearObject(void)
	{
	mFileArray=NULL;
	mLexerArray=NULL;
	mCount=0;
	mMaxCount=0;
	}


////////////////////////////////////////////////
MCTokenReaderStack::MCTokenReaderStack(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MCTokenReaderStack::MCTokenReaderStack(int maxsize)
	{
	ClearObject();
	if(Create(maxsize)==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MCTokenReaderStack::~MCTokenReaderStack(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MCTokenReaderStack::Create(int maxsize)
	{
	Destroy();
	
	/// Allocate array of mFile Objects
	mFileArray=new MFileInput[maxsize];
	if(mFileArray==NULL)
		{
		Destroy(); return false;
		}

	mLexerArray=new MCTokenReader[maxsize];
	if(mLexerArray==NULL)
		{
		Destroy();  return false;
		}

	// Update data members
	mMaxCount=maxsize;  mCount=0;
	return true;
	}


////////////////////////////////////////////////
bool MCTokenReaderStack::Destroy(void)
	{
	if(mFileArray!=NULL)
		{
		delete [] mFileArray;
		}

	if(mLexerArray!=NULL)
		{
		delete [] mLexerArray;
		}

	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MCTokenReaderStack::Read(void)				// Read the next token
	{
	if(mCount<=0)
		{
		return false;
		}

	return mLexerArray[mCount-1].Read();
	}


////////////////////////////////////////////////
const char *MCTokenReaderStack::GetLexeme(void)				// Get the current lexeme
	{
	if(mCount<=0)
		{
		return false;
		}

	return mLexerArray[mCount-1].GetLexeme();
	}


////////////////////////////////////////////////
MCTokenReaderType MCTokenReaderStack::GetLexemeType(void)	// Get the current lexeme type
	{
	if(mCount<=0)
		{
		return MCTRT_UNSET;
		}

	return mLexerArray[mCount-1].GetLexemeType();	
	}


///////////////////////////////////////////////////
bool MCTokenReaderStack::IsEmpty(void)
	{
	if(mCount<=0) { return true; }
	return false;
	}


///////////////////////////////////////////////////
bool MCTokenReaderStack::IsFull(void)
	{
	if(mCount>=mMaxCount) { return true; }
	return false;
	}


///////////////////////////////////////////////////
bool MCTokenReaderStack::Push(const char *filename)			// Push a new filename
	{
	//Check if stack is full
	if(IsFull()==true)
		{
		return false;
		}


	MFileOps fileops;
	if(fileops.Create()==false)
		{
		return false;
		}

	// Check if file exists
	if(fileops.Exists(filename)==false)
		{
		return false;
		}

	// Check if file is text
	if(fileops.IsBinary(filename)==true)
		{
		return false;
		}

	// Start Opening up the file
	if(mFileArray[mCount].Create(filename)==false)
		{
		return false;
		}

	if(mLexerArray[mCount].Create(GCharInput,mFileArray+mCount)==false)
		{
		mFileArray[mCount].Destroy();
		mLexerArray[mCount].Destroy();
		return false;
		}

	mCount = mCount + 1;
	return true;
	}


///////////////////////////////////////////////////
bool MCTokenReaderStack::Pop(void)					// Pop the last file
	{
	if(IsEmpty()==true)
		{
		return false;
		}
	mLexerArray[mCount-1].Destroy(); // Close the lexer object
	mFileArray[mCount-1].Destroy();  // CLose the file
	mCount=mCount-1;				// Point to the last top
	return true;
	}



