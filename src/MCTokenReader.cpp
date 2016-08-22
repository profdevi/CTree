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
#include "MStdLib.h"
#include "MIntQueue.h"
#include "MDynamicString.h"
#include "MCTokenReader.h"


//******************************************************
//**  Module Elements
//******************************************************
static int GQueueSize=128;		// Only need two chars
static int GLexemeSize=128;		// Holds the workable lexeme size

enum GState{ S_START=0,S_SLASH=1,S_SLASH_SLASH=2,S_SLASH_STAR=3
		,S_SLASH_STAR_STAR=4,S_DQUOTE=5,S_SQUOTE=6,S_SPACE=7,S_SYMBOL=8
		,S_DQUOTE_BACKSLASH=9,S_SQUOTE_BACKSLASH=10};


///////////////////////////////////////////////////////////////
static bool GIsSpace(char ch)
	{
	if(ch==' ' || ch=='\t') { return true; }
	return false;
	}


///////////////////////////////////////////////////////////////
static bool GIsSymbol(char ch)
	{
	if(MStdIsAlphaNumeric(ch)!=0 || ch=='_') { return true; }
	return false;	
	}


//******************************************************
//**  MCTokenReader class
//******************************************************
void MCTokenReader::ClearObject(void)
	{
	mState=S_START;
	mStreamInput=NULL;
	mExtraArg=NULL;
	mLexemeType=MCTRT_UNSET;
	mStreamInputDone=false;
	}


///////////////////////////////////////////////////////////////
bool MCTokenReader::MoveNextState(int inp)
	{
	if(mState==S_START)	// Start State ///////////////////////
		{
		if(inp=='/')
			{
			mLexeme.Insert(inp);
			mState=S_SLASH;  return true;
			}
		if(inp=='\"')
			{
			mLexeme.Insert(inp);
			mState=S_DQUOTE; return true;
			}
		if(inp=='\'')
			{
			mLexeme.Insert(inp);
			mState=S_SQUOTE; return true;
			}
		if(inp=='\n')
			{
			mLexeme.Insert('\n');
			mLexemeType=MCTRT_NEWLINE;  return true;
			}
		if(GIsSpace(inp)==true)
			{
			mLexeme.Insert(inp);
			mState=S_SPACE;  return true;
			}
		if(GIsSymbol(inp)==true)
			{
			mLexeme.Insert(inp);
			mState=S_SYMBOL;  return true;
			}
		if(inp==0){  return true;  }
		
		mLexeme.Insert((char)inp);
		mLexemeType=MCTRT_OPERATOR;  return true;
		}
	else if(mState==S_SLASH) /////////////////////////////////////////////
		{
		if(inp=='/')
			{
			mLexeme.Insert(inp);
			mState=S_SLASH_SLASH;  return true;
			}
		if(inp=='*')
			{
			mLexeme.Insert(inp);
			mLexemeType=MCTRT_COMMENT_BEGIN;
			mState=S_SLASH_STAR; return true;
			}

		mLexemeType=MCTRT_OPERATOR;
		mState=S_START;  return false;
		}
	else if(mState==S_SLASH_SLASH)
		{
		if(inp=='\n')
			{
			mLexemeType=MCTRT_LINECOMMENT;
			mState=S_START;  return false;
			}
		if(inp==0)
			{
			mLexemeType=MCTRT_LINECOMMENT;
			mState=S_START;  return true;
			}
		
		mLexeme.Insert(inp);  return true;
		}
	else if(mState==S_SLASH_STAR)
		{
		if(inp=='*')
			{  mState=S_SLASH_STAR_STAR; return true;  }
		if(inp=='\n')
			{
			if(mLexeme.GetLength()==0)
				{
				mLexeme.Insert('\n');
				mLexemeType=MCTRT_NEWLINE;
				return true;
				}

			mLexemeType=MCTRT_COMMENT;  return false;
			}
		if(inp==0)
			{  mState=S_START;  return true;  }


		mLexeme.Insert(inp);  return true;
		}
	else if(mState==S_SLASH_STAR_STAR)
		{
		if(inp=='*')
			{  mLexeme.Insert('*');  return true;  }

		if(inp=='/')
			{
			if(mLexeme.GetLength()==0)
				{
				mLexeme.Insert('*');
				mLexeme.Insert('/');
				mLexemeType=MCTRT_COMMENT_END;
				mState=S_START;	 return true;
				}

			mLexemeType=MCTRT_COMMENT;  return false;
			}

		if(inp=='\n')
			{
			mLexeme.Insert('*');
			mLexemeType=MCTRT_COMMENT;
			mState=S_SLASH_STAR;  return false;
			}

		if(inp==0)
			{
			mLexeme.Insert('*');
			mLexemeType=MCTRT_COMMENT;
			mState=S_START;  return false;			
			}

		mLexeme.Insert(inp);  mState=S_SLASH_STAR;  return true;
		}
	else if(mState==S_DQUOTE)
		{
		if(inp=='\"')
			{
			mLexeme.Insert(inp);
			mLexemeType=MCTRT_DQSTRING;
			mState=S_START;  return true;
			}
		if(inp=='\n' )  // INput has Mistake in double quotes!!!
			{
			mLexemeType=MCTRT_DQSTRING;
			mState=S_START;   return false;
			}
		if(inp=='\\')
			{
			mLexeme.Insert(inp);
			mState=S_DQUOTE_BACKSLASH; return true;
			}
		if(inp==0)
			{
			mLexemeType=MCTRT_DQSTRING;
			mState=S_START;   return true;
			}

		mLexeme.Insert(inp);  return true;
		}
	else if(mState==S_SQUOTE)
		{
		if(inp=='\'')
			{
			mLexeme.Insert(inp);
			mLexemeType=MCTRT_SQSTRING;
			mState=S_START;  return true;
			}
		if(inp=='\n' )  // Input has Mistake in single quotes!!!
			{
			mLexemeType=MCTRT_SQSTRING;
			mState=S_START;   return false;
			}
		if(inp=='\\')
			{
			mLexeme.Insert(inp);
			mState=S_SQUOTE_BACKSLASH; return true;
			}
		if(inp==0)
			{
			mLexemeType=MCTRT_SQSTRING;
			mState=S_START;  return true;
			}

		mLexeme.Insert(inp);  return true;
		}
	else if(mState==S_SPACE)
		{
		if(GIsSpace(inp)==true)
			{  mLexeme.Insert(inp);  return true; }

		mLexemeType=MCTRT_SPACE;  mState=S_START; return false;
		}
	else if(mState==S_SYMBOL)
		{
		if(GIsSymbol(inp)==true)
			{  mLexeme.Insert(inp);  return true; }

		mLexemeType=MCTRT_SYMBOL;
		mState=S_START; return false;
		}
	else if(mState==S_DQUOTE_BACKSLASH)
		{
		if(inp=='\n') // Mistake in escape character
			{
			mLexemeType=MCTRT_DQSTRING;
			mState=S_START;   return false;
			}

		if(inp==0)
			{
			mLexemeType=MCTRT_DQSTRING;
			mState=S_START;  return true;
			}

		mLexeme.Insert(inp);
		mState=S_DQUOTE; return true;
		}
	else if(mState==S_SQUOTE_BACKSLASH)
		{
		if(inp=='\n')	// Mistake in escape character
			{
			mLexemeType=MCTRT_SQSTRING;
			mState=S_START;   return false;
			}

		if(inp==0)
			{
			mLexemeType=MCTRT_SQSTRING;
			mState=S_START;  return true;
			}

		mLexeme.Insert(inp);  mState=S_SQUOTE; return true;
		}
	else
		{
		return false;
		}
	}


////////////////////////////////////////////////
MCTokenReader::MCTokenReader(void)
	{  ClearObject();  }


////////////////////////////////////////////////
MCTokenReader::~MCTokenReader(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MCTokenReader::Create(bool (*streaminput)(int &inp,void *extra),void *extra)
	{
	Destroy();

	///////////////////////////////////////////
	if(streaminput==NULL)
		{
		return false;
		}

	// Set Stream input function
	mStreamInput=streaminput; mExtraArg=extra;

	// Create queue
	if(mQueue.Create(GQueueSize)==false)
		{
		Destroy();  return false;
		}

	// create lexeme buffer
	if(mLexeme.Create(GLexemeSize)==false)
		{
		Destroy();  return false;
		}
	
	return true;
	}


////////////////////////////////////////////////
bool MCTokenReader::Destroy(void)
	{
	mLexeme.Destroy();
	mQueue.Destroy();
	ClearObject();
	return true;
	}


////////////////////////////////////////////////
bool MCTokenReader::Read(void)							// Read the next token
	{
	if(mQueue.IsEmpty()==true && mStreamInputDone==true) { return false; }

	mLexemeType=MCTRT_UNSET;  mLexeme.Clear();
	while(mLexemeType==MCTRT_UNSET)
		{
		// Read atleast two tokens aheads of stream
		while(mQueue.GetSize()<2 && mStreamInputDone==false)
			{	int ch;
			// Read in from stream input
			if((*mStreamInput)(ch,mExtraArg)==true)
				{ 
				if(ch=='\r') { continue; }
				mQueue.Enqueue(ch);
				}
			else
				{ mStreamInputDone=true;  }
			}

		//= We now have at most two tokens ahead

		// Skip "\\\n"  sequences
		if(mQueue.GetSize()>=2 && mQueue.Get(0)=='\\'
				&&  mQueue.Get(1)=='\n')
			{
			mQueue.Dequeue();  mQueue.Dequeue();
			continue;
			}

		// If Queue is Empty, we are done
		if(mQueue.IsEmpty()==true) {  return false;  }

		// Get Next Queue Member
		int nextchar=mQueue.GetFront();
		mQueue.Dequeue();

		// Process the character
		if(MoveNextState(nextchar)==false)
			{  mQueue.EnqueueFront(nextchar);  }
		}

	return true;
	}


////////////////////////////////////////////////
const char *MCTokenReader::GetLexeme(void)				// Get the current lexeme
	{
	return mLexeme.Get();
	}


////////////////////////////////////////////////
MCTokenReaderType MCTokenReader::GetLexemeType(void)			// Get the current lexeme type
	{
	return mLexemeType;
	}


