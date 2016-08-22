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


//v1.8 copyright Comine.com 20150813R1127
#include "MStdLib.h"
#include "MIntQueue.h"


//******************************************************
//**  MIntQueue class
//******************************************************
void MIntQueue::ClearObject(void)
	{
	mArray=NULL;
	mSize=0;
	mMaxSize=0;
	mFront=0;
	mBack=0;
	}


//////////////////////////////////////////////////////
MIntQueue::MIntQueue(void)
	{  ClearObject();  }


//////////////////////////////////////////////////////
MIntQueue::MIntQueue(int maxsize)
	{
	ClearObject();
	if(Create(maxsize)==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MIntQueue::~MIntQueue(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MIntQueue::Create(int queuesize)
	{
	Destroy();

	if(queuesize<=0)
		{
		Destroy();  return false;
		}
	
	mArray=new int[queuesize];
	if(mArray==NULL)
		{
		Destroy();
		return false;
		}

	mFront=mBack=0;
	mMaxSize=queuesize;
	mSize=0;  
	MStdMemZero(mArray,sizeof(int)*mMaxSize);
	return true;
	}


////////////////////////////////////////////////
bool MIntQueue::Destroy(void)
	{
	if(mArray!=NULL)
		{
		delete[] mArray;
		mArray=NULL;
		}

	ClearObject();
	return true;
	}


///////////////////////////////////////////////
bool MIntQueue::Clear(void)			// Clear the queue
	{
	MStdMemSet(mArray,0,sizeof(int)*mMaxSize);
	mFront=mBack=0;  mSize=0;
	return true;
	}


////////////////////////////////////////////////
int MIntQueue::GetFront(void)			// Get the Front item
	{
	return mArray[mFront];
	}


////////////////////////////////////////////////
int MIntQueue::GetBack(void)			// Get the last item enqueued
	{
	return mArray[(mBack+mMaxSize-1)%mMaxSize];
	}

////////////////////////////////////////////////
int MIntQueue::GetSize(void)			// Get the current queue size
	{
	return mSize;
	}


////////////////////////////////////////////////
int MIntQueue::GetMaxSize(void)			// Get the maximum capacity of queue
	{
	return mMaxSize;
	}


////////////////////////////////////////////////
bool MIntQueue::Enqueue(int value)	// Enqueue a new element
	{
	if(mSize==mMaxSize)
		{
		return false;
		}
	
	mArray[mBack]=value;  mBack=(mBack+1)%mMaxSize;  mSize += 1;
	return true;
	}


////////////////////////////////////////////////
bool MIntQueue::EnqueueFront(int value)	// Push back item in front
	{
	if(mSize==mMaxSize)
		{
		return false;
		}

	mFront=(mFront-1+mMaxSize)%mMaxSize;  mSize+=1;
	mArray[mFront]=value;  return true;
	}


////////////////////////////////////////////////
bool MIntQueue::Dequeue(void)			// Dequeue a new element
	{
	if(mSize==0)
		{
		return false;
		}

	mFront=(mFront+1)%mMaxSize;  mSize -= 1; 
	return true;
	}


/////////////////////////////////////////////////
bool MIntQueue::DequeueBack(void)		// Dequeue the back item
	{
	if(mSize==0)
		{
		return false;
		}

	mBack=(mBack-1+mMaxSize)%mMaxSize;  mSize -= 1;
	return true;
	}


////////////////////////////////////////////////
bool MIntQueue::IsEmpty(void)			// =true if queue is empty
	{
	if(mSize==0) { return true; }
	return false;
	}


////////////////////////////////////////////////
bool MIntQueue::IsFull(void)			// =true if queue is full
	{
	if(mSize==mMaxSize) { return true; }
	return false;
	}


////////////////////////////////////////////////
int MIntQueue::Get(int index)
	{
	if(index<0 || index>=mSize)
		{
		return 0;
		}

	return mArray[(mFront+index)%mMaxSize];
	}


