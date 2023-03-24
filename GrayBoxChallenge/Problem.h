#pragma once
#include <string>
#include <vector>
using namespace std;

const int I_NUMBER_OF_VARIABLES_IN_CLAUSE = 3;

class CProblem
{
public:
	CProblem();	
	virtual ~CProblem();

	bool bCheckNumberOfClauses(string sSourcePath);
	bool bLoad(string sSourcePath);
	void vAssignClausesToVariables();

	bool dEvaluateSingleClause(int iWhichClause, int* pdGenotype);

	int iCheckClausesWithVariable(int iVariable, int* pdGenotype);
	int iCompute(int* pdGenotype);
	double dEvaluate(int *pdGenotype);

	int iGetNumberVariables();
	void vPrintClauses();
	void vPrintClausesInfo();

	bool b_dealloc_table_2_dim_int(int*** piTable, int iSizeX, int iSizeY);
	bool b_dealloc_table_2_dim_bool(bool*** pbTable, int iSizeX, int iSizeY);

protected:
	int i_number_of_fulfilled_clauses;
	int i_number_of_clauses;
	int i_number_of_variables;
	int** v_clauses;
	bool** v_signs;
	vector<int>* i_variables_with_their_clauses;

};//class CProblem


