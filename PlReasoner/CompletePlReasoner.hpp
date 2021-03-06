﻿//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// Compute Dung's semantics by SWI-Prolog
// -- Can tackle the following computational problems for complete semantics
// 
//=======================================================================

#ifndef PROLOG_COMPLETE_REASONER_HPP
#define PROLOG_COMPLETE_REASONER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <utility>   
#include <algorithm>   

// argumatrix
#include "config/config.hpp"
#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"
#include "dung_theory/DungAF.hpp"
#include "dung_theory/ArgumentProperty.hpp"
#include "dung_theory/AttackProperty.hpp"

// SWI-prolog
#include <SWI-cpp.h>
#include "swipl-util.hpp"
#include "PlReasoner.hpp"


namespace argumatrix{

using namespace std;

/*!
 * @class CompletePlReasoner
 * @ingroup argumatrix
 * @brief Tackle computational problems for complete semantics.
 * TODO: long description
 * @see [dung_theory/Reasoner.hpp] 
 * @author Fuan Pu
 * @version 1.0
 *
 * @date May 2016
 *
 * Contact: Pu.Fuan@gmail.com
 *
 */
class CompletePlReasoner : public PlReasoner {
public:
	CompletePlReasoner(const DungAF& daf, const string& sm_task = "CO", 
		streambuf* osbuff = std::cout.rdbuf()); 

	/**
	 * Problem [EE-$\sigma$]
	 * @brief Print all extensions (with a vector of integers {0,1,2})
	 * @param no argument
	 * @return no return.
	 */
	void task_EE();
	void task_EX();

	/**
	 * Problem [EC-$\sigma$]
	 * @brief Given an $\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>$ and 
	 * an argument $s \in \mathcal{X}$ (respectively, a set of arguments 
	 * $S\subseteq \mathcal{X}$), enumerate all sets $E\subseteq \mathcal{X}$
	 * such that $E \in \mathcal{E}_\sigma(AF)$ and $s\in E$ (respectively,
	 * $S \subseteq E$).
	 * @param set<string>, or a Boolean vector: a set of argument
	 * @return no return.
	 */
	void task_EC(const std::set<string>& argset);

	/**
	 * Problem [SC-$\sigma$]
	 * @brief Given an $\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>$ and an 
	 * argument $s \in \mathcal{X}$ (respectively, a set of arguments 
	 * $S\subseteq \mathcal{X}$), enumerate some set $E\subseteq \mathcal{X}$
	 * such that $E \in \mathcal{E}_\sigma(AF)$ and $s\in E$ (respectively, 
	 * $S \subseteq E$).
	 * @param a set of arguments.
	 * @return no return.
	 */
	void task_SC(const std::set<string>& argset);
	

	/**
	 * Problem [SE-$\sigma$]
	 * @brief Given an $\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>$, enumerate
	 * some set $E\subseteq \mathcal{X}$ that are in $\mathcal{E}_\sigma(AF)$.
	 * @param a set of arguments.
	 * @return no return.
	 */
	void task_SE();

	/**
	 * Find all extensions and store them in *m_extensions* with the form of
	 * bitvector.
	 * @param no argument.
	 * @return no return. The results are added into *m_extensions*.
	 */
	void findAllExts();


	/**
	 * Problem [DE-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and a set of arguments $S \subseteq X$. Decide whether S is
	 * a \sigma-extension of AF, i.e., S \in E_\sigma(AF).
	 * @param a set of arguments.
	 * @return true if argset is a \sigma-extension; otherwise return false.
	 */
	void task_DE(const std::set<string>& argset);

	/**
	 * Problem [DN-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and a set of arguments $S \subseteq X$. Decide whether
	 * there exist a non-empty \sigma-extension for AF
	 * @return true if there exist a non-empty \sigma-extension; otherwise return false.
	 */
	void task_DN();

	/**
	 * Problem [DC-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and an argument s \in X (respectively, a set of 
	 * arguments $S \subseteq X$). Decide whether s contained (respectively, S included)
	 * in some E \in E_\sigma(AF) (i.e., credulously justified).
	 */
	void task_DC(const std::set<string>& argset);
	
	/**
	 * Problem [DS-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and an argument s \in X (respectively, a set of 
	 * arguments $S \subseteq X$). Decide whether s contained (respectively, S included)
	 * in each E \in E_\sigma(AF) (i.e., skeptically justified).
	 */
	void task_DS(const std::set<string>& argset);
protected:
	string m_predicate;
};


CompletePlReasoner::CompletePlReasoner(
	const DungAF& daf, const string& sm_task /*= "CO"*/, 
	streambuf* osbuff /*= std::cout.rdbuf()*/) :
	PlReasoner(daf, osbuff)
{	
	if (sm_task == "CO") {   // The default predicate
		m_predicate = "complete5";
	} else
	if (sm_task == "CO1") {
		m_predicate = "complete1";
	} else
	if (sm_task == "CO2") {
		m_predicate = "complete2";
	} else
	if (sm_task == "CO3") {
		m_predicate = "complete3";
	} else
	if (sm_task == "CO4") {
		m_predicate = "complete4";
	} else
	if (sm_task == "CO5") {
		m_predicate = "complete5";
	} else {
		cerr << "Error: in CompletePlReasoner, unsupported predicate!" << endl;
		cerr << "  The supported options [CO,CO1,CO2,CO3,CO4,CO5]" << endl;
		exit(1);
	}
}

__inline
void CompletePlReasoner::task_EE()
{
	PlReasoner::printAllExts(m_predicate, getGroundedIntVector());
	// PlReasoner::printAllExts(m_predicate);
}

__inline
void CompletePlReasoner::task_EC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	PlReasoner::printAllExts(m_predicate, labelSet2IntVector(argset) );
}

__inline
void CompletePlReasoner::findAllExts()
{
	PlReasoner::findAllExts(m_predicate);
}

__inline
void CompletePlReasoner::task_DE(const std::set<string>& argset)
{
	//if(argset.empty()) {
	//	cerr << "The task DC needs additional argument (see option -a)" << endl;
	//	exit(1);
	//}

	bitvector bvec = m_daf.labelSet2bv(argset);
	if ( is_complete(bvec)) {
		m_output << STR_YES << endl;
	} else 	{
		m_output << STR_NO << endl;
	}
}


void CompletePlReasoner::task_DN()
{
	if( verifyNonemptyExt(m_predicate))	{
		m_output << STR_YES << endl;
	}
	else {
		m_output << STR_NO << endl;
	}
}

__inline
void CompletePlReasoner::task_SC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task SC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	PlReasoner::printSomeExt(m_predicate, labelSet2IntVector(argset) );
}

__inline
void CompletePlReasoner::task_SE()
{
	PlReasoner::printSomeExt(m_predicate, getGroundedIntVector() );
}

__inline
void CompletePlReasoner::task_DC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	if ( isCredulouslyJustified(m_predicate, argset) ) {
		m_output << STR_YES << endl;
	} else {
		m_output << STR_NO << endl;
	}
}

__inline
void CompletePlReasoner::task_DS(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	if ( isSkepticallyJustified(m_predicate, argset) ) {
		m_output << STR_YES << endl;
	} else {
		m_output << STR_NO << endl;
	}
}

inline
void CompletePlReasoner::task_EX()
{
	fetchAllExts(m_predicate);
}


} // namespace argumatrix


#endif  // PROLOG_COMPLETE_REASONER_HPP