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
#ifndef MIntQueue_h
#define MIntQueue_h

//******************************************************
//**  MIntQueue class
//******************************************************
class MIntQueue
	{
	////////////////////////////////////////////////
	int *mArray;			// holds a memory block
	int mSize;				// current queue size
	int mMaxSize;			// Maximum Queue size capacity	
	int mFront;				// index of the first item
	int mBack;				// index of the next element
	
	////////////////////////////////////////////////
	void ClearObject(void);

	////////////////////////////////////////////////
	public:
	MIntQueue(void);
	MIntQueue(int maxsize);
	~MIntQueue(void);
	bool Create(int maxsize);	// Construct the object state
	bool Destroy(void);			// Destroy the object state
	bool Clear(void);			// Clear the queue
	int GetFront(void);			// Get the Front item
	int GetBack(void);			// Get the last item enqueued
	int GetSize(void);			// Get the current queue size
	int GetMaxSize(void);		// Get the maximum capacity of queue
	bool Enqueue(int value);	// Enqueue a new element
	bool EnqueueFront(int value);	// Push back item in front
	bool Dequeue(void);			// Dequeue a new element
	bool DequeueBack(void);		// Dequeue the back item
	bool IsEmpty(void);			// =true if queue is empty
	bool IsFull(void);			// =true if queue is full
	int Get(int index);			// index=(0...mSize-1), 0 is Front
	};

#endif // MIntQueue_h

