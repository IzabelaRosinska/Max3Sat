#pragma once
#include "Problem.h"
#include <random>
#include <vector>
using namespace std;

using Random = mt19937;

const double D_DEFAULT_PROBABILITY_OF_MUTATION_HAPPENING = 0.8;
const double D_DEFAULT_PROBABILITY_OF_MUTATION = 0.3;

const double D_DEFAULT_PROBABILITY_OF_CROSSOVER = 0.5;
const double D_DEFAULT_PROBABILITY_OF_INITIALIZATION = 0.5;

const int I_DEFAULT_NUMBER_OF_PARENTS_COSIDERED = 2;
const int I_DEFAULT_NUMBER_OF_CHILDREN = 2;

const int I_DEFAULT_SIZE_OF_POPULATION = 80;
const int I_DEFAULT_NUMBER_OF_POPULATIONS = 10;

class  CIndividual;

class COptimizer
{
public:
	COptimizer(CProblem *pcProblem);
	~COptimizer();
	void vDeletePopulation(vector<CIndividual*>& vPopulation);

	void vInitialize();

	void vRunIteration();
	void vRunIterationOnePopulation(vector<CIndividual*>& vPopulation);

	int iChooseParent(vector<CIndividual*>& vPopulation);

	void vFindBest(vector<CIndividual*>& vPopulation);

	CIndividual* pcGetBestFound();

private:
	CProblem *pc_problem;
	mt19937 c_rand_engine;
	CIndividual  *pc_best_found;
	int i_size_of_population;
	int i_current_best_individual_fitness;
	int i_number_of_populations;
	vector<vector<CIndividual*>> v_populations;

};//class COptimizer


class CIndividual
{
public:
	CIndividual(CProblem *pcProblem);
	CIndividual(const CIndividual &pcOther);
	~CIndividual();

	void vSetRandomValues(Random& rand);
	void vPrintPopulation();

	vector<CIndividual*>* pcCrossover(const CIndividual *pcOther, Random& rand);

	void vMutation(Random& rand);
	void vMutationReversed(Random& rand);
	void vMutationRandom(Random& rand);

	int iFitness();

	void vSwitchGeneValue(int index);

private:
	CProblem* pc_problem;
	int* pi_genotype;
	int i_length;
};//class  CIndividual