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
#ifndef MCTokenReaderStack_h
#define MCTokenReaderStack_h

//////////////////////////////////////////////////
#include "MFile.h"
#include "MCTokenReader.h"

//******************************************************
//**  MCTokenReaderStack class
//******************************************************
class MCTokenReaderStack
	{
	////////////////////////////////////////////////
	MFileInput *mFileArray;
	MCTokenReader *mLexerArray;
	int mCount;
	int mMaxCount;

	////////////////////////////////////////////////
	void ClearObject(void);
	
	////////////////////////////////////////////////
	public:
	MCTokenReaderStack(void);
	MCTokenReaderStack(int maxsize);
	~MCTokenReaderStack(void);
	bool Create(int maxlength);
	bool Destroy(void);

	// Implement same functions as MCTokenReader
	bool Read(void);							// Read the next token
	const char *GetLexeme(void);				// Get the current lexeme
	MCTokenReaderType GetLexemeType(void);		// Get the current lexeme type

	// Stack Operations
	bool IsEmpty(void);
	bool IsFull(void);
	bool Push(const char *filename);			// Push a new filename
	bool Pop(void);								// Pop the last file
	};

#endif // MCTokenReaderStack_h

