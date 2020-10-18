all: diseaseAggregator diseaseAggregatorWorker

objects1=diseaseAggregator.o diseaseAggregatorDS.o diseaseAggregatorfuncts.o diseaseAggregatorClasses.o
objects2=diseaseAggregatorWorker.o diseaseAggregatorDS.o diseaseAggregatorWorkerfuncts.o 

diseaseAggregator: $(objects1)
	g++ -std=c++11 -o diseaseAggregator $(objects1)

diseaseAggregatorDS.o: diseaseAggregatorDS.cpp
	g++ -std=c++11 -c diseaseAggregatorDS.cpp

diseaseAggregatorfuncts.o: diseaseAggregatorfuncts.cpp
	g++ -std=c++11 -c diseaseAggregatorfuncts.cpp

diseaseAggregatorClasses.o: diseaseAggregatorClasses.cpp
	g++ -std=c++11 -c diseaseAggregatorClasses.cpp

diseaseAggregator.o: diseaseAggregator.cpp
	g++ -std=c++11 -c diseaseAggregator.cpp



diseaseAggregatorWorker :$(objects2)
	g++ -std=c++11 -o diseaseAggregatorWorker $(objects2)



diseaseAggregatorWorkerfuncts.o :diseaseAggregatorWorkerfuncts.cpp
	g++ -std=c++11 -c diseaseAggregatorWorkerfuncts.cpp



diseaseAggregatorWorker.o :diseaseAggregatorWorker.cpp
	g++ -std=c++11 -c diseaseAggregatorWorker.cpp




clean  :
	rm diseaseAggregator $(objects1)
	rm diseaseAggregatorWorker diseaseAggregatorWorker.o diseaseAggregatorWorkerfuncts.o #diseaseAggregatorDS.h.gch diseaseAggregatorClasses.h.gch
	rm ./logfiles/* ./NamedPipes/*