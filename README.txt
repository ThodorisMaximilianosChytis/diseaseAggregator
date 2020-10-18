README


K24: System Programming Project2
Thodoris Maximilianos Chytis 
1115201700197





	This project successfully tackles the task of interporcessing communication between a given number of workers and, signal handling for all parties. Furthermore, through a user interface it  provides some applications-queries for the user to choose from. The programming language used is C++.
	Components:
    1) source : diseaseAggregator.cpp diseaseAggregatorfuncts.cpp diseaseAggregatorClasses.cpp diseaseAggregatorDS.cp diseaseAggregatorWorker.cpp diseaseAggregatorWorkerfuncts.cpp
    2) header : diseaseAggregatorClasses.h diseaseAggregatorfuncts.h diseaseAggregatorDS.h diseaseAggregatorWorkerfuncts.h
    3) makefile : 	Makefile   #compiles two programms diseaseAggregator and diseaseAggregatorWorker. The 1st calls the second via execv 
    4) diseasesFile,CountriesFileEU.
    5)bash script: create_infiles.sh #creates the directories and recrods needen for the programma	
    6)logfiles: directory where aggregator and worker logfiles are stored
    7)NamedPipes: directory where the the fifo files for ipc are stored


  


Important Notes:
	-The dates entered follow the format dd/mm/yyyy and date1 < date2.
	-Parent keeps information about children in an array of pointers to Child Classes
	-strings and string::append is used for IPC the protocol is:
		@:end of word
		#:end of message
		$:sigint or sigquit has been sent.
	-No slow worker impacts the communication because first the message is transmitted to the corrrect process,processes (write) while they already proceeed acccomplishing the job
	Then the message from each process is read.
	-if buffersize smaller than message then #buffersize bytes are sent multiple times until message is complete
	-Workers are on the toes for new messages through while loop. as soon as they are sent one, they start working. Parent send messages only to specific worker if needed.
	-signals: i)If sigint or sigquit is sent to worker before entering the IPC loop only one child is forked 			before entering the loop
			  ii)if they are sent after entering the sigint sigquit then they are considered working and will be terminated smoothly only after completing a given job(multiple workers can be sigintorquited the Refork() will occur for them after completing the next given task.)
			  iii)if sigint or sigquit is sent to parent before IPC loop then it exits right away.
			  iv)if they are sent during IPC loop then task will be completed first.
			  v)sigusr1 is sent from worker to parent. if the parent is waiting for input, the addition of the new file will be arranged after the completion of the next query sent
	-The Data structures used by each worker are from first project


Compilation:

#pwd is ThodorisMaxmilianosChytis-Project2

-bash file: 
	
	./create_infiles.sh diseasesFile countriesFileEU input_dir 10 10


-diseaseAggregator:	
	
	make
	./diseaseAggregator -b 1 -w 4 -i input_dir
	make clean
