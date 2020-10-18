#!/bin/bash



CreateRandomDate() {
	
	local day=$((RANDOM%30+1))
	local month=$((RANDOM%12+1))
	local year=$((RANDOM%1020+1000))
	
	if (( $month==2 && $day>28 )); then		#february problem
		if (( $year%4==0 )); then
			day=29
		else
			day=28
		fi
	fi

	rdate=$(date -d "$year-$month-$day" '+%d-%m-%Y')	#generate random date and transform into correct format
	echo "$rdate"
}

CreateRandomRecords(){
	
	local EnterOrExit
	# local first_name="$(cat /dev/urandom | tr -dc 'A-Z' | head -c $((RANDOM%10+3)) )"
	# local last_name="$(cat /dev/urandom | tr -dc 'A-Z' | head -c $((RANDOM%10+3)) )"
	local first_name=${first_names[$((RANDOM%${#first_names[@]}))]}
	local last_name=${last_names[ $((RANDOM%${#first_names[@]})) ]}

	local Age="$((RANDOM%121+1))"
	local disease="$(echo -n "$(shuf -n 1 $3)")"

	local p=2
	local flag=-1
	for ((j=1; j<=$2; j++)); do
		if ((flag==1)); then
			p=6
		fi
		if (( $((RANDOM%10)) < $p )); then			#write ENTER OR EXIT changing propabilities
			EnterOrExit="EXIT"
			flag=0
		else
			EnterOrExit="ENTER"
			flag=1
		fi
		records+=("$1 $EnterOrExit $first_name $last_name $disease $Age")
	done		
	
	# for l in "${records[@]}";
	# 	do
	# 		echo "$l"
	# done
	# echo -n "$1 " >> $recordfilepath		#write id

}



scriptName=$0
# echo $scriptName
diseasesFile=$1
countriesFile=$2
input_dir=$3
numFilesPerDirectory=$4
numRecordsPerFile=$5



if ! [[ $numFilesPerDirectory =~ ^[0-9]+$ ]]; then
	echo "wrong number input for numFilesPerDirectory"
	exit 1
fi

if ! [[ $numRecordsPerFile =~ ^[0-9]+$ ]]; then
	echo "wrong number input for numRecordsPerFile"
	exit 1
fi



if [ ! -f ./$diseasesFile ]; then
	echo "$diseasesFile not found"
	exit 1
fi

if [ ! -f ./$countriesFile ]; then
	echo "$countriesFile not found"
	exit 1
fi

#create input_dir
mkdir -p ./$input_dir


#read from countriesFile

recordid=0

declare -a first_names 
declare -a last_names 

first_names+=(James Patricia Robert Linda William Barbara Richard Jessica Thomas Karen Christopher Margaret Matthew Betty Donald Sandra Paul Kimberly Andrew Emily Joshua Carol Amanda Brian Deborah Ronald Rebecca Jason Sharon Ryan Kathleen Gary Amy Eric Angela Jonathan Brenda Justin Nicole Brandon Katherine Benjamin Christine Samuel Catherine Patrick Virginia Jack)
last_names+=(Smith Johnson Williams Brown Jones Garcia Miller Davis Rodriguez Martinez Hernandez Lopez Gonzales Wilson Anderson Thomas Taylor Moore Jackson Martin Lee Perez Thompson White Harris Sanchez Clark Ramirez Lewis Robinson Walker Young Allen King Wright Scott Nguyen Hill Flores Green Adams Nelson Baker Hall Rivera Campbell Mitchell Carter Roberts)
	




while read p; do
	
	declare -a indeces		#set arrays
	declare -a records
	
	#create a folder fow each country
	mkdir -p ./$input_dir/$p
	rm -f ./$input_dir/$p/* #overwrite

	for ((c=1; c<=$numFilesPerDirectory; c++)); do
		
		while : ; do
			rdate=$(CreateRandomDate)
			if [ ! -f ./$input_dir/$p/$rdate ]; then
				break
			fi			
		done

		touch ./$input_dir/$p/$rdate 	#	create a file with random name
		odd=2
		for ((i=1; i<=numRecordsPerFile/2; i++)); do
			
			let recordid+=1
			
			CreateRandomRecords $recordid $odd "./$diseasesFile"

		done
		
		if ((numRecordsPerFile%2==1)) ; then 
			let recordid+=1
			
			odd=1
			CreateRandomRecords $recordid $odd "./$diseasesFile"
		fi	

	done

	
	# for l in "${!records[@]}";
	# 	do
	# 		echo $l
	# 		# echo "${records[$l]}"
	# done

	indeces=( $(shuf -e "${!records[@]}") )	#create shuffled array of the recrod array indeces
	
	index=0
	for datefile in ./$input_dir/$p/* ; do
	# echo $datefile	
				
			for ((i=1; i<=numRecordsPerFile; i++)); do

					# echo $index
					# echo ${records[indeces[index]]} # echo ${records[index]}
					echo ${records[indeces[index]]} >> $datefile		#insert random record from same directory in the file
					index=$((index+1))
			done


	done	




	unset records 		#delete arrays 
	unset indeces

done<$countriesFile

echo "File creation complete"






