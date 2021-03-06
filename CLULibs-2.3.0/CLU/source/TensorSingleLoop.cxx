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

#include "TensorSingleLoop.h"

////////////////////////////////////////////////////////////////////////////////////
/// Single Loop Class Member Functions
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
/// Constructor

template <class CType>
CTensorSingleLoop<CType>::CTensorSingleLoop()
{
	m_pTIdx = 0;
}


////////////////////////////////////////////////////////////////////////////////////
/// Set Tensor Idx Instance

template <class CType>
void CTensorSingleLoop<CType>::Set(CTensorIdx<CType>& rT) 
	throw (CCLUException)
{
	if (!rT.IsValid())
	{
		m_pTIdx = 0;
		m_mFreeIdx.Set(0);
		throw InvalidTensorIdxRef();
	}

	m_pTIdx = &rT;
	m_mFreeIdx.Set(0);

	const typename CTensorIdx<CType>::TFreeIdxMap& mapFreeIdx = rT.GetFreeIdxMap();
	typename CTensorIdx<CType>::TFreeIdxMap::const_iterator it_El, it_End;

	it_End = mapFreeIdx.end();

	// Store free indices in the order they appear in the map
	for (it_El = mapFreeIdx.begin();
		 it_El != it_End;
		 ++it_El)
	{
		m_mFreeIdx.Add(1);
		m_mFreeIdx.Last() = it_El->first;
	}
}


////////////////////////////////////////////////////////////////////////////////////
// Get number of steps at particular loop level.

template <class CType>
int CTensorSingleLoop<CType>::StepCount(int iLevel)
	throw (CCLUException)
{
	if (iLevel < 0 || iLevel >= int(m_mFreeIdx.Count()))
		throw InvalidLoopLevel();

	if (!m_pTIdx)
		throw InvalidTensorIdxRef();

	return m_pTIdx->GetStepCount(m_mFreeIdx[iLevel]);
/*
	const CTensorIdx<CType>::TFreeIdxMap &rMap = m_pTIdx->GetFreeIdxMap();
	CTensorIdx<CType>::TFreeIdxMap::const_iterator it_El;

	it_El = rMap.find(m_mFreeIdx[iLevel]);

	const CTensorIdx<CType>::SFreeIdxData &rIdx = it_El->second;

	return (rIdx.iMax - rIdx.iMin + 1);
*/
}


////////////////////////////////////////////////////////////////////////////////////
// Initialize Loops. Returns number of cascaded loops.

template <class CType>
int CTensorSingleLoop<CType>::Init() 
	throw (CCLUException)
{
	if (!m_pTIdx)
		throw InvalidTensorIdxRef();
	
	m_pTIdx->InitAllLoops();
	
	return m_mFreeIdx.Count();
}



////////////////////////////////////////////////////////////////////////////////////
// Step loop. Returns loop level, in which step was made.
// Loop level starts at zero for most inner loop.
// If loop level is -1, all loops are finished.
// No exceptions generated by this function!

template <class CType>
int CTensorSingleLoop<CType>::Step() 
{
	int iLevel = 0;

	if (m_mFreeIdx.Count() == 0)
		return -1;

	while (!m_pTIdx->StepLoop(m_mFreeIdx[iLevel]))
	{
		m_pTIdx->InitLoop(m_mFreeIdx[iLevel]);
		if (iLevel == m_mFreeIdx.Count() - 1)
			return -1;	// All loops finished

		++iLevel;
	}

	return iLevel;
}
