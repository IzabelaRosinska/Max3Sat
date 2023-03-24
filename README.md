# Max3Sat
Genetic algorithm for optimization of Max3Sat.

Genetic algorithm <br>
Charles Darwin’s theory is present in IT. Genetic algorithms are inspired by the way population evolves. They have same starting population of solutions (which we call individuals) and through generations these solutions cross with each other and mutate as real-life individuals in population would do. This approach allows optimization of many NP (nondeterministic polynomial time) problems, Max3Sat being one of them. Starting parameters of population influence the way in which individuals interact with other, which is visualized by the achieved results.


Max3Sat <br>
Many business problems can be broke down to Max3Sat. Max3Sat consists of clauses. Each clause is made of three bool variables, which can be negated or not. The clause is satisfed when in clause at least one variable is True or at least one negated variable is False.


File with problem <br>
Only .txt files with problems are allowed. There is one type of formatting allowed: '( 1 2 -3 )'. Numbers refer to variables' indexes and '-' tells that the variable is negated.


Size of population <br>
Tells how many individuals make up a population. For example size of population equal 200 means, that there are 200 individuals in this population at each point of simulation.


Number of populations <br>
Tells how many parallel populations evolve at the time. For number of populations equal 5 means, that there are 5 separated populations at each point of simulation.


Number of parents considered <br>
Tells how many individuals fight with each other to become a parent to new individual. Each individaul (besides starting population) has two parents. If two individuals are considered to become a parent for new solution, wins the one which is a better solution to the problem.


Probability of crossover <br>
Each pair of parents will have two children. Child1 and Child2 can either be the exact copies of Parent1 and Parent2 or they can be the cross of Parent1 or Parent2 genotypes. This statistic describes how big chances are that genotypes of Parent1 and Parent2 will be crossed.


Probability of gene crossover <br>
When Parent1 and Parent2 genotypes cross, this statistic tells how probable is that Child1 same as Parent1 will take this peculiar gene from Parent2. The same with Child2.


Probability of smart mutation <br>
As in real life, beside genetic variability coming from cross of genotypes of Parent1 and Parent2, there are some random changes in genotype of the child. This statistic tells how probable is that this changes won't be completely random but will only enhance the quality of solution.


Probability of gene mutation <br>
Tells how probable is that single gene in genotype will be mutated.
