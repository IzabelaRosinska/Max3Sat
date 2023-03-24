#include "Optimizer.h"
#include <cfloat>
#include <iostream>
#include <windows.h>

COptimizer::COptimizer(CProblem* pcProblem)
{
	pc_problem = pcProblem;
	pc_best_found = NULL;
	i_size_of_population = I_DEFAULT_SIZE_OF_POPULATION;
	i_number_of_populations = I_DEFAULT_SIZE_OF_POPULATION;
	i_current_best_individual_fitness = 0;
	v_populations.resize(I_DEFAULT_NUMBER_OF_POPULATIONS);

	c_rand_engine.seed(random_device()());
	c_rand_engine.discard(1500);
}

COptimizer::~COptimizer()
{
	for (int i = 0; i < i_number_of_populations; i++)
	{
		vDeletePopulation(v_populations[i]);
	}

	delete pc_best_found;
}

void COptimizer::vDeletePopulation(vector<CIndividual*>& vPopulation)
{
	for (int i = 0; i < I_DEFAULT_SIZE_OF_POPULATION; i++)
	{
		if (vPopulation[i] != NULL)
		{
			delete vPopulation[i];
		}
	}
}

void COptimizer::vInitialize()
{
	v_populations.resize(i_number_of_populations);

	for (int i = 0; i < i_number_of_populations; i++)
	{
		vector<CIndividual*> vPopulation;
		for (int j = 0; j < I_DEFAULT_SIZE_OF_POPULATION; j++)
		{
			vPopulation.push_back(new CIndividual(pc_problem));
			vPopulation[j]->vSetRandomValues(c_rand_engine);
		}

		vFindBest(vPopulation);

		v_populations[i] = vPopulation;
	}

}


void COptimizer::vRunIteration()
{
	for (int i = 0; i < i_number_of_populations; i++)
	{	
		vRunIterationOnePopulation(v_populations[i]);
	}
}

void COptimizer::vRunIterationOnePopulation(vector<CIndividual*>& vPopulation)
{
	int i_index_parent_1;
	int i_index_parent_2;
	vector<CIndividual*> pc_children;
	vector<CIndividual*> c_new_population;

	while (c_new_population.size() < i_size_of_population)
	{
		i_index_parent_1 = iChooseParent(vPopulation);
		i_index_parent_2 = iChooseParent(vPopulation);

		pc_children = *(vPopulation[i_index_parent_1]->pcCrossover(vPopulation[i_index_parent_2], c_rand_engine));

		pc_children[0]->vMutation(c_rand_engine);
		pc_children[1]->vMutation(c_rand_engine);

		c_new_population.push_back(pc_children[0]);
		c_new_population.push_back(pc_children[1]);
	}

	vDeletePopulation(vPopulation);
	vPopulation = c_new_population;

	vFindBest(vPopulation);
}

int COptimizer::iChooseParent(vector<CIndividual*>& vPopulation)
{
	uniform_int_distribution<> i_parent_index(0, i_size_of_population - 1);
	int i_index_of_fittest_parent = 0, i_fitness_of_fittest_parent = 0, i_current_parent = 0, i_current_fitness = 0;

	for (int i = 0; i < I_DEFAULT_NUMBER_OF_PARENTS_COSIDERED; i++)
	{
		i_current_parent = i_parent_index(c_rand_engine);
		i_current_fitness = vPopulation[i_current_parent]->iFitness();

		if (i_current_fitness > i_fitness_of_fittest_parent)
		{
			i_fitness_of_fittest_parent = i_current_fitness;
			i_index_of_fittest_parent = i_current_parent;
		}
	}

	return i_index_of_fittest_parent;

}

void COptimizer::vFindBest(vector<CIndividual*>& vPopulation)
{
	int i_index_of_fittest_parent = 0, i_fitness_of_fittest_parent = 0, i_current_fitness = 0;

	for (int i = 0; i < signed(vPopulation.size()); i++)
	{
		i_current_fitness = vPopulation[i]->iFitness();

		if (i_current_fitness > i_fitness_of_fittest_parent)
		{
			i_fitness_of_fittest_parent = i_current_fitness;
			i_index_of_fittest_parent = i;
		}
	}

	if (i_fitness_of_fittest_parent > i_current_best_individual_fitness)
	{
		i_current_best_individual_fitness = i_fitness_of_fittest_parent;

		delete pc_best_found;
		pc_best_found = new CIndividual(*vPopulation[i_index_of_fittest_parent]);
	}
}

CIndividual* COptimizer::pcGetBestFound() 
{ 
	return pc_best_found;
}

CIndividual::CIndividual(CProblem* pcProblem)
{
	pc_problem = pcProblem;
	i_length = pcProblem->iGetNumberVariables();
	pi_genotype = new int[i_length];
}

CIndividual::CIndividual(const CIndividual &pcOther)
{
	pc_problem = pcOther.pc_problem;
	i_length = pcOther.i_length;
	pi_genotype = new int [i_length];

	for (int i = 0; i < pcOther.i_length; i++)
	{
		pi_genotype[i] = pcOther.pi_genotype[i];
	}
}

CIndividual::~CIndividual()
{
	delete[] pi_genotype;
}

void CIndividual::vSetRandomValues(Random& rand)
{
	uniform_int_distribution<int> i_new_gene(0, 1);

	for (int i = 0; i < i_length; i++)
	{
		pi_genotype[i] = i_new_gene(rand);
	}
}

void CIndividual::vPrintPopulation()
{
	for (int i = 0; i < i_length; i++)
	{
		cout << pi_genotype[i] << " ";
	}

	cout << "\n";
}

vector<CIndividual*>* CIndividual::pcCrossover(const CIndividual *pcOther, Random& rand)
{
	uniform_real_distribution<> d_probability(0.0, 1.0);
	vector<CIndividual*> pc_children;
	pc_children.resize(I_DEFAULT_NUMBER_OF_PARENTS_COSIDERED);
	pc_children[0] = new CIndividual(*this);
	pc_children[1] = new CIndividual(*pcOther);

	if (d_probability(rand) < D_DEFAULT_PROBABILITY_OF_CROSSOVER)
	{
		for (int i = 0; i < i_length; i++)
		{
			if (d_probability(rand) < D_DEFAULT_PROBABILITY_OF_INITIALIZATION)
			{
				pc_children[0]->pi_genotype[i] = pcOther->pi_genotype[i];
				pc_children[1]->pi_genotype[i] = pi_genotype[i];
			}

		}
	}

	return &pc_children;
}

void CIndividual::vMutation(Random& rand)
{
	uniform_real_distribution<> d_probability(0.0, 1.0);
	int i_current_state_for_variable;

	if (d_probability(rand) < D_DEFAULT_PROBABILITY_OF_MUTATION_HAPPENING)
	{
		for (int i = 0; i < i_length; i++)
		{
			if (d_probability(rand) < D_DEFAULT_PROBABILITY_OF_MUTATION)
			{
				i_current_state_for_variable = pc_problem->iCheckClausesWithVariable(i, pi_genotype);
				vSwitchGeneValue(i);


				if (pc_problem->iCheckClausesWithVariable(i, pi_genotype) < i_current_state_for_variable)
				{
					vSwitchGeneValue(i);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < i_length; i++)
		{
			if (d_probability(rand) < D_DEFAULT_PROBABILITY_OF_MUTATION)
			{
				vSwitchGeneValue(i);
			}
		}
	}
}

void CIndividual::vSwitchGeneValue(int index)
{
	if (pi_genotype[index] == 0)
	{
		pi_genotype[index] = 1;
	}
	else
	{
		pi_genotype[index] = 0;
	}
}


int CIndividual::iFitness()
{
	return pc_problem->iCompute(pi_genotype);
}