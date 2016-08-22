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


//v1.11 copyright Comine.com 20150818T0511
#ifndef MStringStack_h
#define MStringStack_h


////////////////////////////////////////////////////
struct MStringStackNode
	{
	MStringStackNode *Next;
	char *Value;
	};

//******************************************************
//**  MStringStack class
//******************************************************
class MStringStackReader;
class MStringStack
	{
	////////////////////////////////////////////////
	MStringStackNode *mList;				// List of Nodes
	int mCount;					// Number of Elements in stack
	
	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MStringStack(bool create=false);
	~MStringStack(void);
	bool Create(void);
	bool Destroy(void);
	bool Push(const char *string);		// Push an item onto stack
	bool Pop(void);						// Pop an item off stack
	const char *GetTop(void);			// Get Top Item in Stack
	bool IsEmpty(void);					// Check if stack is empty
	bool IsElement(const char *string);	// Check if element is of set
	bool Reverse(void);					// Reverse Stack Elements
	int GetCount(void);					// Returns Element count
	int GetElementPos(const char *str);	// =-1 on failure, 0(Top of stack)
	bool GetReader(MStringStackReader &reader);		// Init Reader
	};


/////////////////////////////////////////////////
class MStringStackReader
	{
	const MStringStackNode *mList;

	//////////////////////////////////////////////
	void ClearObject(void);

	//////////////////////////////////////////////
	public:
	MStringStackReader(void);
	~MStringStackReader(void);
	bool Create(const MStringStackNode *list);
	bool Destroy(void);
	const char *Read(void);
	};

#endif // MStringStack_h

