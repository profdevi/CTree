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


//v1.7 copyright Comine.com 20150813R1130
#ifndef MCTokenReader_h
#define MCTokenReader_h

///////////////////////////////////////////////////
#include "MIntQueue.h"
#include "MDynamicString.h"


//******************************************************
//**  MCTokenReaderType Enumeration
//******************************************************
enum MCTokenReaderType
	{
	MCTRT_UNSET=0,				// Text Type has never been set
	MCTRT_DONE=1,
	MCTRT_SYMBOL=2,
	MCTRT_OPERATOR=3,
	MCTRT_DQSTRING=4,
	MCTRT_SQSTRING=5,
	MCTRT_COMMENT_BEGIN=6,
	MCTRT_COMMENT=7,
	MCTRT_COMMENT_END=8,
	MCTRT_SPACE=9,
	MCTRT_LINECOMMENT=12,
	MCTRT_NEWLINE=13
	};


//******************************************************
//**  MCTokenReader class
//******************************************************
class MCTokenReader
	{
	////////////////////////////////////////////////
	MIntQueue mQueue;					// Holds a queue of character input
	int mState;							// Holds Current State of of stream
	MDynamicString mLexeme;				// Holds the current lexeme value
	MCTokenReaderType mLexemeType;		// Holds the current lexeme type
	bool (*mStreamInput)(int &inp,void *extra);		// Holds pointer to stream function
	void *mExtraArg;					// Extra argument passed to callback
	bool mStreamInputDone;				// =true if input is closed
	
	////////////////////////////////////////////////
	void ClearObject(void);
	bool MoveNextState(int inp);	// State Transitions

	////////////////////////////////////////////////
	public:
	MCTokenReader(void);
	~MCTokenReader(void);
	bool Create(bool (*streaminput)(int &inp,void *extra),void *extra=0);
	bool Destroy(void);
	bool Read(void);							// Read the next token
	const char *GetLexeme(void);				// Get the current lexeme
	MCTokenReaderType GetLexemeType(void);		// Get the current lexeme type
	};

#endif // MCTokenReader_h

