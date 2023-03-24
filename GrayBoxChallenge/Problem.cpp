#include "Problem.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <stdexcept>

CProblem::CProblem()
{
	i_number_of_clauses = 0;
	i_number_of_variables = 0;
	i_number_of_fulfilled_clauses = 0;
}

CProblem::~CProblem()
{
	b_dealloc_table_2_dim_int(&v_clauses, i_number_of_clauses, I_NUMBER_OF_VARIABLES_IN_CLAUSE);
	b_dealloc_table_2_dim_bool(&v_signs, i_number_of_clauses, I_NUMBER_OF_VARIABLES_IN_CLAUSE);

	delete [] i_variables_with_their_clauses;
}

bool CProblem::b_dealloc_table_2_dim_int(int*** piTable, int iSizeX, int iSizeY)
{
	if (iSizeX <= 0 || iSizeY <= 0)
	{
		return false;
	}

	for (int i = 0; i < iSizeX; i++)
	{
		delete[](*piTable)[i];
	}

	delete[](*piTable);

	return true;
}

bool CProblem::b_dealloc_table_2_dim_bool(bool*** pbTable, int iSizeX, int iSizeY)
{
	if (iSizeX <= 0 || iSizeY <= 0)
	{
		return false;
	}

	for (int i = 0; i < iSizeX; i++)
	{
		delete[](*pbTable)[i];
	}

	delete[](*pbTable);

	return true;
}


bool CProblem::bCheckNumberOfClauses(string sSourcePath)
{
	string s_line;
	ifstream file_to_read;

	try
	{
		file_to_read.open(sSourcePath + ".txt");

		if (file_to_read.is_open())
		{
			while (getline(file_to_read, s_line))
			{
				i_number_of_clauses++;
			}

			file_to_read.close();
		}

		return true;
	}
	catch (const ifstream::failure& e)
	{
		cout << "\nCannot open file with problem";

		return false;
	}
}

bool  CProblem::bLoad(string sSourcePath) 
{ 
	if (bCheckNumberOfClauses(sSourcePath) == false)
	{
		return false;
	}

	v_clauses = new int* [i_number_of_clauses];
	v_signs = new bool* [i_number_of_clauses];

	string s_word;
	ifstream file_to_read;

	try
	{
		file_to_read.open(sSourcePath + ".txt");
	
		if (file_to_read.is_open())
		{
			for(int i = 0; i < i_number_of_clauses; i++)
			{
				file_to_read >> s_word;

				v_clauses[i] = new int[I_NUMBER_OF_VARIABLES_IN_CLAUSE];
				v_signs[i] = new bool[I_NUMBER_OF_VARIABLES_IN_CLAUSE];

				for(int j = 0; j < I_NUMBER_OF_VARIABLES_IN_CLAUSE; j++)
				{
					file_to_read >> s_word;

					if (s_word[0] != '-')
					{
						v_clauses[i][j] = stoi(s_word);
						v_signs[i][j] = true;
					}
					else
					{
						v_clauses[i][j] = (-1) * stoi(s_word);
						v_signs[i][j] = false;
					}

					if (v_clauses[i][j] > i_number_of_variables)
					{
						i_number_of_variables = v_clauses[i][j];
					}
				}

				file_to_read >> s_word;
			}

			vPrintClausesInfo();

			file_to_read.close();
		}

		i_number_of_variables++;
		vAssignClausesToVariables();

		return true;
	}
	catch (const ifstream::failure& e)
	{
		cout << "\nCannot open file with problem";

		return false;
	}
}

void CProblem::vAssignClausesToVariables()
{
	i_variables_with_their_clauses = new vector<int>[i_number_of_variables];

	for (int i = 0; i < i_number_of_clauses; i++)
	{
		for (int j = 0; j < I_NUMBER_OF_VARIABLES_IN_CLAUSE; j++)
		{
			i_variables_with_their_clauses[v_clauses[i][j]].push_back(i);
		}
	}
}

int CProblem::iCheckClausesWithVariable(int iVariable, int* pdGenotype)
{
	int i_current_state = 0;

	for (int i = 0; i < i_variables_with_their_clauses[iVariable].size(); i++)
	{
		if (dEvaluateSingleClause(i_variables_with_their_clauses[iVariable][i], pdGenotype) == true)
		{
			i_current_state++;
		}
	}

	return i_current_state;
}


int CProblem::iCompute(int* pdGenotype)
{
	i_number_of_fulfilled_clauses = 0;

	for (int i = 0; i < i_number_of_clauses; i++)
	{
		if (dEvaluateSingleClause(i, pdGenotype) == true)
		{
			i_number_of_fulfilled_clauses++;
		}
	}

	return i_number_of_fulfilled_clauses;
}

double CProblem::dEvaluate(int* pdGenotype)
{ 
	i_number_of_fulfilled_clauses = 0;

	for (int i = 0; i < i_number_of_clauses; i++)
	{
		if (dEvaluateSingleClause(i, pdGenotype) == true)
		{
			i_number_of_fulfilled_clauses++;
		}
	}

	return i_number_of_fulfilled_clauses/ i_number_of_clauses;
}

bool CProblem::dEvaluateSingleClause(int iWhichClause, int* pdGenotype)
{
	for (int i = 0; i < I_NUMBER_OF_VARIABLES_IN_CLAUSE; i++)
	{
		if (v_signs[iWhichClause][i] == true)
		{
			if (pdGenotype[v_clauses[iWhichClause][i]] == 1)
			{
				return true;
			}
		}
		else
		{
			if (pdGenotype[v_clauses[iWhichClause][i]] == 0)
			{
				return true;
			}
		}
	}

	return false;
}

int CProblem::iGetNumberVariables()
{
	return i_number_of_variables;
}

void CProblem::vPrintClauses()
{
	cout << "Number of clauses: " << i_number_of_clauses << '\n';
	cout << "Number of variables: " << i_number_of_variables << '\n';

	for (int i = 0; i < i_number_of_clauses; i++)
	{
		cout << "\n";

		for (int j = 0; j < I_NUMBER_OF_VARIABLES_IN_CLAUSE; j++)
		{
			if (v_signs[i][j])
			{
				cout << v_clauses[i][j] << " ";
			}
			else
			{
				cout << (-1) * v_clauses[i][j] << " ";
			}
		}
	}
}

void CProblem::vPrintClausesInfo()
{
	cout << "Number of clauses: " << i_number_of_clauses << '\n';
	cout << "Number of variables: " << i_number_of_variables << '\n';
}

