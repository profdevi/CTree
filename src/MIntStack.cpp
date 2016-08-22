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


//v1.6 copyright Comine.com 20150816U2148
#include "MStdLib.h"
#include "MIntStack.h"


//******************************************************
//**  MIntStack class
//******************************************************
void MIntStack::ClearObject(void)
	{
	mArray=NULL;
	mMaxSize=0;
	mTop=-1;
	}


////////////////////////////////////////////////
MIntStack::MIntStack(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MIntStack::MIntStack(int size)
	{
	ClearObject();

	if(Create(size)==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MIntStack::~MIntStack(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MIntStack::Create(int maxsize)
	{
	Destroy();
	mArray=new int[maxsize];
	if(mArray==NULL)
		{
		return false;
		}

	mMaxSize=maxsize;
	return true;
	}


////////////////////////////////////////////////
bool MIntStack::Destroy(void)
	{
	if(mArray!=NULL)
		{
		delete[] mArray;
		}

	ClearObject();
	return true;
	}


///////////////////////////////////////////////
bool MIntStack::Clear(void)
	{
	mTop=-1;
	return true;
	}


///////////////////////////////////////////////
bool MIntStack::Push(int element)
	{
	if(mTop==mMaxSize-1)
		{
		return false;
		}
	
	mTop += 1;
	mArray[mTop]=element;
	return true;
	}


///////////////////////////////////////////////
bool MIntStack::Pop(void)
	{
	if(mTop==-1)
		{
		return false;
		}

	mTop -= 1;
	return true;
	}


///////////////////////////////////////////////
int MIntStack::GetTop(void)
	{
	if(mTop==-1)
		{
		return 0;
		}

	return mArray[mTop];
	}


///////////////////////////////////////////////
int MIntStack::GetSize(void)
	{
	return mTop+1;
	}


///////////////////////////////////////////////
int MIntStack::GetMaxSize(void)
	{
	return mMaxSize;
	}


///////////////////////////////////////////////
bool MIntStack::IsEmpty(void)
	{
	if(mTop==-1)
		{ return true; }

	return false;
	}


///////////////////////////////////////////////
bool MIntStack::IsFull(void)
	{
	if(mTop>=mMaxSize-1)
		{ return true; }

	return false;
	}


//////////////////////////////////////////////
int MIntStack::Get(int index)
	{
	return mArray[index];
	}


