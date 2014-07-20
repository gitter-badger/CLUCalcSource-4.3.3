/// LICENSE START
////////////////////////////////////////////////////////////////////////////////////
//
// This file is part of the CLU Library.
//
// 
// Copyright (C) 1997-2004  Christian B.U. Perwass
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The GNU Library General Public License can be found in the file
// license.txt distributed with this library.
//
// Please send all queries and comments to
//
// email: help@clucalc.info
// mail: Institut fuer Informatik, Olshausenstr. 40, 24098 Kiel, Germany.
//
////////////////////////////////////////////////////////////////////////////////////
/// LICENSE END

#include "TensorPointLoop.h"

////////////////////////////////////////////////////////////////////////////////////
/// Single Loop Class Member Functions
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
/// Constructor

template <class CType>
CTensorPointLoop<CType>::CTensorPointLoop()
{
}


////////////////////////////////////////////////////////////////////////////////////
/// Set Tensor Idx Instance

template <class CType>
void CTensorPointLoop<CType>::Set(CTensorData<CType>& rTResult, CTensorIdx<CType>& rTLeft, CTensorIdx<CType>& rTRight) 
	throw (CCLUException)
{
	Mem<int> mDim, mIdx;
	int iLoopCount, iLoop, iIdx, iDim;

	m_Loop.Set(rTLeft, rTRight);
	iLoopCount = m_Loop.Init();
	mDim.Set(iLoopCount);
	mIdx.Set(iLoopCount);

	for(iLoop = iLoopCount-1, iIdx = -1, iDim = 0; iLoop >= 0; iLoop--, iDim++)
	{
		mDim[iDim] = m_Loop.StepCount(iLoop);
		mIdx[iDim] = iIdx--;
	}

	rTResult.Reset(mDim);
	m_ResultTIdx.Set(rTResult, mIdx);
	m_ResultLoop.Set(m_ResultTIdx);
}




////////////////////////////////////////////////////////////////////////////////////
// Initialize Loops. Returns number of cascaded loops.

template <class CType>
int CTensorPointLoop<CType>::Init() 
	throw (CCLUException)
{
	m_ResultLoop.Init();

	return 	m_Loop.Init();
}



////////////////////////////////////////////////////////////////////////////////////
// Step loop. Returns loop level, in which step was made.
// Loop level starts at zero for most inner loop.
// If loop level is -1, all loops are finished.
// No exceptions generated by this function!

template <class CType>
int CTensorPointLoop<CType>::Step() 
{
	int iLevel = m_Loop.Step();

	if (iLevel < 0)
		return -1;

	m_ResultLoop.Step();

	return iLevel;
}

