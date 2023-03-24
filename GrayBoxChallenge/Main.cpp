#include "Problem.h"
#include "Optimizer.h"
#include "Timer.h"

#include <exception>
#include <iostream>

using namespace TimeCounters;

using namespace std;

#define dMAX_TIME 1 * 60


void vRunExperiment(CProblem *pcProblem)
{
	try
	{
		CTimeCounter c_time_counter;

		double d_time_passed;

		COptimizer c_optimizer(pcProblem);

		c_time_counter.vSetStartNow();


		c_optimizer.vInitialize();


		c_time_counter.bGetTimePassed(&d_time_passed);

		cout << "\nProgram will run for " << dMAX_TIME << " seconds\n";

		while (d_time_passed <= dMAX_TIME)
		{
			c_optimizer.vRunIteration();
			c_optimizer.pcGetBestFound();

			c_time_counter.bGetTimePassed(&d_time_passed);
		}//while (d_time_passed <= MAX_TIME)

		cout << "Number of  fulfilled clauses: " << c_optimizer.pcGetBestFound()->iFitness() << "\n";

	}//try
	catch (exception &c_exception)
	{
		cout << c_exception.what() << endl;
	}//catch (exception &c_exception)
}//void vRunExperiment(const CEvaluator &cConfiguredEvaluator)

void checkWycieki()
{
	CProblem c_problem;

	if (c_problem.bLoad("m3s_350_37") == true)
	{
		vRunExperiment(&c_problem);
	}//if (c_problem.bLoad("m3s_100_2") == true)
}

void main(int iArgCount, char **ppcArgValues)
{
	checkWycieki();
}//void main(int iArgCount, char **ppcArgValues)
